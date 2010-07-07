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

#ifndef SCENE_HH_INCLUDED_20090303
#define SCENE_HH_INCLUDED_20090303

#include "../setup.hh"
#include "../rendertargets/rendertarget.hh"
#include "../cameras/camera.hh"
#include "sample.hh"
#include "../primitives/primitive.hh"
#include "background.hh"
#include "intersection.hh"
#include "transport.hh"
#include "volume.hh"

#include "../interaction/progressreporter.hh"
#include "../interaction/usercommandprocessor.hh"

namespace redshift {
        class Random;

        class Scene {
        public:
                /*enum LiMode {
                        volume_only,
                        full
                };*/
                struct LiMode {
                        bool SkipSurface;
                        bool SkipSun;

                        LiMode () : SkipSun(0), SkipSurface(0) {}

                        static LiMode Full () {
                                return LiMode();
                        }
                };

                Scene(
                        shared_ptr<RenderTarget>,
                        shared_ptr<Camera>,
                        shared_ptr<Primitive>,
                        shared_ptr<Sky> bg ,
                        shared_ptr<Integrator> integrator,
                        shared_ptr<VolumeRegion> volumeRegion,
                        shared_ptr<VolumeIntegrator> volumeIntegrator
                );
                virtual ~Scene ();

                void render(
                        interaction::ProgressReporter::Ptr,
                        interaction::UserCommandProcessor::Ptr,
                        unsigned int samplePerPixel,
                        unsigned int minY, unsigned int maxY,
                        unsigned int userSeed
                ) const ;

                optional<Intersection> intersect(Ray const &ray) const;
                bool doesIntersect (Ray const &ray)       const;

                shared_ptr<Sky>            getBackground ()   const;
                shared_ptr<Camera>         getCamera ()       const;
                shared_ptr<VolumeRegion>   getVolumeRegion () const;

                tuple<real_t,Color> Li(RayDifferential const &, Sample const&, Random &, LiMode mode = LiMode::Full()) const;
                Color attenuate (
                        Color const &orig,
                        Ray const &ray,
                        Sample const& sample,
                        real_t distance,
                        Random &rand
                ) const;

                unsigned int currentScanline () const { return currentScanline_; }

                bool running () const ;
        private:
                // non copyable
                // TODO use NonCopyable base class instead
                Scene (Scene const &);
                Scene & operator= (Scene const &);
                Scene();


                inline optional<Intersection> intersect(
                                             Sample const &sample) const;

                //Scene scene;
                shared_ptr<RenderTarget>         renderTarget;
                shared_ptr<Camera>               camera;
                shared_ptr<Primitive>            aggregate;
                shared_ptr<Sky>                  background;
                shared_ptr<Integrator>           surfaceIntegrator;

                shared_ptr<VolumeRegion>         volumeRegion;
                shared_ptr<VolumeIntegrator>     volumeIntegrator;

                mutable bool running_;

                mutable unsigned int currentScanline_;
        };
}

#endif // SCENE_HH_INCLUDED_20090303
