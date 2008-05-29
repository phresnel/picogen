/***************************************************************************
 *            cameras.h
 *
 *  Thu Oct 11 20:00:25 2007
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


#ifndef _CAMERAS_H
#define _CAMERAS_H


namespace picogen {
    namespace graphics {
        namespace cameras {
            class FromPointToRect {
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;

                    real m_width, m_height, m_Z;
                    Vector3d m_position;
                public:
                    FromPointToRect (real width, real height, real Z)
                            : m_width (width), m_height (height), m_Z (Z), m_position (0.0,0.0,0.0) {
                    }
                    FromPointToRect()
                            : m_width (1), m_height (1), m_Z (1), m_position (0.0,0.0,0.0) {
                    }
                    void defineCamera (real width, real height, real Z) {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::DefineCamera( base_types::real width, base_types::real height, base_types::real Z )");
                        m_width  = width;
                        m_height = height;
                        m_Z = Z;
                        XRT_CALL_STACK_POP();
                    }
                    /*void SetPosition( Vector3d pos )
                    {
                     m_position = pos;
                    }*/
                    void rayFromUV (param_out (Ray,ray), const real u, const real v) const {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::RayFromUV( param_out(base_types::ray,ray), const base_types::real u, const base_types::real v )");
                        ray.w() [0] = u*m_width-0.5*m_width;
                        ray.w() [1] = - (v*m_height-0.5*m_height);
                        ray.w() [2] = m_Z;
                        ray.w() = ray.w().normal();
                        ray.x() = m_position;
                        XRT_CALL_STACK_POP();
                    }
            };
            class FromPointToRect_Cube {
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;

                    real m_width, m_height, m_Z;
                    Vector3d m_position;
                public:
                    FromPointToRect_Cube (real width, real height, real Z/*, Vector3d pos*/)
                            : m_width (width), m_height (height), m_Z (Z), m_position (0.0,0.0,0.0) {
                    }
                    FromPointToRect_Cube()
                            : m_width (1), m_height (1), m_Z (1), m_position (0.0,0.0,0.0) {
                    }
                    void defineCamera (real width, real height, real Z) {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::DefineCamera( base_types::real width, base_types::real height, base_types::real Z )");
                        m_width  = 1;
                        m_height = 1;
                        m_Z = 0.5;
                        XRT_CALL_STACK_POP();
                    }
                    /*void SetPosition( Vector3d pos )
                    {
                     m_position = pos;
                    }*/
                    void rayFromUV (param_out (Ray,ray), const real _u, const real v) const {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::RayFromUV( param_out(base_types::ray,ray), const base_types::real u, const base_types::real v )");
                        real u = _u * 4;
                        if (u<1) {
                            ray.w() [2] = ( (u) *m_width-0.5*m_width);
                            ray.w() [1] = - (v*m_height-0.5*m_height);
                            ray.w() [0] = -m_Z;
                        } else if (u<2) {
                            ray.w() [0] = (u-1) *m_width-0.5*m_width;
                            ray.w() [1] = - (v*m_height-0.5*m_height);
                            ray.w() [2] = m_Z;
                        } else if (u<3) {
                            ray.w() [2] = - ( (u-2) *m_width-0.5*m_width);
                            ray.w() [1] = - (v*m_height-0.5*m_height);
                            ray.w() [0] = m_Z;
                        } else {
                            ray.w() [0] = - ( (u-3) *m_width-0.5*m_width);
                            ray.w() [1] = - (v*m_height-0.5*m_height);
                            ray.w() [2] = -m_Z;
                        }

                        ray.w() = ray.w().normal();
                        ray.x() = m_position;
                        XRT_CALL_STACK_POP();
                    }
            };
            class FromPointToRect_Cylinder {
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;

                    real m_width, m_height, m_Z;
                    Vector3d m_position;
                public:
                    FromPointToRect_Cylinder (real width, real height, real Z/*, Vector3d pos*/)
                            : m_width (width), m_height (height), m_Z (Z), m_position (0.0,0.0,0.0) {
                    }
                    FromPointToRect_Cylinder()
                            : m_width (1), m_height (1), m_Z (1), m_position (0.0,0.0,0.0) {
                    }
                    void defineCamera (real width, real height, real Z) {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::DefineCamera( base_types::real width, base_types::real height, base_types::real Z )");
                        m_width  = width;
                        m_height = height;
                        m_Z = Z;
                        XRT_CALL_STACK_POP();
                    }
                    /*void SetPosition( Vector3d pos )
                    {
                     m_position = pos;
                    }*/
                    void rayFromUV (param_out (Ray,ray), const real u, const real v) const {
                        XRT_CALL_STACK_PUSH ("void FromPointToRect::RayFromUV( param_out(base_types::ray,ray), const base_types::real u, const base_types::real v )");
                        real a = -u*2*::picogen::misc::constants::pi;
                        ray.w() [0] = m_Z*cos (a);
                        ray.w() [1] = - (v*m_height-0.5*m_height);
                        ray.w() [2] = m_Z*sin (a);
                        ray.w() = ray.w().normal();
                        ray.x() = m_position;
                        XRT_CALL_STACK_POP();
                    }
            };
        } // namespace cameras {
    }//namespace picogen {
}//namespace graphics {


#endif /* _CAMERAS_H */
