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

#include "../../include/basictypes/scene.hh"
#include "../../include/integrators/direct-lighting.hh"

#include "../../include/integrators/emission.hh"
#include "../../include/integrators/single-scattering.hh"

#include <omp.h>

namespace {
        redshift::Random createRandom(
                uint32_t a, uint32_t b, uint32_t c, uint32_t d
        ) {
                using namespace kallisto::random::marsaglia;
                MWC A(a,b);          // a, b
                MWC B(c,d);          // c, d
                for (uint32_t warm=0; warm<4; ++warm) {  // warm up
                        A(); B();
                }
                MWC mwc = MWC(A(), B()); // A+B
                mwc.skip(16);      // warm up

                // IMPORTANT: using variables instead of passing
                // as between the ( and the ) of a call, there is no sequence point
                // (and thus it'd be not portable)
                const uint32_t e=mwc(), f=mwc(), g=mwc(), h=mwc();
                redshift::Random ret = redshift::Random (e, f, g, h);
                ret.skip(16);
                return ret;
        }
}



namespace redshift {

Scene::Scene (
        shared_ptr<RenderTarget> rt,
        shared_ptr<Camera> cam,
        shared_ptr<Primitive> prim_,
        shared_ptr<Background> bg,
        shared_ptr<Integrator> surfaceIntegrator,
        shared_ptr<VolumeRegion> volumeRegion_,
        shared_ptr<VolumeIntegrator> volumeIntegrator_
)
: renderTarget(rt)
, camera(cam)
, aggregate (prim_)
, background (bg)
, surfaceIntegrator(surfaceIntegrator)
, volumeRegion (volumeRegion_)
, volumeIntegrator(volumeIntegrator_)
, running_(false)
{
}



Scene::~Scene () {
}



shared_ptr<Background> Scene::getBackground () const {
        return background;
}



shared_ptr<Camera> Scene::getCamera () const {
        return camera;
}



shared_ptr<VolumeRegion> Scene::getVolumeRegion () const {
        return volumeRegion;
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



tuple<real_t,Color> Scene::Li (Sample const & sample, Random& rand, LiMode mode) const {
        // Intersect geometry.
        const tuple<real_t,Color,real_t>
                Lo_ = aggregate && mode==full
                    ? surfaceIntegrator->Li(*this, sample.primaryRay, sample, rand)
                    : make_tuple(1., Color(0), constants::infinity)
        ;

        const Color Lo = get<1>(Lo_);
        const real_t distance = get<2>(Lo_);
        const bool didIntersect = distance != constants::infinity;

        const Interval i (0, distance); // TODO: quirk

        // Intersect volumes.
        const tuple<real_t,Color>
                T_  = volumeIntegrator
                   ? volumeIntegrator->Transmittance (*this, sample.primaryRay, sample, i, rand)
                   : make_tuple(real_t(1), Color(real_t(1))),

                Lv_ = volumeIntegrator
                   ? volumeIntegrator->Li (*this, sample.primaryRay, sample, i, rand)
                   : make_tuple(real_t(1), Color(real_t(0)))
        ;
        const Color T = get<1>(T_), Lv = get<1>(Lv_);


        const Color
                atmosphere_plus_sun     = background->query(sample.primaryRay)
                                        + background->querySun(sample.primaryRay),
                atmosphere_or_geom      = didIntersect ? Lo : (atmosphere_plus_sun),

                // to volume == apply volume scattering onto sth.
                atmosphere_or_geom_volumed = T * atmosphere_or_geom + Lv,

                // to atmosphere == apply aerial perspective onto sth.
                // but not onto sun, which is already attenuated inside PssSunSky
                atmosphere_or_geom_atmosphered
                    = (background && background->hasAtmosphereShade())
                    ? background->atmosphereShade (atmosphere_or_geom_volumed, sample.primaryRay, distance)
                    : atmosphere_or_geom_volumed,

                //
                ret = atmosphere_or_geom_atmosphered
        ;

        return make_tuple (1.f, ret);
}



void Scene::render (
        interaction::ProgressReporter::Ptr reporter,
        interaction::UserCommandProcessor::Ptr ucp,
        unsigned int numAASamples_,
        unsigned int minY, unsigned int maxY,
        unsigned int userSalt
) const {
        running_ = true;

        const uint32_t numAASamples = numAASamples_?numAASamples_:1;
        const uint64_t totalNumberOfSamples =
                (renderTarget->getWidth() * renderTarget->getHeight() * numAASamples);
        uint64_t sampleNumber = 0;

        if (aggregate)
                aggregate->prepare(*this);

        // non-portable thread id visualisation
        /*const Color threadCol[] = {
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f)
        };*/

        shared_ptr<RenderTargetLock> lock (renderTarget->lock());

        const unsigned int width = renderTarget->getWidth();
        const unsigned int height =
                (minY<maxY && maxY<(unsigned int)renderTarget->getHeight())
                ? maxY
                : renderTarget->getHeight();
//#define NO_OMP_THREADING
        for (unsigned int y_=minY; y_<height; ++y_) {
                const unsigned int y = y_;
                currentScanline_ = (unsigned int)y;
                aggregate->setCurrentScanline ((unsigned int)y);
                //#warning no multicore!
                #ifndef NO_OMP_THREADING
                #pragma omp parallel for \
                        schedule(dynamic) \
                        reduction(+:sampleNumber)
                #endif
                for (int x_=0; x_<(int)width; ++x_) { // OMP wants them signed
                        const unsigned int x = (unsigned int)x_;
                        Color accu(0);

                        for (unsigned int i_=0; i_<numAASamples; ++i_) {
                                const int i = i_;
                                redshift::Random rand;

                                // That's totally strange. g++ version 4.4 chokes
                                // (the whole system if you don't kill -9 it)
                                // if I directly construct rand, instead of using
                                // the following assignment. But I failed to minimize
                                // a testcase and haven't filed a report yet.
                                rand = createRandom(x, y, userSalt, i);

                                const real_t u = rand(), v = rand();
                                const real_t imageX = x+u;
                                const real_t imageY = y+v;

                                const ImageCoordinates IC = ImageCoordinates(imageX, imageY);
                                const LensCoordinates LC = LensCoordinates ();

                                Sample sample = Sample (IC, LC, renderTarget);

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
                                const tuple<real_t,Color> Ls_ = Li(sample, rand);
                                const real_t Ls_alpha (get<0>(Ls_));
                                const Color Ls_color  (get<1>(Ls_));
                                const Color finalColor = rayWeight * Ls_color;

                                if (isnan (finalColor)) {
                                        std::cout << "NaN pixel at " << x << ":" << y << ":" << i << std::endl;
                                } else if (isinf (finalColor)) {
                                        std::cout << "inf pixel at " << x << ":" << y << ":" << i << std::endl;
                                } else {
                                        accu = accu + finalColor;
                                }
                                //PBRT:<issue warning if unexpected radiance value returned>
                        }

                        //-------------------------------------------------------------
                        // 4) PBRT:<add sample contribution to image> 28
                        //-------------------------------------------------------------
                        /*if (0) {
                                accu = accu * threadCol[omp_get_thread_num()];
                        }*/
                        if (isnan (accu)) {
                                std::cout << "NaN pixel at " << x << ":" << y << std::endl;
                        } else if (isinf (accu)) {
                                std::cout << "inf pixel at " << x << ":" << y << std::endl;
                        } else {
                                Color c = accu*(Color::real_t(1)/numAASamples);
                                lock->setPixel (x,y,c);
                                sampleNumber += numAASamples;
                        }


                        //-------------------------------------------------------------
                        // 5) Report Progress.
                        //-------------------------------------------------------------
                }
                reporter->report (lock, sampleNumber, totalNumberOfSamples);
                ucp->tick();
                if (ucp->userWantsToQuit())
                        break;

                while (ucp->userWantsToPause() && !ucp->userWantsToQuit()) {
                        ucp->tick();
                }

                if (ucp->userWantsToQuit())
                        break;

                aggregate->prune ();
        }
        reporter->reportDone ();
        running_ = false;
}



bool Scene::running() const {
        return running_;
}

} // namespace redshift
