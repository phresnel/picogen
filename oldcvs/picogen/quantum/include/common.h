/***************************************************************************
 *            common.h
 *
 *  2008-05-29 10:00:00
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

#ifndef COMMON_H__INCLUDED
#define COMMON_H__INCLUDED

namespace picogen {
    namespace common {
        typedef ::picogen::misc::prim::real real;

        typedef ::picogen::misc::geometrics::Vector3d Vector3d;
        typedef ::picogen::misc::geometrics::Ray Ray;
        typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
        typedef ::picogen::misc::geometrics::Transformation Transformation;

        typedef ::picogen::graphics::image::color::Color Color;
        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
        typedef ::picogen::graphics::structs::intersection_t intersection_t;

        typedef ::picogen::graphics::objects::LinearList LinearList;
        typedef ::picogen::graphics::objects::TriBIH TriBIH;
        typedef ::picogen::graphics::objects::Preetham Preetham;
        typedef ::picogen::graphics::objects::SimpleHeightField SimpleHeightField;
        typedef ::picogen::graphics::objects::VelocityInstance VelocityInstance;
        typedef ::picogen::graphics::objects::Sphere Sphere;
        typedef ::picogen::graphics::objects::ZBNCubemap ZBNCubemap;
        typedef ::picogen::graphics::objects::AABox AABox;

        typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
        typedef ::picogen::graphics::objects::abstract::ISky ISky;
    } // namespace common
} // namespace common

#endif // #ifndef COMMON_H__INCLUDED
