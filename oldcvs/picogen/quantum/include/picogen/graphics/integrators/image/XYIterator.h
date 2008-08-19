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

namespace picogen {
    namespace graphics {
        namespace integrators {
            namespace screen {
                class XYIterator {
                    private:
                        typedef ::picogen::real real;
                        typedef ::picogen::geometrics::Vector3d Vector3d;
                        typedef ::picogen::geometrics::Ray Ray;
                        typedef ::picogen::geometrics::BoundingBox BoundingBox;
                        typedef ::picogen::geometrics::Transformation Transformation;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;
                        typedef ::picogen::graphics::objects::abstract::IIntersectable IIntersectable;
                        typedef ::picogen::graphics::color::Color Color;
                        typedef ::picogen::graphics::cameras::abstract::ICamera ICamera;
                        typedef ::picogen::graphics::film::abstract::IFilm IFilm;
                        typedef ::picogen::graphics::integrators::surface::ISurfaceIntegrator ISurfaceIntegrator;

                        unsigned int m_currX, m_currY;
                        unsigned int m_width, m_height;
                        unsigned int m_numPixelsPerRun;
                        ICamera *m_camera;
                        IFilm *m_film;
                        ISurfaceIntegrator *m_surfaceIntegrator;
                        bool m_done;
                        bool m_BeginRender_called;

                        Transformation m_camTransform;
                    public:
                        XYIterator();
                        virtual ~XYIterator();

                        const Transformation &transformation() const {
                            return m_camTransform;
                        }
                        Transformation &transformation()       {
                            return m_camTransform;
                        }

                        ICamera *getCamera () const {
                            return m_camera;
                        }
                        void setCamera (ICamera *camera) {
                            m_camera = camera;
                        }

                        IFilm *getFilm () const {
                            return m_film;
                        }
                        void setFilm (IFilm *film) {
                            m_film = film;
                        }

                        ISurfaceIntegrator *getSurfaceIntegrator () const {
                            return m_surfaceIntegrator;
                        }
                        void setSurfaceIntegrator (ISurfaceIntegrator *surfaceIntegrator) {
                            m_surfaceIntegrator = surfaceIntegrator;
                        }

                        void setNumPixelsPerContinue (unsigned int num);
                        bool renderMore();
                        void beginRender();
                        void oneMoreRun();
                };

            } // namespace screen {
        } // namespace integrators
    } // namespace graphics {
} // namespace picogen {


#endif /* _XYITERATOR_H */
