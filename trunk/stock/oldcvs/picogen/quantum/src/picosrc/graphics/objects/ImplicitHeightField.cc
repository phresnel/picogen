/***************************************************************************
 *            ImplicitHeightField.cc
 *
 *  Wed Aug 06 2008
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

namespace picogen {
    namespace graphics {
        namespace objects {



            ImplicitHeightField::ImplicitHeightField() : brdf(0), shader(0), function(0) {
            }



            ImplicitHeightField::~ImplicitHeightField() {
            }



            void ImplicitHeightField::setBox (param_in (Vector3d, min), param_in (Vector3d, max)) {
                box.setMin (min);
                box.setMax (max);
                boxSize = max - min;
                boxSizeInv = Vector3d (1.0 / boxSize [0], 1.0 / boxSize [2], 1.0 / boxSize [2]);
            }



            void ImplicitHeightField::setBRDF (const IBRDF* brdf) {
                this->brdf = brdf;
            }



            void ImplicitHeightField::setShader (const IShader* shader) {
                this->shader = shader;
            }



            void ImplicitHeightField::setFunction (const Function_R2_R1* function) {
                this->function = function;
            }




            bool ImplicitHeightField::intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const {

                using namespace ::picogen::constants;

                // Intersect bounding box; eventually return.
                real t_min, t_max;
                if (!box.intersect (t_min, t_max, ray))
                    return false;
                // Saturate t_min to 0; eventually return.
                t_min = t_min < epsilon ? epsilon : t_min;
                if (t_min > t_max)
                    return false;

                /*const Vector3d gbegin_ = ray (t_min) - box.getMin();
                const Vector3d gend_ = ray (t_max) - box.getMin();
                const Vector3d gbegin (gbegin_[0]*boxSizeInv[0], gbegin_[1]*boxSizeInv[1], gbegin_[2]*boxSizeInv[2]);
                const Vector3d gbegin (gend_[0]*boxSizeInv[0], gend_[1]*boxSizeInv[1], gend_[2]*boxSizeInv[2]);*/


                intersection.side = ::picogen::constants::outside;
                intersection.normal = -ray.getDirection();
                //intersection.t = 100.0;
                intersection.color = color::Color (1,1,1);
                intersection.brdf = brdf;
                intersection.L_e = 0.0;

                int prev_sign;//(*function) (ray.getPosition()[0], ray.getPosition()[2]) - ray.getPosition()[1];
                {
                    const Vector3d curr_p = ray (t_min);

                    const real x = (curr_p[0] - box.getMin()[0]) * boxSizeInv[0];
                    const real y = (curr_p[1] - box.getMin()[1]) * boxSizeInv[1];
                    const real z = (curr_p[2] - box.getMin()[2]) * boxSizeInv[2];

                    const real prev_h = (*function) (x, z) - y;
                    prev_sign = prev_h > 0 ? -1 : 1;
                }

                const real step = 0.5;
                for (real t=step+t_min; t<t_max; t+=step) {

                    const Vector3d curr_p = ray (t);

                    const real x = (curr_p[0] - box.getMin()[0]) * boxSizeInv[0];
                    const real y = (curr_p[1] - box.getMin()[1]) * boxSizeInv[1];
                    const real z = (curr_p[2] - box.getMin()[2]) * boxSizeInv[2];

                    const real curr_h = (*function) (x, z) - y;
                    const int curr_sign = curr_h > 0 ? -1 : 1;

                    if (curr_sign != prev_sign) { // <-- Sign Flip.
                        intersection.t = t;

                        const real uh = (*function) (x+epsilon, z) - curr_h;
                        const real vh = (*function) (x, z+epsilon) - curr_h;

                        {
                            static const real epsilon = 0.1;
                            const Vector3d u (epsilon, uh, 0.0);
                            const Vector3d v (0.0, vh, epsilon);
                            intersection.normal = u.cross (v).computeNormal();
                        }
                        return true;
                    }
                    prev_sign = curr_sign;
                }
                //intersection.t = t_min;
                return false;
            }




        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {
