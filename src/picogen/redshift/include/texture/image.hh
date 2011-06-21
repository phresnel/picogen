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

#ifndef IMAGE_HH_INCLUDED_20100817
#define IMAGE_HH_INCLUDED_20100817

#include "../basictypes/texture.hh"
#include "../auxiliary/image.hh"
#include <string>

namespace redshift {
        namespace texture {
                class ColorImage : public ColorTexture {
                public:
                        ColorImage (const char *filename);
                        ColorImage (const std::string &filename);
                        virtual ~ColorImage();

                        Color color(DifferentialGeometry const &) const;
                        real_t alpha(DifferentialGeometry const &) const;
                private:
                        aux::ColorImage image;
                };
        }
}

#endif // CONSTANT_HH_INCLUDED_20100817
