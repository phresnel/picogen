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

#ifndef TRANSMISSIVELAMBERTIAN_HH_INCLUDED_20100824
#define TRANSMISSIVELAMBERTIAN_HH_INCLUDED_20100824

#include "../basictypes/bsdf.hh"

namespace redshift { namespace bsdf {
        SEALED(TransmissiveLambertian);
        class TransmissiveLambertian
        : public Bxdf, MAKE_SEALED(TransmissiveLambertian) {
        public:
                TransmissiveLambertian (Color const &color_,
                                        real_t transmissionProb);

                BsdfSample sample_f (
                        const Vector &in, Random &
                ) const;

                Color f (const Vector &out, const Vector &in, Random &) const;

        private:
                Color const color;
                real_t transmissionProb;
        };
} }

#endif // TRANSMISSIVELAMBERTIAN_HH_INCLUDED_20100824
