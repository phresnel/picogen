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

#include "../../include/material/leaf0.hh"
#include "../../include/basictypes/bsdf.hh"
#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/basictypes/texture.hh"

#include "../../include/bxdf/lambertian.hh"
#include "../../include/bxdf/brdftobtdf.hh"


namespace redshift { namespace material {



Leaf0::Leaf0 (shared_ptr<ColorTexture> color,
              shared_ptr<ScalarTexture> transmissionProb
)
: color(color), transmissionProb(transmissionProb)
, random(1,31,101,221)
{
        random.skip(1024);
}



shared_ptr<Bsdf> Leaf0::getBsdf(const DifferentialGeometry &dgGeom) const {
        shared_ptr<Bsdf> bsdf (new Bsdf(dgGeom));

        shared_ptr<Bxdf> lambertian (new bsdf::Lambertian (color->color(dgGeom)));

        //if (random() < transmissionProb->value(dgGeom))
                bsdf->add (shared_ptr<Bxdf>(new bsdf::BrdfToBtdf(lambertian)));
        /*else
                bsdf->add (lambertian);*/
        return bsdf;
}



real_t Leaf0::alpha (const DifferentialGeometry &dgGeom) const {
        return color->alpha(dgGeom);
}



} }

