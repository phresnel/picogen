/***************************************************************************
 *            Sphere.h
 *
 *  Tue Nov 13 09:25:28 2007
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


#ifndef _SPHERE_H
#define _SPHERE_H

namespace picogen {
    namespace graphics {
        namespace objects {



            class Sphere : public ::picogen::graphics::objects::abstract::IIntersectable {
                private:

                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;
                    typedef ::picogen::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::graphics::color::Color Color;

                private:
                    Vector3d x;
                    real     r, L_e;
                    Color    color;
                    const IBRDF *brdf;

                public:
                    Sphere();

                    void     setPosition (const Vector3d &pos);
                    Vector3d getPosition() const;
                    void     setColor (const Color &color);
                    Color    getColor() const;
                    void     setRadius (real r);
                    real     getRadius() const;
                    void     setEmittance (real e);
                    real     getEmittance() const;
                    void     setBRDF (const IBRDF* brdf);
                    const IBRDF*   getBRDF() const;

                    virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const;
            };

        } // namespace objects {
    } // namespace graphics {
}//namespace picogen {


#endif /* _SPHERE_H */
