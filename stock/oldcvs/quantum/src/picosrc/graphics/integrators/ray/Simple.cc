/***************************************************************************
 *            Simple.cc
 *
 *  Sun Aug 12 12:08:57 2007
 *  Copyright  2007  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <picogen/picogen.h>

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace ray {


                Simple::Simple (IIntersectable *intersectable)
                        : m_intersectable (intersectable), m_skyShader (0) {
                }

                Simple::Simple()
                        : m_intersectable (0), m_skyShader (0) {
                }

                Simple::~Simple() {
                }

                void Simple::setIntersectable (IIntersectable *intersectable) {
                    XRT_CALL_STACK_PUSH ("void WhittedStyle::SetIntersectable( abstract::IIntersectable *intersectable )");
                    m_intersectable = intersectable;
                    XRT_CALL_STACK_POP();
                }

                void Simple::setSky (ISky *skyShader) {
                    XRT_CALL_STACK_PUSH ("void WhittedStyle::SetIntersectable( abstract::IIntersectable *intersectable )");
                    m_skyShader = skyShader;
                    XRT_CALL_STACK_POP();
                }

                graphics::color::Color Simple::L_i (param_in (Ray,ray), bool specularOrFirst, param_out (intersection_t,primaryIntersection), unsigned int max) {
                    using namespace constants;

                    primaryIntersection.t = -1;
                    intersection_t I;

                    if (max == 0) {
                        return Color (0.0,0.0,0.0);
                    } else if (0==m_intersectable || !m_intersectable->intersect (I, ray)) {
                        //> apply sky color if there is no object to intersect or no intersection
                        if (0 != m_skyShader) {
                            Color skyColor, sunColor;
                            m_skyShader->shade (skyColor, ray);
                            if (specularOrFirst) {
                                m_skyShader->sunShade (sunColor, ray);
                                return skyColor + sunColor;
                            } else {
                                return skyColor;
                            }
                        } else {
                            return Color (0.0,0.0,0.0);
                        }
                    } else {
                        //> sample new ray and get brdf/probability
                        primaryIntersection = I;
                        real BRDF,pdf;
                        Ray  r_out,  r_in (ray (I.t), ray.w());
                        bool specular = false;
                        if (I.brdf->randomSample (BRDF, pdf, specular, r_out, r_in, I.normal)) {
                            if (specular == false)
                                specularOrFirst = false;

                            Color col (0.0,0.0,0.0);

                            //> do epsilon correction
                            r_out.x() = r_out.x() + I.normal*epsilon;
                            const real dot = fabs (I.normal.normal() * r_out.w().normal());
                            const real f = (BRDF*dot) / pdf;
                            intersection_t dummyi;
                            col += (I.color*I.L_e) + (L_i (r_out,specularOrFirst,dummyi,max-1) * I.color * f);

                            //> atmosphere lighting and shading
                            if (0 != m_skyShader) {
                                if (!specularOrFirst) {
                                    Ray sunRay;
                                    Color sunColor;
                                    real sun_p = 0.0;
                                    m_skyShader->sunSample (sunColor, sunRay, sun_p, r_out.x());
                                    intersection_t tmp_I;
                                    if (sun_p > epsilon && (0==m_intersectable || !m_intersectable->intersect (tmp_I, sunRay))) {
                                        real c = I.normal.normal() * sunRay.w();
                                        col += I.color * sunColor * c;
                                    }
                                }
                                //*/
                                m_skyShader->atmosphereShade (col, col, ray, I.t);
                            }
                            return col;
                        } else {
                            //> has been absorbed
                            return (I.color*I.L_e);
                        }
                    }
                }

                bool Simple::integrate (param_out (Color, color), param_out (intersection_t,primaryIntersection), param_in (Ray,ray)) {
                    XRT_CALL_STACK_PUSH ("bool WhittedStyle::Integrate( param_out(base_types::color, color), param_in(base_types::ray,ray) )");
#ifdef XRT_DEBUG
                    if (0 == m_intersectable)
                        throw exceptions::null_pointer (__FILE__, __LINE__, "WhittedStyle-Integrator Zero Pointer: no intersectable object");
#endif
                    color = L_i (ray,true,primaryIntersection,15);
                    return true;
                }


            }  // namespace ray
        }  // namespace samplers
    }  // namespace graphics
} // namespace picogen
