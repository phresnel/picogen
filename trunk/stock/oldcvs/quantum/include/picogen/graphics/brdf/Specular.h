/***************************************************************************
 *            Specular.h
 *
 *  Jul 01 17:00:00 2008
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


#ifndef _SPECULAR_H
#define _SPECULAR_H

namespace picogen {
    namespace graphics {
        namespace material {
            namespace brdf {

                class Specular : public ::picogen::graphics::material::abstract::IBRDF {
                    private:
                        typedef ::picogen::real real;
                        typedef ::picogen::geometrics::Vector3d Vector3d;
                        typedef ::picogen::geometrics::Ray Ray;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;

                        const real reflectance;
                        mutable bool RR;
                        ::picogen::generators::rng::IRNG *rng;

                    public:
                        Specular();
                        Specular( real reflectance );

                        bool enableRussianRoulette (bool enable) const;
                        void setRNG (::picogen::generators::rng::IRNG *rng);

                        bool randomSample (
                            param_out (real,brdf),
                            param_out (real,p),
                            param_out (bool,specular),
                            param_out (Ray,r_out),
                            param_in (Ray,r_in),
                            param_in (Vector3d,N)
                        ) const;

                        bool isSpecular () const;
                        ::picogen::real getReflectance () const;
                };

            } // namespace brdf {
        } // namespace material {
    } // namespace graphics {
} // namespace picogen {

#endif // _LAMBERTIAN_H
