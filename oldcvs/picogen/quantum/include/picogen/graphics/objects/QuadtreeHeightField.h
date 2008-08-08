/***************************************************************************
 *            QuadtreeHeightField.h
 *
 *  2008-08-08
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


#ifndef _QUADTREEHEIGHTFIELD_H
#define _QUADTREEHEIGHTFIELD_H

#include <picogen/graphics/objects.h>

namespace picogen {
    namespace graphics {
        namespace objects {

            class SimpleHeightField;

            class QuadtreeHeightField : public ::picogen::graphics::objects::abstract::IIntersectable {
                private:
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;

                    SimpleHeightField *field;

                public:
                    QuadtreeHeightField (unsigned int minSize, SimpleHeightField *simpleHeightField);
                    virtual ~QuadtreeHeightField();
                    virtual bool intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const;
            };

        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {



#endif /* _QUADTREEHEIGHTFIELD_H */
