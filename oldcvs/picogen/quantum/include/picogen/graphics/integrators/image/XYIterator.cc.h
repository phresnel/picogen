/***************************************************************************
 *            XYIterator.cc.h
 *
 *  Fri Oct 12 20:16:11 2007
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


namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace screen {


                XYIterator_Member() ::XYIterator() :
                        m_currX (0), m_currY (0),
                        m_numPixelsPerRun (1),
                        m_camera(0),
                        m_film(0),
                        m_done (false),
                        m_BeginRender_called (false) {
                }

                XYIterator_Member() ::~XYIterator() {
                }

                /*
                WhittedStyle_Member( void )::SetCamera ( t_camera *p_cam )
                {
                 XRT_CALL_STACK_PUSH( "void WhittedStyle::SetCamera( t_camera *pCam )" );
                 #ifdef XRT_DEBUG
                  if( m_BeginRender_called )
                   throw exceptions::invalid_call( __FILE__, __LINE__, "WhittedStyle::SetCamera() cannot be called during rendering" );
                  if( 0 == p_cam ){
                   throw exceptions::null_pointer( __FILE__, __LINE__,
                    "WhittedStyle::SetCamera(...) called with 0-pointer as param"
                   );
                  }
                 #endif
                 m_camera = p_cam;
                 XRT_CALL_STACK_POP();
                }
                */
                /*
                WhittedStyle_Member( void )::SetSurface ( t_surface *pSurf )
                {
                 XRT_CALL_STACK_PUSH( "void WhittedStyle::SetSurfacce( t_surface *pSurf )" );
                 #ifdef XRT_DEBUG
                  if( m_BeginRender_called )
                   throw exceptions::invalid_call( __FILE__, __LINE__, "WhittedStyle::SetSurface() cannot be called during rendering" );
                  if( 0 == pSurf ){
                   throw exceptions::null_pointer( __FILE__, __LINE__,
                    "WhittedStyle::SetSurface(...) called with 0-pointer as param"
                   );
                  }
                 #endif
                 m_surface = pSurf;
                 XRT_CALL_STACK_POP();
                }
                */
                XYIterator_Member (void) ::setNumPixelsPerContinue (unsigned int num) {
                    XRT_CALL_STACK_PUSH ("void XYIterator::SetNumPixelsPerContinue( unsigned int num )");
#ifdef XRT_DEBUG
                    if (m_BeginRender_called)
                        throw exceptions::invalid_call (__FILE__, __LINE__, "WhittedStyle::SetNumPixelsPerContinue() cannot be called during rendering");
                    if (num < 1)
                        throw exceptions::value_constraint_2 (__FILE__, __LINE__, "parameter ´num´ must not have a value < 1");
#endif
                    m_numPixelsPerRun = num;
                    XRT_CALL_STACK_POP();
                }

                XYIterator_Member (void) ::beginRender() {
                    XRT_CALL_STACK_PUSH ("void XYIterator::BeginRender()");
#ifdef XRT_DEBUG
                    if (m_BeginRender_called)
                        throw exceptions::invalid_call (__FILE__, __LINE__, "XYIterator::BeginRender() cannot be called during rendering");
                    /*if( 0 == m_surface ){
                     throw exceptions::null_pointer( __FILE__, __LINE__,
                      "WhittedStyle::SetSurface(...) not called or called with invalid param."
                     );
                    }
                    if( 0 == m_camera ){
                     throw exceptions::null_pointer( __FILE__, __LINE__,
                      "WhittedStyle::SetCamera(...) not called or called with invalid param."
                     );
                    }*/
#endif
                    m_currX = 0;
                    m_currY = 0;
                    m_width  = m_film->getWidth();
                    m_height = m_film->getHeight();
                    m_BeginRender_called = true;
                    m_done = false;
                    XRT_CALL_STACK_POP();
                }

                XYIterator_Member (void) ::oneMoreRun() {
                    XRT_CALL_STACK_PUSH ("void XYIterator::OneMoreRun()");
#ifdef XRT_DEBUG
                    if (!m_BeginRender_called)
                        throw exceptions::invalid_call (__FILE__, __LINE__, "XYIterator::BeginRender() must have been called before OneMoreRun");
#endif
                    m_currX = 0;
                    m_currY = 0;
                    m_done = false;
                    XRT_CALL_STACK_POP();
                }


                XYIterator_Member (bool) ::renderMore() {
                    XRT_CALL_STACK_PUSH ("bool XYIterator::Continue()");

#ifdef XRT_DEBUG
                    if (!m_BeginRender_called)
                        throw exceptions::invalid_call (__FILE__, __LINE__, "XYIterator::BeginRender() has not been called");
                    if (m_width != m_surface.width())
                        throw exceptions::base (__FILE__, __LINE__, "the surface we're rendering to has changed its width during rendering.");
                    if (m_height != m_surface.height())
                        throw exceptions::base (__FILE__, __LINE__, "the surface we're rendering to has changed its height during rendering.");
#endif
                    if (!m_done) {
                        unsigned int num = 0;
                        while (num++ < m_numPixelsPerRun) {
                            if (m_currX >= m_width) {
                                m_currY++;
                                m_currX = 0;
                            }
                            if (m_currY >= m_height) {
                                m_done = true;
                                break;
                            }

                            //> get uv
                            const real aa_rad = 1.0;
                            const real uf = aa_rad * (static_cast<real> (static_cast<double> (rand()) /static_cast<double> (RAND_MAX))  - 0.5); //> anti-
                            const real uv = aa_rad * (static_cast<real> (static_cast<double> (rand()) /static_cast<double> (RAND_MAX))  - 0.5); //> alias
                            const real u = (uf+static_cast<real> (m_currX)) / static_cast<real> (m_width-1);
                            const real v = (uv+static_cast<real> (m_currY)) / static_cast<real> (m_height-1);

                            //> get ray from camera
                            Ray ray = (*m_camera)(u, v);

                            //> transform ray
                            const Vector3d x = ray.x(), w = x + ray.w();
                            ray.x() = m_camTransform * x;
                            ray.w() = ( (m_camTransform * w) - ray.x()).normal();

                            //> ray trace
                            Color color;
                            intersection_t I;
                            structs::sample sample (m_currX, m_currY, u, v, ray);

                            if (m_pathIntegrator.integrate (color, I, ray)) {
                                sample.primaryIntersection = &I;
                                sample.color = color;
                                m_film->addSample (sample);
                                //m_surface (m_currX,m_currY) += color;
                            } else {
                                m_film->addSample (sample);
                                /*m_surface (m_currX,m_currY) += Color (
                                                                   ray.w() [0]+0.5,
                                                                   ray.w() [1]+0.5,
                                                                   ray.w() [2]+0.5
                                                               );*/
                            }
                            m_currX++;
                        }
                    }
                    XRT_CALL_STACK_POP();
                    return m_done;
                }
            } // namespace screen {
        } // namespace integrators {
    } // namespace graphics {
} // namespace picogen {
