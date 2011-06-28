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

#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/texture/image.hh"

namespace picogen { namespace redshift { namespace texture {



ColorImage::ColorImage (const char *filename)
: image(filename)
{
}


ColorImage::ColorImage (const std::string &filename)
: image(filename.c_str())
{
}



ColorImage::~ColorImage() {
}



Color ColorImage::color(DifferentialGeometry const &dg) const {
        real_t u = std::fmod(dg.u(),1);
        real_t v = std::fmod(dg.v(),1);
        //return Color::FromRGB(1.,0.,0., ReflectanceSpectrum);
        return image.lerp(u,v);
}



real_t ColorImage::alpha(DifferentialGeometry const &dg) const {
        real_t u = std::fmod(dg.u(),1);
        real_t v = std::fmod(dg.v(),1);
        //return Color::FromRGB(1.,0.,0., ReflectanceSpectrum);
        return image.alpha_lerp(u,v);
}



} } }

