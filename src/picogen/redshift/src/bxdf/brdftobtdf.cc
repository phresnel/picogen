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

#include "../../include/bxdf/brdftobtdf.hh"
#include "../../include/random.hh"
#include "../../include/sampling.hh"

namespace picogen { namespace redshift { namespace bsdf {



BrdfToBtdf::BrdfToBtdf (shared_ptr<const Bxdf> brdf)
: Bxdf (BsdfType(Transmissive, brdf->type().specularKind()))
, brdf (brdf)
{}



BsdfSample BrdfToBtdf::sample_f (
        const Vector &out, Random &rand
) const {
        const BsdfSample tmp = brdf->sample_f(out, rand);
        const Vector
                incident_ = tmp.incident(),
                incident = Vector(incident_.x, -incident_.y, incident_.z);

        return BsdfSample(tmp.color(),
                          incident,
                          tmp.pdf(),
                          this->type()); // <-- important
}



Color BrdfToBtdf::f (const Vector &out, const Vector &in, Random &rand) const {
        return brdf->f(out, Vector(in.x, -in.y, in.z), rand);
}



real_t BrdfToBtdf::pdf (const Vector &out, const Vector &in) const {
        return brdf->pdf(out, Vector(in.x, -in.y, in.z));
}



} } }
