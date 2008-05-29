/***************************************************************************
 *            scene_LinearList.h
 *
 *  Mon Aug 13 15:35:04 2007
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


#ifndef _LINEARLIST_H
#define _LINEARLIST_H

namespace picogen {
    namespace graphics {
        namespace objects {
            class LinearList : public ::picogen::graphics::objects::abstract::IScene {
                private:
                    typedef ::picogen::misc::prim::real              real;
                    typedef ::picogen::misc::geometrics::Vector3d    Vector3d;
                    typedef ::picogen::misc::geometrics::Ray         Ray;
                    typedef ::picogen::graphics::image::color::Color Color;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;

                    typedef ::std::list<IIntersectable*>  t_objectList;
                    t_objectList m_list;
                public:
                    LinearList();
                    virtual ~LinearList();
                    virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const;
                    virtual void insert (const IIntersectable*i);
                    virtual void flush();
                    virtual void invalidate();
            };

        }
    }
}//namespace picogen {namespace graphics {namespace objects {


#endif /* _LINEARLIST_H */
