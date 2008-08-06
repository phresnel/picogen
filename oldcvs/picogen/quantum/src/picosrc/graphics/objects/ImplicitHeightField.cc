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
                boxMin = min;
                boxMax = max;
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
                intersection.side = ::picogen::misc::constants::outside;
                intersection.normal = -ray.getDirection();
                intersection.t = 100.0;
                intersection.color = image::color::Color (1,1,1);
                intersection.brdf = brdf;
                intersection.L_e = 0.0;
                return false;
            }




        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {
