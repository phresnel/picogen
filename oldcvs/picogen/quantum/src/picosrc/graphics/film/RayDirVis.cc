/***************************************************************************
 *            RayDirVis.cc
 *
 *  2008-08-18 08:00:00 2008
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
#include <picogen/graphics/film.h>
#include <picogen/graphics/film/RayDirVis.h>

namespace picogen {
    namespace graphics {
        namespace film {

            RayDirVis::RayDirVis(unsigned int width, unsigned int height)
            : width (width), height (height), surface () {
                surface.reset (width, height);
            }



            RayDirVis::~RayDirVis() {
            }



            void RayDirVis::addSample (param_in (::picogen::graphics::structs::sample, sample)) {
                ::picogen::geometrics::Vector3d dir = sample.cameraRay.getDirection();
                surface (sample.x, sample.y) += ::picogen::graphics::color::Color(
                    dir [0]+0.5,
                    dir [1]+0.5,
                    dir [2]+0.5
                );
            }



            ::picogen::graphics::color::Color RayDirVis::operator () (unsigned int x, unsigned int y) const {
                return surface (x, y);
            }



            unsigned int RayDirVis::getWidth() const {
                return width;
            }


            unsigned int RayDirVis::getHeight() const {
                return height;
            }

        } // namespace film {
    } // namespace graphics {
} // namespace picogen {

