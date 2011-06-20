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

#ifndef LEAF0_HH_INCLUDED_20100829
#define LEAF0_HH_INCLUDED_20100829

#include "../random.hh"
#include "../sealed.hh"
#include "../real.hh"
#include "../smart_ptr.hh"
#include "../basictypes/material.hh"

namespace redshift {
        class Bsdf;
        class DifferentialGeometry;
        class ColorTexture;
        class ScalarTexture;
        class Matte;
}

namespace redshift { namespace material {
        SEALED(Leaf0);
        class Leaf0 : MAKE_SEALED(Leaf0), public Material {
        public:
                Leaf0 (shared_ptr<ColorTexture> color,
                       shared_ptr<ScalarTexture> transmissionProb);
                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry &) const;
                real_t alpha (const DifferentialGeometry &) const;
        private:
                shared_ptr<ColorTexture> color;
                shared_ptr<ScalarTexture> transmissionProb;
                mutable Random random;
        };
} }

#endif // LEAF0_HH_INCLUDED_20100829
