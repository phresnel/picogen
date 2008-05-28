/***************************************************************************
 *            Instance.h
 *
 *  2008-05-27 01:00:00
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

#ifndef _INSTANCE_H
#define _INSTANCE_H

#ifndef INSIDE_OBJECTS_H
namespace picogen {
namespace graphics {
namespace objects {
#endif // #ifndef INSIDE_OBJECTS_H

class Instance : public picogen::graphics::objects::abstract::IIntersectable {
    picogen::graphics::objects::abstract::IIntersectable *intersectable;
    picogen::misc::geometrics::Transformation transformation;

public:
    explicit Instance( picogen::graphics::objects::abstract::IIntersectable *intersectable )
            : intersectable(intersectable) {}

    void setTransform( const picogen::misc::geometrics::Transformation &transformation ) {
        this->transformation = transformation;
    }

    virtual bool Intersect(
        param_out(structs::intersection_t,intersection), param_in(misc::geometrics::Ray,ray)
    ) const {
        const Vector3d A = transformation * ray(0.0);
        const Vector3d B = transformation * ray(1.0);
        const Ray   tray = Ray( A, (B-A).normal() );
        if ( intersectable->Intersect( intersection, tray ) ) {
            const Vector3d i  = tray(intersection.t);
            intersection.t = ( i - ray(0.0)).length();
            /*const Vector3d i_ = i + intersection.normal;
            intersection.normal*/
            return true;
        } else {
            return false;
        }
    }
};

#ifndef INSIDE_OBJECTS_H
}
}
} // namespace picogen{ namespace graphics{ namespace objects{
#endif // #ifndef INSIDE_OBJECTS_H

#endif /* _INSTANCE_H */
