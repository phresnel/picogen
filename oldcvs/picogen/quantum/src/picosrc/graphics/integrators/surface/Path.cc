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

#include <picogen/picogen.h>
#include <picogen/graphics/integrators/surface/Path.h>

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace surface {

                Path::Path() : intersectable (0) {
                }



                void Path::setIntersectable (const ::picogen::graphics::objects::abstract::IIntersectable *intersectable) {
                    this->intersectable = intersectable;
                }



                bool Path::integrate (::picogen::graphics::structs::sample &sample) const {
                    /*sample.primaryIntersection->t = 10;
                    sample.primaryIntersection->normal = Vector3d (0, 1, 0);*/
                    if (0 != intersectable) {
                        if (intersectable->intersect (*sample.primaryIntersection, sample.cameraRay)) {
                            sample.color = sample.primaryIntersection->color;
                        }
                    }
                    //Vector3d w = sample.cameraRay.getDirection();
                    //sample.color = ::picogen::graphics::color::Color (w [0]+0.5, w [1]+0.5, w [2]+0.5);
                    return true;
                }

            } // namespace surface {
        } // namespace integrators {
    } // namespace graphics {
} // namespace picogen {

