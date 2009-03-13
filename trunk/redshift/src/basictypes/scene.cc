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
#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"
#include "../../include/basictypes/sample.hh"

#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"

#include "../../include/cameras/camera.hh"

#include "../../include/basictypes/scene.hh"

#include "../../include/basictypes/scene.hh"


namespace redshift {



Scene::Scene (
        shared_ptr<RenderTarget> rt,
        shared_ptr<camera::Camera> cam,
        shared_ptr<Primitive> prim_
)
: renderTarget(rt)
, camera(cam)
, aggregate (prim_)
{
}



Scene::~Scene () {                
}       



inline bool Scene::doesIntersect (RayDifferential const &ray) {
        return aggregate->doesIntersect (ray);
}



inline tuple<bool,Intersection> Scene::intersect (RayDifferential const &ray) {
        return aggregate->intersect (ray);
}
                                                
                                                

inline tuple<real_t,Color> Scene::Li (
        RayDifferential const & ray,
        Sample const & sample
) const {
        /* PBRT:
           Spectrum Lo = surfaceIntegrator->Li (this, ray, sample, alpha)
           Spectrum T = volumeIntegrator->Transmittance (this,ray,sample,alpha)
           Spectrum Lv = volumeIntegrator->Li (this,ray,sample,alpha)
           return T * Lo + Lv
        */
        Color col (0.5+ray.direction.x,0.5+ray.direction.y,
                ray.direction.z);
        return tuple<real_t,Color> (1.0, col); 
}



void Scene::render() const {
        shared_ptr<RenderTargetLock> lock (renderTarget->lock());
        for (int y=0; y<renderTarget->getHeight(); ++y)
         for (int x=0; x<renderTarget->getWidth(); ++x) {
                Sample sample (
                        ImageCoordinates(static_cast<real_t>(x),
                                            static_cast<real_t>(y)),
                        LensCoordinates()
                );

                const tuple<real_t,RayDifferential>
                                          primo = camera->generateRay (sample);
                const real_t rayWeight (get<0>(primo));
                RayDifferential ray (get<1>(primo));

                sample.imageCoordinates.u++;
                ray.rx = get<1>(camera->generateRay (sample));
                sample.imageCoordinates.u--;
                
                ++sample.imageCoordinates.v;
                ray.ry = get<1>(camera->generateRay (sample));
                --sample.imageCoordinates.v;
                ray.hasDifferentials= true;
                
                const tuple<real_t,Color> Ls_ (Li(ray,sample));
                const real_t Ls_alpha (get<0>(Ls_));
                const Color Ls_color  (get<1>(Ls_));
                const Color finalColor = rayWeight * Ls_color;
                
                //<issue warning if unexpected radiance value returned>

                //<add sample contribution to image> 28                
                lock->setPixel (x,y,finalColor);
                        /*Rgb (
                                (float)x/(float)renderTarget->getWidth(),
                                (float)y/(float)renderTarget->getHeight(), 
                                0.5));*/
        }
}



} // namespace redshift