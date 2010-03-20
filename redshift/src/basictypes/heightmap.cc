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

#include "../../include/basictypes/heightmap.hh"

namespace redshift {



HeightmapRenderer::HeightmapRenderer (
        shared_ptr<RenderTarget> rt,
        shared_ptr<HeightFunction> fun)
: renderTarget(rt)
, function(fun)
{
}



HeightmapRenderer::~HeightmapRenderer () {
}



void HeightmapRenderer::render (
        interaction::ProgressReporter::Ptr reporter,
        interaction::UserCommandProcessor::Ptr ucp

) const {
        const real_t totalNumberOfSamples = static_cast<real_t>
                (renderTarget->getWidth() * renderTarget->getHeight());
        real_t sampleNumber = 0;

        int const width = renderTarget->getWidth();
        int const height = renderTarget->getHeight();

        shared_ptr<RenderTargetLock> lock (renderTarget->lock());
        for (int y=height-1; y>=0; --y)
         for (int x=0; x<width; ++x) {

                const real_t
                        u = static_cast<real_t>(x) / width,
                        v = static_cast<real_t>(y) / height,
                        h_ = (*function) (u, v)
                        ;

                lock->setPixel (x,y, Color::FromRGB(h_,h_,h_));

                ++sampleNumber;
                reporter->report (lock, sampleNumber, totalNumberOfSamples);


                /*Sample sample (
                        ImageCoordinates(static_cast<real_t>(x),
                                            static_cast<real_t>(y)),
                        LensCoordinates(),
                        renderTarget
                );
                const tuple<real_t,RayDifferential>
                                          primo = camera->generateRay (sample);
                const real_t & rayWeight (get<0>(primo));
                sample.primaryRay = get<1>(primo); // Will be modified, hence
                                                     // non-const non-ref.

                sample.imageCoordinates.u++;
                sample.primaryRay.rx = get<1>(camera->generateRay (sample));
                sample.imageCoordinates.u--;

                ++sample.imageCoordinates.v;
                sample.primaryRay.ry = get<1>(camera->generateRay (sample));
                --sample.imageCoordinates.v;

                sample.primaryRay.hasDifferentials= true;

                const tuple<real_t,Color> Ls_ (Li(sample));
                const real_t Ls_alpha (get<0>(Ls_));
                const Color Ls_color  (get<1>(Ls_));
                const Color finalColor = rayWeight * Ls_color;

                //PBRT:<issue warning if unexpected radiance value returned>

                //PBRT:<add sample contribution to image> 28
                lock->setPixel (x,y,finalColor);
                        //Rgb (
                          //      (float)x/(float)renderTarget->getWidth(),
                          //      (float)y/(float)renderTarget->getHeight(),
                                0.5));

                ++sampleNumber;
                reporter->report (lock, sampleNumber, totalNumberOfSamples);
                */
                ucp->tick();
                if (ucp->userWantsToQuit()) {
                        break;
                }
        }
        reporter->reportDone ();
}



} // namespace redshift
