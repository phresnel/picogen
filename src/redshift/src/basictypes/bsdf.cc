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

#include "../../include/basictypes/bsdf.hh"
#include "../../include/sampling.hh"

namespace redshift {


Bsdf::Bsdf (DifferentialGeometry const &shadingDG_
        // PBRT adds a refraction index parameter, see 10.1, p. 462
)
: shadingDG(shadingDG_)
, geometricNormal (shadingDG.getGeometricNormal())
, shadingNormal (shadingDG.getShadingNormal())
, s (normalize (shadingDG.get_dpdu()))
, t (cross (vector_cast<Vector>(shadingNormal), s))
//,s (normalize (shadingDG.get_dpdu()))
//,t (normalize (shadingDG.get_dpdv()))
{
}



Vector Bsdf::worldToLocal (Vector const &v) const {
        return Vector (
                dot (v, s),
                dot (v, vector_cast<Vector>(shadingNormal)),
                dot (v, t)
        );
}



Vector Bsdf::localToWorld (Vector const &v) const {
        return Vector (
                s.x*v.x + shadingNormal.x*v.y + t.x*v.z,
                s.y*v.x + shadingNormal.y*v.y + t.y*v.z,
                s.z*v.x + shadingNormal.z*v.y + t.z*v.z
        );
}



BsdfSample Bsdf::sample_f (
        const Vector &out_, BsdfFilter filter, Random &rand
) const {
        typedef std::vector<shared_ptr<Bxdf> >::const_iterator It;

        // Choose bxdf.
        const int nc = numComponents (filter);
        if (nc == 0) {
                return BsdfSample::null();
        }

        const int which_ = rand() * nc,
                  which = which_>nc-1 ? nc-1 : which_;
        shared_ptr<Bxdf> bxdf;

        int x = which;
        for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it) {
                if (filter.allows ((**it).type()))
                        if (x-- == 0) {
                                bxdf = *it;
                                break;
                        }
        }

        // Sample it.
        const Vector localOut = worldToLocal (out_);
        const BsdfSample sample = bxdf->sample_f(worldToLocal (localOut),
                                                 rand);
        if (sample.isNull()) {
                return BsdfSample::null();
        }
        const Vector localIn = sample.incident();
        const Vector worldIn = localToWorld (localIn);

        // Compute avergage pdf.
        real_t averagePdf = sample.pdf();
        if (!bxdf->type().isSpecular() && nc>1) {
                averagePdf = 0;
                for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it) {
                        if (filter.allows ((**it).type())) {
                                averagePdf += (**it).pdf (localOut, localIn);
                        }
                }
        }
        if (nc > 1)
                averagePdf /= nc;

        // Compute BSDF value.
        Color f;
        if (bxdf->type().isSpecular()) {
                f = sample.color();
        } else {
                f = f_local(localOut, localIn, filter, rand);
        }

        return BsdfSample (
                f,
                worldIn,
                averagePdf,
                sample.type()
        );
}



Color Bsdf::f (
        const Vector &out_, const Vector &in_,
        BsdfFilter s,
        Random &rand
) const {
        const Vector out = worldToLocal (out_);
        const Vector in  = worldToLocal (in_);
        return f_local(out, in, s, rand);
}



Color Bsdf::f_local (
        const Vector &out, const Vector &in,
        BsdfFilter s,
        Random &rand
) const {
        const real_t dotIn  = dot (vector_cast<Normal>(in), geometricNormal);
        const real_t dotOut = dot (vector_cast<Normal>(out), geometricNormal);
        if (dotIn * dotOut > 0) {
                // Both in same hemisphere -> BRDF only
                s.disable (Reflective);
        } else {
                // Different hemisphere -> BTDF only
                s.disable (Transmissive);
        }

        Color col;
        typedef std::vector<shared_ptr<Bxdf> >::const_iterator It;
        for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it) {
                if (s.allows ((**it).type())) {
                        col += (**it).f (out, in, rand);
                }
        }
        return col;
}



bool Bsdf::hasComponent (BsdfFilter s) const {
        typedef std::vector<shared_ptr<Bxdf> >::const_iterator It;
        for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it) {
                if (s.allows ((**it).type()))
                        return true;
        }
        return false;
}



/*bool Bsdf::hasComponent (BsdfType::Specular s) const {
        typedef std::vector<shared_ptr<Bxdf> >::const_iterator It;
        for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it)
                if (s.is ((**it).type()))
                        return true;
        return false;
}*/



int Bsdf::numComponents (BsdfFilter s) const {
        typedef std::vector<shared_ptr<Bxdf> >::const_iterator It;
        int ret = 0;
        for (It it = bxdfs.begin(); it!=bxdfs.end(); ++it)
                //if ((**it).is (s))
                if (s.allows((**it).type()))
                        ++ret;
        return ret;
}



}
