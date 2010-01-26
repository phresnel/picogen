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

#include "../../include/material/mirror.hh"

namespace redshift { namespace bsdf {



Mirror::Mirror (
        DifferentialGeometry const &shadingDG_,
        Color const &color_
)
: Bsdf (shadingDG_)
, color (color_)
{}



optional<tuple<Color,Vector> > Mirror::sample_f (
        const Vector &in,
        Reflection refl, Specular spec
) const {
        switch (refl) {
        case reflection:
                switch (spec) {
                case specular:
                        return make_tuple(
                                color,
                                reflect (in, shadingDG.getNormal())
                        );;
                case diffuse: return false;
                };
                break;
        };
        return false;
}



Color Mirror::f (const Vector &out, const Vector &in) const {
        return Color::fromRgb(0,0,0);
}



} }
