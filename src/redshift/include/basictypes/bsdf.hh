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

#include "../geometry.hh"
#include "../optional.hh"
#include "../tuple.hh"
#include "../smart_ptr.hh"
#include "spectrum.hh"
#include "differentialgeometry.hh"

namespace redshift {
class Bxdf;
class Random;

class Bsdf {
public:
        Bsdf (
                DifferentialGeometry const &shadingDG_
                // PBRT adds a refraction index parameter, see 10.1, p. 462
        );

        virtual ~Bsdf () {}

        enum Reflection {
                reflection = 1<<0,
                transmission = 1<<1
        };
        enum Specular {
                specular = 1<<0,
                diffuse  = 1<<1
        };
        //virtual bool hasAny (Reflection, Specular) const = 0;

        optional<tuple<Color,Vector,real_t> > sample_f (
                const Vector &in, Reflection, Specular, Random &
        ) const;

        Color f (const Vector &out, const Vector &in,
                Bsdf::Reflection r, Bsdf::Specular s, Random&) const;
        // HasShadingGeometry(), pbrt 10.1, p. 464

        DifferentialGeometry getShadingDifferentialGeometry () const {
                return shadingDG;
        }
        __attribute__((deprecated))
        Normal getTrueNormal () const {
                return getGeometricNormal();
        }
        Normal getGeometricNormal () const {
                return shadingDG.getGeometricNormal();
        }

        Normal getShadingNormal () const {
                return shadingDG.getShadingNormal();
        }

        void add (shared_ptr<Bxdf> bxdf) {
                bxdfs.push_back (bxdf);
        }
        void clear () {
                std::vector<shared_ptr<Bxdf> >().swap (bxdfs);
        }

        bool is (Bsdf::Reflection r, Bsdf::Specular s) const ;

private:
        Vector worldToLocal (Vector const &v) const ;
        Vector localToWorld (Vector const &v) const ;
        DifferentialGeometry const shadingDG;
        std::vector<shared_ptr<Bxdf> > bxdfs;

        Normal geometricNormal, shadingNormal;
        Vector s, t;

        int numComponents (Bsdf::Reflection r, Bsdf::Specular s) const ;
};


class Bxdf {
public:
        Bxdf (Bsdf::Reflection refl, Bsdf::Specular spec)
        : reflection(refl)
        , specular(spec)
        {}

        virtual ~Bxdf () {}

        bool is (Bsdf::Reflection r, Bsdf::Specular s) const {
                return ((reflection&r)==r) && ((specular&s)==s);
        }

        virtual tuple<Color,Vector,real_t> sample_f (const Vector &in, Random &) const = 0;
        virtual Color f (const Vector &out, const Vector &in, Random &) const = 0;

protected:
        const Bsdf::Reflection reflection;
        const Bsdf::Specular   specular;
};
}

#endif // BSDF_HH_INCLUDED_20100118
