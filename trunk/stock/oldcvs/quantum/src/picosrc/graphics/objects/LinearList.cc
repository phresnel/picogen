/***************************************************************************
 *            scene_LinearList.cc
 *
 *  Mon Aug 13 15:35:30 2007
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


#include <picogen/picogen.h>

namespace picogen {
    namespace graphics {
        namespace objects {


            LinearList::LinearList() {
            }



            LinearList::~LinearList() {
                flush();
            }



            bool LinearList::intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
                intersection.t = constants::real_max;
                t_objectList::const_iterator it;
                bool any = false;
                for (it=m_list.begin(); it!=m_list.end(); it++) {
                    intersection_t tmp;

                    if ( (*it)->intersect (tmp, ray) && tmp.t<intersection.t) {
                        any = true;
                        intersection = tmp;
                    }
                }
                return any;
            }



            void LinearList::insert (const abstract::IIntersectable*i) {
                m_list.insert (m_list.end(), const_cast<IIntersectable*> (i));
            }



            void LinearList::invalidate() {
                // nothing to do here, keep off
            }



            void LinearList::flush() {
                // TODO URGENT!!!!one delete also the objects themselves
                m_list.clear();
            }


        } // objects
    } // graphics
} // picogen
