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


inline bool sameHemisphere (Vector const &a, Vector const &b) {
        return a.y * b.y > 0;
}


enum ReflectionKind {
        Reflective,
        Transmissive
};

enum SpecularKind {
        Specular,
        Diffuse
};


class BsdfType {
public:
        BsdfType(ReflectionKind refl, SpecularKind spec)
        : refl(refl), spec(spec)
        {}

        ReflectionKind reflection() const {
                return refl;
        }
        ReflectionKind reflectionKind() const {
                return refl;
        }

        SpecularKind specular() const {
                return spec;
        }
        SpecularKind specularKind() const {
                return spec;
        }

        bool isReflective()   const { return refl == Reflective; }
        bool isTransmissive() const { return refl == Transmissive; }
        bool isSpecular()     const { return spec == Specular; }
        bool isDiffuse()      const { return spec == Diffuse; }

private:
        ReflectionKind refl;
        SpecularKind spec;
};


class BsdfFilter {
public:
        BsdfFilter(bool Reflective, bool Transmissive,
                   bool Specular, bool Diffuse)
        : Reflective_(Reflective)
        , Transmissive_(Transmissive)
        , Specular_(Specular)
        , Diffuse_(Diffuse)
        {}

        static BsdfFilter all() {
                return BsdfFilter(true, true, true, true);
        }
        static BsdfFilter allDiffuse() {
                return BsdfFilter(true, true, false, true);
        }
        static BsdfFilter allSpecular() {
                return BsdfFilter(true, true, true, false);
        }
        static BsdfFilter none() {
                return BsdfFilter(false, false, false, false);
        }
        /*BsdfFilter operator | (BsdfFilter rhs) const {
                return BsdfFilter(Reflective_  || rhs.Reflective_,
                                  Transmissive_|| rhs.Transmissive_,
                                  Specular_    || rhs.Specular_,
                                  Diffuse_     || rhs.Diffuse_);
        }*/

        void enable (ReflectionKind kind) {
                switch (kind) {
                case Reflective: Reflective_ = true; break;
                case Transmissive: Transmissive_ = true; break;
                }
        }
        void disable (ReflectionKind kind) {
                switch (kind) {
                case Reflective: Reflective_ = false; break;
                case Transmissive: Transmissive_ = false; break;
                }
        }


        void enable (SpecularKind kind) {
                switch (kind) {
                case Specular: Specular_ = true; break;
                case Diffuse:  Diffuse_ = true; break;
                }
        }
        void disable (SpecularKind kind) {
                switch (kind) {
                case Specular: Specular_ = false; break;
                case Diffuse:  Diffuse_ = false; break;
                }
        }

        bool allows (BsdfType type) const {
                return allows(type.reflection()) && allows(type.specular());
        }

        bool allows (ReflectionKind refl) const {
                switch (refl) {
                case Reflective: if (!Reflective_) return false; break;
                case Transmissive: if (!Transmissive_) return false; break;
                }
                return true;
        }

        bool allows (SpecularKind spec) const {
                switch (spec) {
                case Specular: if (!Specular_) return false; break;
                case Diffuse: if (!Diffuse_) return false; break;
                }
                return true;
        }
private:
        bool Reflective_, Transmissive_;
        bool Specular_, Diffuse_;
};





SEALED(BsdfSample);
class BsdfSample : MAKE_SEALED(BsdfSample) {
public:
        BsdfSample(Color const &R,
                   Vector const &incident,
                   real_t pdf,
                   BsdfType sampledType)
        : reflection_(R)
        , incident_(incident)
        , pdf_(pdf)
        , sampledType_(sampledType)
        {}

        Color  color ()    const { return reflection_; }
        Vector incident () const { return incident_; }
        real_t pdf ()      const { return pdf_; }
        BsdfType type()    const { return sampledType_; }

        static BsdfSample null() {
                return BsdfSample(
                        Color(0),
                        Vector(),
                        0,
                        BsdfType(Reflective, Diffuse));
        }

        bool isNull() const {
                return pdf_ == 0;
        }
private:
        Color reflection_;
        Vector incident_;
        real_t pdf_;
        BsdfType sampledType_;
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
                const Vector &in, BsdfFilter, Random &
        ) const;

        Color f (const Vector &out, const Vector &in,
                BsdfFilter, Random&) const;
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

        bool hasComponent (BsdfFilter) const ;
        bool hasComponent (SpecularKind s) const ;

private:
        Color f (const Vector &worldOut, const Vector &worldIn,
                 const Vector &localOut, const Vector &localIn,
                 BsdfFilter, Random&) const;

        Vector worldToLocal (Vector const &v) const ;
        Vector localToWorld (Vector const &v) const ;
        DifferentialGeometry const shadingDG;
        std::vector<shared_ptr<Bxdf> > bxdfs;

        Normal geometricNormal, shadingNormal;
        Vector s, t;

        int numComponents (BsdfFilter) const ;
};


class Bxdf {
public:
        Bxdf (BsdfType type)
        : bsdfType(type)
        {}

        virtual ~Bxdf () {}

        BsdfType type() const {
                return bsdfType;
        }

        virtual BsdfSample sample_f (
                const Vector &in, Random &
        ) const = 0;
        virtual Color f (const Vector &out, const Vector &in, Random &) const = 0;
        virtual real_t pdf (const Vector &out, const Vector &in) const = 0;

protected:
        const BsdfType bsdfType;
};
}

#endif // BSDF_HH_INCLUDED_20100118
