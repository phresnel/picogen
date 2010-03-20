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
        shared_ptr<camera::Camera> cam,
        shared_ptr<primitive::Primitive> prim_,
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
{
}



Scene::~Scene () {
}



shared_ptr<Background> Scene::getBackground () const {
        return background;
}



shared_ptr<camera::Camera> Scene::getCamera () const {
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



tuple<real_t,Color> Scene::Li_VolumeOnly(Sample const& sample, Random& rand) const {

        const Interval i (0, 100000); // TODO: quirk

        const tuple<real_t,Color>
                T  = volumeIntegrator
                   ? volumeIntegrator->Transmittance (*this, sample.primaryRay, sample, i, rand)
                   : make_tuple(real_t(1), Color(real_t(1))),

                Lv = volumeIntegrator
                   ? volumeIntegrator->Li (*this, sample.primaryRay, sample, i, rand)
                   : make_tuple(real_t(1), Color(real_t(0)))
        ;

        const Color atmosphere = background
                        ? background->query(sample.primaryRay)
                        : Color(0.5);

        const Color ret_ = get<1>(T) * atmosphere + get<1>(Lv);
        const Color ret =
                background && background->hasAtmosphereShade()
                ? background->atmosphereShade (ret_, sample.primaryRay, constants::infinity)
                : ret_
        ;


        return make_tuple (1.f, ret);
}



tuple<real_t,Color> Scene::Li (Sample const & sample, Random& rand) const {
        // Intersect geometry.
        const tuple<real_t,Color,real_t>
                Lo_ = aggregate
                    ? surfaceIntegrator->Li(*this, sample.primaryRay, sample, rand)
                    : make_tuple(1., Color(0), constants::infinity)
        ;

        const Color Lo = get<1>(Lo_);
        const real_t distance = get<2>(Lo_);
        const bool didIntersect = distance != constants::infinity;

        const Interval i (0, distance>10000?10000:distance); // TODO: quirk

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

        // Background.
        const Color atmosphere = background
                        ? background->query(sample.primaryRay)
                        : Color(0.5);

        // Now either put atmo- or geom-color into eq.
        const Color
                ret_ = T * (didIntersect ? Lo : atmosphere)  +  Lv,
                ret = background && background->hasAtmosphereShade()
                    ? background->atmosphereShade (ret_, sample.primaryRay, distance)
                    : ret_
        ;

        return make_tuple (1.f, ret);
}



void Scene::render (
        interaction::ProgressReporter::Ptr reporter,
        interaction::UserCommandProcessor::Ptr ucp,
        unsigned int numAASamples_
) const {

        const uint32_t numAASamples = numAASamples_?numAASamples_:1;
        const real_t totalNumberOfSamples = static_cast<real_t>
                (renderTarget->getWidth() * renderTarget->getHeight() * numAASamples);
        real_t sampleNumber = 0;

        if (aggregate)
                aggregate->prepare(*this);

        // non-portable thread id visualisation
        const Color threadCol[] = {
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f),
                Color::FromRGB ((rand()%255)/255.f,(rand()%255)/255.f,(rand()%255)/255.f)
        };

        const uint32_t userSalt = 0; // TODO: this should be exposed to job-level

        shared_ptr<RenderTargetLock> lock (renderTarget->lock());

        const int width = renderTarget->getWidth();
        const int height = renderTarget->getHeight();

        for (int y_=0; y_<height; ++y_) {
                const int y = y_;
                //#warning no multicore!
                #pragma omp parallel for \
                        schedule(dynamic) \
                        reduction(+:sampleNumber)
                for (int x_=0; x_<width; ++x_) {
                        const int x = x_;
                        Color accu = Color::FromRGB(0,1,0);

                        for (int i_=0; i_<(int)numAASamples; ++i_) {
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
                        if (0) {
                                accu = accu * threadCol[omp_get_thread_num()];
                        }
                        if (isnan (accu)) {
                                std::cout << "NaN pixel at " << x << ":" << y << std::endl;
                        } else if (isinf (accu)) {
                                std::cout << "inf pixel at " << x << ":" << y << std::endl;
                        } else {
                                Color c = accu*(Color::real_t(1)/numAASamples);
                                lock->setPixel (x,y,c);
                                ++sampleNumber;
                        }


                        //-------------------------------------------------------------
                        // 5) Report Progress.
                        //-------------------------------------------------------------
                }
                bool userWantsToQuit = false;
                reporter->report (lock, sampleNumber, totalNumberOfSamples);
                ucp->tick();
                userWantsToQuit = ucp->userWantsToQuit();

                if (userWantsToQuit)
                        break;
        }
        reporter->reportDone ();
}



} // namespace redshift
