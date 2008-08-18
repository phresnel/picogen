/***************************************************************************
 *            DistanceVis.cc
 *
 *  2008-08-17 15:09:00 2008
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
#include <picogen/graphics/film/DistanceVis.h>

namespace picogen {
    namespace graphics {
        namespace film {

            DistanceVis::DistanceVis(unsigned int width, unsigned int height)
            : width (width), height (height), surface () {
                surface.reset (width, height);
            }



            DistanceVis::~DistanceVis() {
            }



            void DistanceVis::addSample (param_in (::picogen::graphics::structs::sample, sample)) {
                if (0 != sample.primaryIntersection) {
                    surface (sample.x, sample.y) += ::picogen::graphics::color::Color(
                        sample.primaryIntersection->t*0.1,
                        sample.primaryIntersection->t*0.1,
                        sample.primaryIntersection->t*0.1
                    );
                } else {
                    surface (sample.x, sample.y) += ::picogen::graphics::color::Color(0,0,0);
                }
            }



            ::picogen::graphics::color::Color DistanceVis::operator () (unsigned int x, unsigned int y) const {
                return surface (x, y);
            }



            unsigned int DistanceVis::getWidth() const {
                return width;
            }


            unsigned int DistanceVis::getHeight() const {
                return height;
            }

        } // namespace film {
    } // namespace graphics {
} // namespace picogen {

