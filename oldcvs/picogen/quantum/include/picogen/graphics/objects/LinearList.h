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

class LinearList : public abstract::IScene {
private:
    typedef misc::prim::real              real;
    typedef misc::geometrics::Vector3d    Vector3d;
    typedef misc::geometrics::Ray         Ray;
    typedef graphics::image::color::Color Color;
    typedef graphics::structs::intersection_t intersection_t;

    typedef std::list<abstract::IIntersectable*>  t_objectList;
    t_objectList m_list;
public:
    LinearList();
    virtual ~LinearList();
    virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const;
    virtual void Insert( const abstract::IIntersectable*i );
    virtual void Flush();
    virtual void Invalidate();
};



#endif /* _LINEARLIST_H */
