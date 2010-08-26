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

#ifndef MATTE_HH_INCLUDED_20100826
#define MATTE_HH_INCLUDED_20100826

#include "../sealed.hh"
#include "../smart_ptr.hh"

namespace redshift {
        class Bsdf;
        class DifferentialGeometry;
        class ColorTexture;
        class ScalarTexture;
}

namespace redshift { namespace material {
        SEALED(Matte);
        class Matte : MAKE_SEALED(Matte) {
        public:
                Matte (shared_ptr<ColorTexture> color,
                       shared_ptr<ScalarTexture> roughness);
                shared_ptr<Bsdf> getBsdf(
                        const DifferentialGeometry &) const;
        private:
                shared_ptr<ColorTexture> color;
                shared_ptr<ScalarTexture> roughness;
        };
} }

#endif // MATTE_HH_INCLUDED_20100826
