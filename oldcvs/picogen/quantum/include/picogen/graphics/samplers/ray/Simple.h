/***************************************************************************
 *            path_integrator_WhittedStyle.h
 *
 *  Sun Aug 12 12:08:41 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
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


#ifndef _RAYSAMP_SIMPLE_H
#define _RAYSAMP_SIMPLE_H

namespace picogen {
    namespace graphics {
        namespace samplers {
            namespace ray {
                class Simple {
                    private:
                        /// \todo only typedef in the actually needed types
                        typedef ::picogen::misc::prim::real real;
                        typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                        typedef ::picogen::misc::geometrics::Ray Ray;
                        typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;
                        typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                        typedef ::picogen::graphics::objects::abstract::ISky ISky;
                        typedef ::picogen::graphics::image::color::Color Color;
                        typedef ::picogen::misc::geometrics::Transformation Transformation;

                        IIntersectable *m_intersectable;
                        ISky *m_skyShader;
                        Color L_i (param_in (Ray,ray), bool specularOrFirst, param_out (intersection_t,primaryIntersection), unsigned int max);
                    public:
                        Simple (IIntersectable *intersectable);
                        Simple();
                        virtual ~Simple();
                        void setIntersectable (IIntersectable *intersectable);
                        void setSky (ISky *skyShader);
                        bool integrate (param_out (Color, color), param_out (intersection_t,primaryIntersection), param_in (Ray,ray));
                };


            } // namespace ray {
        } // namespace samplers {
    } // namespace graphics {
} // namespace picogen {


#endif /* _RAYSAMP_SIMPLE_H */
