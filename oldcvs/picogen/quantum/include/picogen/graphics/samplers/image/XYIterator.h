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

#define XYIterator_Template template <class t_surface, class t_camera, class t_pathIntegrator>
#define XYIterator_Member( type ) \
	XYIterator_Template	type XYIterator<t_surface, t_camera, t_pathIntegrator>


XYIterator_Template class XYIterator {
private:
    unsigned int m_currX, m_currY;
    unsigned int m_width, m_height;
    unsigned int m_numPixelsPerRun;
    t_camera  m_camera;
    t_surface m_surface;
    t_pathIntegrator m_pathIntegrator;
    bool m_done;
    bool m_BeginRender_called;
    //WhittedStyle() {};
    misc::geometrics::Transformation m_camTransform;
public:
    XYIterator();
    virtual ~XYIterator();
    //void SetCamera( t_camera *pCam );
    //void SetSurface( t_surface *pSurf );
    misc::geometrics::Transformation &transformation() const {
        return m_camTransform;
    }
    misc::geometrics::Transformation &transformation()       {
        return m_camTransform;
    }
    t_surface &surface() const {
        return m_surface;
    }
    t_surface &surface()       {
        return m_surface;
    }
    t_camera &camera() const {
        return m_camera;
    }
    t_camera &camera()       {
        return m_camera;
    }
    t_pathIntegrator &path_integrator() const {
        return m_pathIntegrator;
    }
    t_pathIntegrator &path_integrator()       {
        return m_pathIntegrator;
    }
    void SetNumPixelsPerContinue( unsigned int num );
    bool Continue();
    void BeginRender();
    void OneMoreRun();
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

#include "XYIterator.cc.h"
#undef XYIterator_Template
#undef XYIterator_Member

#endif /* _XYITERATOR_H */
