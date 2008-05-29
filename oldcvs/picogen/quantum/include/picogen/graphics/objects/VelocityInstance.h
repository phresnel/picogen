/***************************************************************************
 *            VelocityInstance.h
 *
 *  Thu Nov  8 01:10:06 2007
 *  Copyright  2007  Sebastian Mach
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


#ifndef _VELOCITYINSTANCE_H
#define _VELOCITYINSTANCE_H

namespace picogen {
    namespace graphics {
        namespace objects {

            class VelocityInstance : public ::picogen::graphics::objects::abstract::IIntersectable {
                    typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;

                    IIntersectable *intersectable;
                    Vector3d velocity, offset;
                    real timeexp;
                public:
                    VelocityInstance() : intersectable (NULL), timeexp (1.0) {}

                    void setTimeExponent (real t) {
                        timeexp = t;
                    }
                    void setVelocity (param_in (Vector3d, V)) {
                        velocity = V;
                    }
                    void setOffset (param_in (Vector3d, o)) {
                        offset = o;
                    }
                    void setIntersectable (const IIntersectable* I) {
                        /// \todo  const_cast?
                        intersectable = const_cast<IIntersectable*> (I);
                    }

                    virtual bool intersect (
                        param_out (intersection_t,intersection), param_in (Ray,ray)
                    ) const {
                        if (NULL == intersectable)
                            return false;
                        Ray newRay;
                        newRay.x() = ray.x() + offset + velocity * (powf (real (rand()) /real (RAND_MAX), timeexp));
                        newRay.w() = ray.w();
                        return intersectable->intersect (intersection, newRay);
                    }
            };

        }//namespace picogen {
    }//namespace graphics {
}// namespace objects {

#endif /* _VELOCITYINSTANCE_H */
