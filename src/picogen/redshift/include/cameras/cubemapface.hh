//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef CUBEMAPFACE_HH_INCLUDED_20100411
#define CUBEMAPFACE_HH_INCLUDED_20100411

#include "../cameras/camera.hh"
#include "../smart_ptr.hh"

namespace picogen { namespace redshift { namespace camera {

        SEALED(CubeMapFace);

        class CubeMapFace : public Camera, MAKE_SEALED(CubeMapFace) {
        public:
                enum Face {
                        front, back,
                        left, right,
                        bottom, top
                };

                CubeMapFace (unsigned int width, unsigned int height,
                             Face face,
                             Transform const &xform = Transform::identity());

                virtual ~CubeMapFace () ;

                inline tuple<real_t,RayDifferential>
                           generateRay(Sample const &) const;
                bool hasCommonCenter () const ;
                Point getCommonCenter () const ;

        private:
                real_t invFilmWidth;
                real_t invFilmHeight;
        };
} } }

#endif // CUBEMAPFACE_HH_INCLUDED_20100411
