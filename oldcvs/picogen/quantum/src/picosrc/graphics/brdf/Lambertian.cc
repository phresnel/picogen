/***************************************************************************
 *            Lambertian.cc
 *
 *  Jul 01 16:40:00 2008
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
        namespace material {
            namespace brdf {

                Lambertian::Lambertian()
                : reflectance (1.0), rng (0)
                {
                }



                Lambertian::Lambertian (real reflectance)
                : reflectance (reflectance), rng (0)
                {
                }



                void Lambertian::setRNG (::picogen::generators::rng::IRNG *rng) {
                    this->rng = rng;
                }



                bool Lambertian::randomSample (
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

                    using ::picogen::constants::pi;
                    if (rng->randf() > reflectance)
                        return false;
                    r_out.x() = r_in.x();
                    do {
                        r_out.w() = Vector3d (
                            rng->randf()*2.0-1.0,
                            rng->randf()*2.0-1.0,
                            rng->randf()*2.0-1.0
                        );
                    } while (r_out.w().lengthSq() > 1 || N*r_out.w() < 0.0);

                    r_out.w() = r_out.w().normal();
                    p = 1.0 / (2.0*pi);
                    brdf = 1.0/pi;
                    specular = false;
                    return true;
                }


            } // namespace brdf {
        } // namespace material {
    } // namespace graphics {
} // namespace picogen {
