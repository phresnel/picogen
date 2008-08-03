/***************************************************************************
 *            Sphere.cc
 *
 *  Tue Nov 13 09:25:40 2007
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

using picogen::misc::prim::real;
using picogen::misc::geometrics::Vector3d;
using picogen::misc::geometrics::Ray;
using picogen::misc::geometrics::BoundingBox;
using picogen::graphics::material::abstract::IBRDF;
using picogen::graphics::structs::intersection_t;
using picogen::graphics::image::color::Color;


namespace picogen {
    namespace graphics {
        namespace objects {

            Sphere::Sphere() :
                    x (0.0,0.0,0.0), r (1.0), L_e (0.0),
                    color (1.0,1.0,1.0),
                    brdf (0) {
            }



            void Sphere::setPosition (const Vector3d &pos) {
                x = pos;
            }



            Vector3d Sphere::getPosition() const {
                return x;
            }



            void Sphere::setColor (const Color &c) {
                color = c;
            }



            Color Sphere::getColor() const {
                return color;
            }



            void Sphere::setRadius (real radius) {
                r = radius;
            }



            real Sphere::getRadius() const {
                return r;
            }



            void Sphere::setEmittance (real e) {
                L_e = e;
            }



            real Sphere::getEmittance() const {
                return L_e;
            }



            void Sphere::setBRDF (const IBRDF* b) {
                brdf = const_cast<IBRDF*> (b);
            }



            const IBRDF* Sphere::getBRDF() const {
                return brdf;
            }



            bool Sphere::intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
                intersection.color = Color (1,0,0);

                using namespace picogen::misc::constants;

                side currSide = noside;
                int i;
                Vector3d v;
                real sqrV = 0;
                real b    = 0;

                for (i=0; i<3; i++) {
                    v[i]  = ray.x() [i] - this->x[i];
                    b    -= v[i] * ray.w() [i];
                    sqrV += v[i] * v[i];
                }

                real det = (b * b) - sqrV + r*r;
                real i1, i2;

                real currT = real_max;
                if (det > 0) {
                    det = sqrt (det);
                    i1 = b - det;
                    i2 = b + det;
                    if (i2>0) {
                        if (i1<0) {
                            currT = i2;
                            currSide = inside;
                        } else {
                            currT = i1;
                            currSide = outside;
                        }
                    }
                }
                if (currSide != noside) {
                    if (currSide == inside) {
                        intersection.normal = (x - ray (currT)).normal();
                    } else {
                        intersection.normal = (ray (currT) - x).normal();
                    }
                    intersection.t = currT;
                    intersection.color = color;
                    intersection.brdf = brdf;
                    intersection.L_e = L_e;
                }
                return currSide != noside;
            }



        }; // namespace objects
    }; // namespace graphics
}; // namespace picogen
