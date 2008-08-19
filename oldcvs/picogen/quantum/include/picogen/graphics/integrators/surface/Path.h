/***************************************************************************
 *            Path.h
 *
 *  2008-08-18 19:10:00 2008
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

#ifndef PATH_H__INCLUDED
#define PATH_H__INCLUDED

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace surface {
                class Path : public ISurfaceIntegrator {
                    private:
                        typedef ::picogen::real real;
                        typedef ::picogen::geometrics::Vector3d Vector3d;
                        typedef ::picogen::geometrics::Ray Ray;
                        typedef ::picogen::geometrics::BoundingBox BoundingBox;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;
                        typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                        typedef ::picogen::graphics::objects::abstract::ISky ISky;
                        typedef ::picogen::graphics::color::Color Color;
                        typedef ::picogen::geometrics::Transformation Transformation;
                    private:
                        const ::picogen::graphics::objects::abstract::IIntersectable *intersectable;
                    public:
                        Path ();
                        void setIntersectable (const ::picogen::graphics::objects::abstract::IIntersectable *intersectable);
                        bool integrate (::picogen::graphics::structs::sample &sample) const;
                }; // class Path
            } // namespace surface {
        } // namespace integrators {
    } // namespace graphics {
} // namespace picogen {

#endif // PATH_H__INCLUDED
