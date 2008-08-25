/***************************************************************************
 *            Whitted.h
 *
 *  2008-08-24 13:30:00 2008
 *  Copyright  2008  Sebastian Mach
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
#include <picogen/graphics/integrators/surface/Whitted.h>

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace surface {

                Whitted::Whitted() : sample (0) {
                }



                ::picogen::graphics::color::Color Whitted::L_i (
                    param_in (Ray,ray),
                    bool specularOrFirst,
                    param_out (intersection_t,primaryIntersection),
                    unsigned int max
                ) const {
                    using namespace constants;

                    intersection_t I;
                    primaryIntersection.t = -1;

                    if (max == 0) {
                        return Color (0.0,0.0,0.0);
                    } else if (0==sample->intersectable || !sample->intersectable->intersect (I, ray)) {
                        //> apply sky color if there is no object to intersect or no intersection
                        if (0 != sample->skyShader) {
                            Color skyColor, sunColor;
                            sample->skyShader->shade (skyColor, ray);
                            if (specularOrFirst) {
                                sample->skyShader->sunShade (sunColor, ray);
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
                            if (0 != sample->skyShader) {
                                if (!specularOrFirst) {
                                    Ray sunRay;
                                    Color sunColor;
                                    real sun_p = 0.0;
                                    sample->skyShader->sunSample (sunColor, sunRay, sun_p, r_out.x());
                                    intersection_t tmp_I;
                                    if (sun_p > epsilon && (0==sample->intersectable || !sample->intersectable->intersect (tmp_I, sunRay))) {
                                        real c = I.normal.normal() * sunRay.w();
                                        col += I.color * sunColor * c;
                                    }
                                }
                                //*/
                                sample->skyShader->atmosphereShade (col, col, ray, I.t);
                            }
                            return col;
                        } else {
                            //> has been absorbed
                            return (I.color*I.L_e);
                        }
                    }
                }



                bool Whitted::integrate (::picogen::graphics::structs::sample &sample) const {
                    this->sample = &sample;
                    sample.color = L_i (sample.cameraRay, true, *sample.primaryIntersection, 3);
                    /*if (sample.primaryIntersection->t > 0) {
                        printf ("{%.1f, %.1f, %.1f}", sample.primaryIntersection->normal[0], sample.primaryIntersection->normal[1], sample.primaryIntersection->normal[2]);
                    }*/

                    //Vector3d w = sample.cameraRay.getDirection();
                    //sample.color = ::picogen::graphics::color::Color (w [0]+0.5, w [1]+0.5, w [2]+0.5);
                    return true;//sample.primaryIntersection->t > 0;
                }

            } // namespace surface {
        } // namespace integrators {
    } // namespace graphics {
} // namespace picogen {

