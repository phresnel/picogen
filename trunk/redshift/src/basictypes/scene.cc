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

#include "../../include/basictypes/transport.hh"

#include "../../include/basictypes/background.hh"
#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"

#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"

#include "../../include/basictypes/sample.hh"

#include "../../include/cameras/camera.hh"

#include "../../include/primitives/primitive.hh"

#include "../../include/interaction/progressreporter.hh"
#include "../../include/interaction/usercommandprocessor.hh"

#include "../../include/integrators/direct-lighting.hh"
#include "../../include/backgrounds/visualise-direction.hh"

#include "../../include/basictypes/scene.hh"

namespace redshift {



Scene::Scene (
        shared_ptr<RenderTarget> rt,
        shared_ptr<camera::Camera> cam,
        shared_ptr<primitive::Primitive> prim_,
        shared_ptr<Background> bg
)
: renderTarget(rt)
, camera(cam)
, aggregate (prim_)
, background (bg)
{
}



Scene::~Scene () {                
}       



shared_ptr<Background> Scene::getBackground () const {
        return background;
}



bool Scene::doesIntersect (Sample const &sample) const {
        return aggregate->doesIntersect (sample);
}



bool Scene::doesIntersect (Ray const &ray) const {
        return aggregate->doesIntersect (ray);
}



optional<Intersection> Scene::intersect (Sample const &sample) const {
        return aggregate->intersect (sample);
}



optional<Intersection> Scene::intersect(
        RayDifferential const &ray
) const {
        return aggregate->intersect (ray);
}



tuple<real_t,Color> Scene::Li (Sample const & sample) const {
        /* PBRT:
           Spectrum Lo = surfaceIntegrator->Li (this, ray, sample, alpha)
           Spectrum T = volumeIntegrator->Transmittance (this,ray,sample,alpha)
           Spectrum Lv = volumeIntegrator->Li (this,ray,sample,alpha)
           return T * Lo + Lv
        */
        DirectLighting dl;
        return dl.Li (*this, sample.primaryRay, sample);
}



void Scene::render (
        interaction::ProgressReporter::ConstPtr reporter,
        interaction::UserCommandProcessor::Ptr ucp
        
) const {
        const real_t totalNumberOfSamples = static_cast<real_t>
                (renderTarget->getWidth() * renderTarget->getHeight());
        real_t sampleNumber = 0;

        shared_ptr<RenderTargetLock> lock (renderTarget->lock());
        for (int y=renderTarget->getHeight()-1; y>=0; --y)
        for (int x=0; x<renderTarget->getWidth(); ++x) {
                
                Sample sample (
                        ImageCoordinates(static_cast<real_t>(x),
                                         static_cast<real_t>(y)),
                        LensCoordinates(),
                        renderTarget
                );

                //-------------------------------------------------------------
                // 1) Generate Primary Ray.
                //-------------------------------------------------------------
                const tuple<real_t,RayDifferential>
                                          primo = camera->generateRay (sample);
                const real_t & rayWeight (get<0>(primo));
                sample.primaryRay = get<1>(primo); // Will be modified, hence
                                                   // non-const non-ref.


                //-------------------------------------------------------------
                // 2) Generate Ray Differential.
                //-------------------------------------------------------------
                sample.imageCoordinates.u++;
                sample.primaryRay.rx = get<1>(camera->generateRay (sample));
                sample.imageCoordinates.u--;

                ++sample.imageCoordinates.v;
                sample.primaryRay.ry = get<1>(camera->generateRay (sample));
                --sample.imageCoordinates.v;
                
                sample.primaryRay.hasDifferentials= true;


                //-------------------------------------------------------------
                // 3) Evaluate Radiance Along Primary Ray.
                //-------------------------------------------------------------
                const tuple<real_t,Color> Ls_ (Li(sample));
                const real_t Ls_alpha (get<0>(Ls_));
                const Color Ls_color  (get<1>(Ls_));
                const Color finalColor = rayWeight * Ls_color;
                
                //PBRT:<issue warning if unexpected radiance value returned>


                //-------------------------------------------------------------
                // 4) PBRT:<add sample contribution to image> 28                
                //-------------------------------------------------------------
                lock->setPixel (x,y,finalColor);
                        /*Rgb (
                                (float)x/(float)renderTarget->getWidth(),
                                (float)y/(float)renderTarget->getHeight(), 
                                0.5));*/
                ++sampleNumber;


                //-------------------------------------------------------------
                // 5) Report Progress.
                //-------------------------------------------------------------
                reporter->report (lock, sampleNumber, totalNumberOfSamples);

                ucp->tick();
                if (ucp->userWantsToQuit()) {
                        break;
                }
        }
        reporter->reportDone ();
}



} // namespace redshift