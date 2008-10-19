/***************************************************************************
 *            Specular_DistortedHeight.cc
 *
 *  2008-10-19
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
#include <boost/intrusive_ptr.hpp>

namespace picogen {
    namespace graphics {
        namespace material {
            namespace brdf {

                Specular_DistortedHeight::Specular_DistortedHeight (
                    real reflectance,
                    const ::boost::intrusive_ptr<picogen::misc::functional::Function_R2_R1_Refcounted> &fun
                ) : reflectance (1.0), rng (0), fun (fun)
                {
                }



                void Specular_DistortedHeight::setRNG (::picogen::generators::rng::IRNG *rng) {
                    this->rng = rng;
                }



                bool Specular_DistortedHeight::enableRussianRoulette (bool enable) const {
                    bool ret = RR;
                    RR = enable;
                    return ret;
                }



                bool Specular_DistortedHeight::randomSample (
                        param_out (real,brdf),
                        param_out (real,p),
                        param_out (bool,specular),
                        param_out (Ray,r_out),
                        param_in (Ray,r_in),
                        param_in (Vector3d,N)
                ) const {
                    // Tests {
                    assert (0 != this->rng);
                    // }

                    using picogen::constants::pi;
                    if (RR && rng->randf() > reflectance)
                        return false;
                    r_out.x() = r_in.x();
                    r_out.w() = r_in.w() - N.normal() *2* (r_in.w() * N.normal());
                    r_out.w()[1] *= (*fun) (r_in.x()[0], r_in.x()[2]);

                    p = fabs (N.normal() *r_out.w().normal());
                    brdf = RR ? 1.0 : reflectance;
                    specular = true;
                    return true;
                }



                bool Specular_DistortedHeight::isSpecular () const {
                    return true;
                }



                ::picogen::real Specular_DistortedHeight::getReflectance () const {
                    return reflectance;
                }



            } // namespace brdf {
        } // namespace material {
    } // namespace graphics {
} // namespace picogen {
