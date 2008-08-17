/***************************************************************************
 *            XYIterator.h
 *
 *  Fri Oct 12 20:14:05 2007
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


#ifndef _XYITERATOR_H
#define _XYITERATOR_H

#define XYIterator_Template template <class t_surface, class t_pathIntegrator>
#define XYIterator_Member( type ) \
    XYIterator_Template type XYIterator<t_surface, t_pathIntegrator>

namespace picogen {
    namespace graphics {
        namespace samplers {
            namespace screen {
                XYIterator_Template class XYIterator {
                    private:
                        typedef ::picogen::misc::prim::real real;
                        typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                        typedef ::picogen::misc::geometrics::Ray Ray;
                        typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                        typedef ::picogen::misc::geometrics::Transformation Transformation;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;
                        typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                        typedef ::picogen::graphics::image::color::Color Color;
                        typedef ::picogen::graphics::cameras::abstract::ICamera ICamera;

                        unsigned int m_currX, m_currY;
                        unsigned int m_width, m_height;
                        unsigned int m_numPixelsPerRun;
                        ICamera *m_camera;
                        t_surface m_surface;
                        t_pathIntegrator m_pathIntegrator;
                        bool m_done;
                        bool m_BeginRender_called;
                        //WhittedStyle() {};
                        Transformation m_camTransform;
                    public:
                        XYIterator();
                        virtual ~XYIterator();
                        //void SetCamera( t_camera *pCam );
                        //void SetSurface( t_surface *pSurf );
                        Transformation &transformation() const {
                            return m_camTransform;
                        }
                        Transformation &transformation()       {
                            return m_camTransform;
                        }
                        t_surface &surface() const {
                            return m_surface;
                        }
                        t_surface &surface()       {
                            return m_surface;
                        }

                        ICamera *getCamera () const {
                            return m_camera;
                        }
                        void setCamera (ICamera *camera) {
                            m_camera = camera;
                        }

                        t_pathIntegrator &path_integrator() const {
                            return m_pathIntegrator;
                        }
                        t_pathIntegrator &path_integrator()       {
                            return m_pathIntegrator;
                        }
                        void setNumPixelsPerContinue (unsigned int num);
                        bool renderMore();
                        void beginRender();
                        void oneMoreRun();
                };

// meh :|
// looks like as long as gcc does not support  the  'export'  keyword
// and  as  long  as  we want  to  keep  away from  abstract  classes
// we have  to  crunch  all  function-def's  inside  this  header ...
// - -
// - -
// IF YOU CONTRIBUTE TO THIS FILE:  please do not put the definitions
// inside the class-definition, but rather below
// - -

            } // { namespace screen {
        } // namespace samplers
    } // namespace graphics {
} // namespace picogen {
#include "XYIterator.cc.h"
#undef XYIterator_Template
#undef XYIterator_Member


#endif /* _XYITERATOR_H */
