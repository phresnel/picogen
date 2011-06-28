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

#ifndef VOLUME_HH_INCLUDED_20100210
#define VOLUME_HH_INCLUDED_20100210

#include "transport.hh"
#include "../tuple.hh"

namespace picogen { namespace redshift {

        class Interval;

        // Phase functions. (Direct transcription from PBRT.)
        inline real_t phaseIsotropic(const Vector &, const Vector &) {
                return 1.f / (4.f * constants::pi);
        }
        inline real_t phaseRayleigh(const Vector &w, const Vector &wp) {
                real_t costheta = dot(w, wp);
                return  3.f/(16.f*constants::pi) * (1 + costheta * costheta);
        }
        inline real_t phaseMieHazy(const Vector &w, const Vector &wp) {
                real_t costheta = dot(w, wp);
                return 9.f/(4.f*constants::pi) * powf(1.f + costheta*costheta, 8.f);
        }
        inline real_t phaseMieMurky(const Vector &w, const Vector &wp) {
                real_t costheta = dot(w, wp);
                return 50.f/(4.f*constants::pi) * powf(1.f + costheta*costheta, 32.f);
        }

        inline real_t phaseHG(const Vector &w, const Vector &wp, real_t g) {
                real_t costheta = dot(w, wp);
                return 1.f / (4.f * constants::pi) * (1.f - g*g) /
                        powf(1.f + g*g - 2.f * g * costheta, 1.5f);
        }

        inline real_t phaseSchlick(const Vector &w,
                           const Vector &wp, real_t g) {
                real_t k = 1.55f * g - .55f * g * g * g;
                real_t kcostheta = k * dot(w, wp);
                return 1.f / (4.f * constants::pi) * (1.f - k*k) /
                        ((1.f - kcostheta) * (1.f - kcostheta));
        }


        // VolumeRegion.
        class VolumeRegion {
        public:
                virtual ~VolumeRegion () {}

                virtual Interval cull (const Ray &ray) const = 0;

                // Absorption:
                //   Absorption cross section.
                virtual Color sigma_a (const Point &p, const Vector &w, Random& rand)const=0;



                // Emission:
                //   Change in radiance due to emmission.
                virtual Color Lve (const Point &p,const Vector &w, Random& rand) const = 0;



                // Scattering:
                //   Out-Scattering Probability per Unit Distance.
                virtual Color sigma_s (const Point &p, const Vector &w, Random& rand)const=0;

                //   "Extinction" or "Attenuation": sigma_a+sigma_s
                virtual Color sigma_t (const Point &p, const Vector &w, Random& rand)const;


                // Phase function.
                virtual real_t p (const Point &p,
                                  const Vector &w_in,
                                  const Vector &w_out,
                                  Random& rand
                ) const = 0;

                virtual Color tau (const Ray &r, const Interval &i,
                                   real_t step, real_t offset,
                                   Random& rand) const=0;
        };



        // DensityRegion.
        class DensityRegion : public VolumeRegion {
        public:
                DensityRegion (
                        Color const & sigma_a,
                        Color const & sigma_s,
                        Color const & Lve,
                        real_t henyeyGreensteinParameter
                )
                : sigma_a_(sigma_a)
                , sigma_s_(sigma_s)
                , Lve_(Lve)
                , henyeyGreensteinParameter(henyeyGreensteinParameter)
                {
                }

                virtual ~DensityRegion () {}

                virtual Interval cull (const Ray &ray) const = 0;

                virtual real_t density(const Point &p, Random& rand) const = 0;

                Color sigma_a(const Point &p, const Vector &, Random& rand) const {
                        return density(p,rand) * sigma_a_;
                }
                Color sigma_s(const Point &p, const Vector &, Random& rand) const {
                        return density(p,rand) * sigma_s_;
                }
                Color sigma_t(const Point &p, const Vector &w, Random& rand) const {
                        const real_t a = density(p,rand);
                        const Color b = VolumeRegion::sigma_t(p,w,rand);
                        return a * b;
                }
                Color Lve(const Point &p, const Vector &/*w*/, Random& rand) const {
                        return density(p,rand) * Lve_;
                }
                real_t p(
                        const Point &/*p*/,
                        const Vector &w_in,
                        const Vector &w_out, Random& /*rand*/
                ) const {
                        return phaseHG(w_in, w_out, henyeyGreensteinParameter);
                }
                Color tau (const Ray &r, const Interval &i,
                                real_t step, real_t offset, Random& rand) const;
        private:
                const Color sigma_a_;
                const Color sigma_s_;
                const Color Lve_;
                const real_t henyeyGreensteinParameter;
        };

        // VolumeIntegrator.
        class VolumeIntegrator {
        public:
                virtual ~VolumeIntegrator () {}
                virtual tuple<real_t,Color> Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const Interval &interval,
                        Random& rand
                ) const = 0;
                virtual tuple<real_t,Color> Transmittance(const Scene &scene,
                        const Ray &ray, const Sample &sample,
                        const Interval &interval,
                        Random& rand
                ) const = 0;
        };
} }

#endif // VOLUME_HH_INCLUDED_20100210
