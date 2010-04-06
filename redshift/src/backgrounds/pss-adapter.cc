//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
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

#include "../../include/backgrounds/pss-adapter.hh"

namespace redshift { namespace backgrounds {

PssAdapter::PssAdapter (
        shared_ptr<redshift::background::PssSunSky> preetham,
        real_t sunSizeFactor
)
: preetham (preetham)
, sunSizeFactor(sunSizeFactor)
{
}

bool PssAdapter::isInSunSolidAngle (Vector const & vector) const {
        const real_t
                dotS = dot(vector, getSunDirection()),
                alpha = acos (dotS),
                sr = 2 * constants::pi * (1 - std::cos(alpha/2))
        ;
        return sr < (preetham->GetSunSolidAngle() * sunSizeFactor);
}

Color PssAdapter::query (Ray const &ray) const {
        return Color(query_(ray));
}

redshift::background::PssSunSky::Spectrum
PssAdapter::query_ (Ray const &ray) const {
        using redshift::background::PssSunSky;

        const real_t teh_sun = isInSunSolidAngle (ray.direction) ? 1.f : 0.f;

        return preetham->GetSkySpectralRadiance (ray.direction)
                + teh_sun * preetham->GetSunSpectralRadiance();
}

Color PssAdapter::getSunColor () const {
        return Color(preetham->GetSunSpectralRadiance());
}

Color PssAdapter::querySun (Ray const &ray) const {
        const real_t teh_sun = isInSunSolidAngle (ray.direction)?1.f:0.f;
        return getSunColor() * teh_sun;
}
/*
Color PssAdapter::diffuseQuery (
        Point const &poi, Normal const &normal, Random &rand
) const {
        Color sum = Color::FromRGB (0,0,0);
        Ray ray;
        ray.position = poi;

        int numSamples;
        for (numSamples = 0; numSamples < 10; ++numSamples) {
                const tuple<real_t,real_t,real_t> sphere = cosineHemisphereR(rand);
                const tuple<Vector,Vector,Vector> cs = coordinateSystem (normal);

                const real_t &sx = get<0>(sphere);
                const real_t &sy = get<1>(sphere);
                const real_t &sz = get<2>(sphere);
                const Vector &X = get<0>(cs);
                const Vector &Y = get<1>(cs);
                const Vector &Z = get<2>(cs);
                const Vector d = X * sx + Y * sy + Z * sz;
                if (d.y > 0) {
                        ray.direction = d;
                        sum = sum + query (ray);
                }
        }
        return sum / Color::real_t(numSamples);

        return query (Ray(poi, vector_cast<Vector>(normal)));
}*/

Color PssAdapter::atmosphereShade (
        Color const &color, Ray const &ray, real_t distance
) const {
        if (!preetham->atmosphericEffectsEnabled()) {
                return color;
        }

        const Vector viewer = vector_cast<Vector>(ray.position);
        const Vector source = vector_cast<Vector>(ray(distance));

        /*if (distance == constants::infinity)
                return color;*/

        typedef background::PssSunSky::Spectrum SSpectrum;
        SSpectrum attenuation(SSpectrum::noinit), inscatter(SSpectrum::noinit);
        preetham->GetAtmosphericEffects (
                viewer, source,
                attenuation, inscatter
        );

        //return Color (SSpectrum(color) * attenuation + inscatter * (1.-attenuation));
        return Color (SSpectrum(color) * attenuation + inscatter);
}

} } // namespace redshift { namespace backgrounds {
