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

#ifndef MIRROR_HH_INCLUDED_20100119
#define MIRROR_HH_INCLUDED_20100119

#include "../setup.hh"
#include "../basictypes/bsdf.hh"

namespace redshift { namespace bsdf {
        class Mirror : public Bxdf {
        public:
                Mirror (
                        Color const &color_
                );

                virtual optional<tuple<Color,Vector> > sample_f (
                        const Vector &in
                ) const;

                virtual Color f (const Vector &out, const Vector &in) const;
        private:
                Color const color;
        };
} }

#endif // LAMBERTIAN_HH_INCLUDED_20100119