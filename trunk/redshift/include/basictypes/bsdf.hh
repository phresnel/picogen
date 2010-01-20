//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009, 2010  Sebastian Mach (*1983)
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

#ifndef BSDF_HH_INCLUDED_20100118
#define BSDF_HH_INCLUDED_20100118

#include "../setup.hh"
#include "differentialgeometry.hh"

namespace redshift {
class Bsdf {
public:
        Bsdf (
                DifferentialGeometry const &shadingDG_, 
                Normal const &trueNormal_
                // PBRT adds a refraction index parameter, see 10.1, p. 462
        )        
        : shadingDG(shadingDG_)
        , trueNormal(trueNormal_)
        {}

        virtual ~Bsdf () {}

        enum Reflection {
                reflection
        };
        enum Specular {
                specular,
                diffuse
        };
        virtual bool hasAny (Reflection, Specular) const = 0;

        virtual optional<tuple<Color,Vector> > sample_f (
                const Vector &in, Reflection, Specular) const = 0;
        virtual Color f (const Vector &out, const Vector &in) const = 0;
        // HasShadingGeometry(), pbrt 10.1, p. 464

        DifferentialGeometry getShadingDifferentialGeometry () const {
                return shadingDG;
        }
        Normal getTrueNormal () const {
                return trueNormal;
        }


protected:
        DifferentialGeometry const shadingDG;
        Normal const trueNormal;
};
}

#endif // BSDF_HH_INCLUDED_20100118
