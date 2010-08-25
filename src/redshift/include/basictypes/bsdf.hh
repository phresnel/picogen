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
#include "../sealed.hh"
#include "spectrum.hh"
#include "differentialgeometry.hh"

namespace redshift {
class Bxdf;
class Random;

SEALED(BsdfSample);
class BsdfSample : MAKE_SEALED(BsdfSample) {
public:
        BsdfSample(Color const &R, Vector const &incident, real_t pdf)
        : reflection_(R), incident_(incident), pdf_(pdf)
        {}

        BsdfSample (BsdfSample const &rhs)
        : SEALED_CONSTRUCT(BsdfSample)
        , reflection_(rhs.reflection_)
        , incident_(rhs.incident_)
        , pdf_(rhs.pdf_)
        {}

        BsdfSample& operator= (BsdfSample const &rhs) {
                reflection_ = rhs.reflection_;
                incident_ = rhs.incident_;
                pdf_ = rhs.pdf_;
                return *this;
        }

        Color  color ()    const { return reflection_; }
        Vector incident () const { return incident_; }
        real_t pdf ()      const { return pdf_; }

        static BsdfSample null() {
                return BsdfSample();
        }

private:
        Color reflection_;
        Vector incident_;
        real_t pdf_;

        BsdfSample() ;
};

class BsdfType {
public:
        enum Reflection {
                reflection = 1<<0,
                transmission = 1<<1
        };
        enum Specular {
                specular = 1<<0,
                diffuse  = 1<<1
        };

        BsdfType (Reflection refl, Specular spec)
        : specular_(spec)
        , reflection_(refl)
        {
        }

        bool isDiffuse () const { return (specular_ & diffuse) == diffuse; }
        bool isSpecular () const { return (specular_ & specular) == specular; }
        bool is(Specular s) const {
                if (s == 0) return false;
                return (specular_ & s) == s;
        }

        bool isReflective () const { return (reflection_ & reflection) == reflection; }
        bool isTransmissive () const { return (reflection_ & transmission) == transmission; }
        bool is(Reflection s) const {
                if (s == 0) return false;
                return (reflection_ & s) == s;
        }

        void cancel (Reflection r) {
                reflection_ = Reflection(reflection_ & ~r);
        }

        bool is(BsdfType s) const {
                return is(s.specular_) && is(s.reflection_);
        }
private:
        Specular specular_;
        Reflection reflection_;
};

class Bsdf {
public:
        Bsdf (
                DifferentialGeometry const &shadingDG_
                // PBRT adds a refraction index parameter, see 10.1, p. 462
        );

        virtual ~Bsdf () {}


        //virtual bool hasAny (Reflection, Specular) const = 0;

        BsdfSample sample_f (
                const Vector &in, BsdfType, Random &
        ) const;

        Color f (const Vector &out, const Vector &in,
                BsdfType, Random&) const;
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

        bool hasComponent (BsdfType) const ;
        bool hasComponent (BsdfType::Specular s) const ;

private:
        Vector worldToLocal (Vector const &v) const ;
        Vector localToWorld (Vector const &v) const ;
        DifferentialGeometry const shadingDG;
        std::vector<shared_ptr<Bxdf> > bxdfs;

        Normal geometricNormal, shadingNormal;
        Vector s, t;

        int numComponents (BsdfType) const ;
};


class Bxdf {
public:
        Bxdf (BsdfType type)
        : bsdfType(type)
        {}

        virtual ~Bxdf () {}

        bool is (BsdfType s) const {
                return bsdfType.is(s);
        }

        bool is (BsdfType::Specular s) const {
                return bsdfType.is(s);
        }

        virtual BsdfSample sample_f (
                const Vector &in, Random &
        ) const = 0;
        virtual Color f (const Vector &out, const Vector &in, Random &) const = 0;

protected:
        const BsdfType bsdfType;
};
}

#endif // BSDF_HH_INCLUDED_20100118
