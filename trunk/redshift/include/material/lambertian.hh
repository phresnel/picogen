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

#ifndef LAMBERTIAN_HH_INCLUDED_20100119
#define LAMBERTIAN_HH_INCLUDED_20100119

#include "../setup.hh"
#include "../basictypes/bsdf.hh"

namespace redshift { namespace bsdf {
        class Lambertian : public Bsdf {
        public:
                Lambertian (
                        DifferentialGeometry const &shadingDG_, 
                        Normal const &trueNormal_,
                        Color const &color_,
                        shared_ptr<MersenneTwister<real_t,0,1> > mt
                );

                virtual optional<tuple<Color,Vector> > sample_f (
                        const Vector &in, 
                        Reflection refl, Specular spec
                ) const;

                virtual Color f (const Vector &out, const Vector &in) const;
        private:
                Color const color;
                shared_ptr<MersenneTwister<real_t,0,1> > mt;
        };
} }

#endif // LAMBERTIAN_HH_INCLUDED_20100119