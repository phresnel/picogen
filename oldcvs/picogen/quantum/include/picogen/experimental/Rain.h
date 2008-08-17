/***************************************************************************
 *            Snow.h
 *
 *  Tue Apr 18 18:40:00 2008
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


#ifndef _RAIN_H
#define _RAIN_H


namespace picogen {
    namespace graphics {
        namespace objects {

            class Rain : public ::picogen::graphics::objects::abstract::IIntersectable {
                private:
                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;
                    typedef ::picogen::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                    typedef ::picogen::graphics::color::Color Color;
                    typedef ::picogen::misc::functions::vector_to_scalar::PerlinNoise PerlinNoise;

                private:

                public:
                    Rain();
                    virtual ~Rain();
                    virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const;
            };

        }
    }
} // namespace picogen{ namespace graphics{ namespace objects{

#endif /* _SNOW_H */

