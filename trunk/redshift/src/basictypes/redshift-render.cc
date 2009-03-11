//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include "../../include/setup.hh"

#include "../../include/basictypes/background.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/basictypes/primitive.hh"
#include "../../include/basictypes/scene.hh"
#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"
#include "../../include/basictypes/sample.hh"

#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"

#include "../../include/cameras/camera.hh"

#include "../../include/basictypes/redshift-render.hh"

namespace redshift {

        Renderer::Renderer (
                shared_ptr<RenderTarget> rt,
                shared_ptr<Camera> cam)
        : renderTarget(rt)
        , camera(cam)
        {
        }
        
        Renderer::~Renderer () {                
        }       

        void Renderer::render() const {
                shared_ptr<RenderTargetLock> lock (renderTarget->lock());
                for (int y=0; y<renderTarget->getHeight(); ++y) {
                        for (int x=0; x<renderTarget->getWidth(); ++x) {
                                Sample sample (
                                   ImageCoordinates(static_cast<real_t>(x),
                                                    static_cast<real_t>(y)));
                                Ray ray;
                                camera->generateRay (sample, ray);
                                Color tmp;
                                tmp.fromRgb (0.5, 0.5, 0.5);
                                lock->setPixel (x,y,tmp);
                        }
                }
        }        
}