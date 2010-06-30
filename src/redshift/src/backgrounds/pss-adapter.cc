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


class PssSunAdapter : public Sun {
public:
        PssSunAdapter (background::PssSunSky const &pss)
        : pss(pss)
        , sunSizeFactor(1), sunBrightnessFactor(1)
        {
        }
        Vector direction() const {
                return pss.GetSunPosition();
        }
        Color color(Ray const &ray) const {
                const real_t isIn = isInSunSolidAngle(ray.direction)?1:0;
                return Color(
                  sunBrightnessFactor*isIn
                  *(pss.GetSunSpectralRadiance()
                   +pss.GetSkySpectralRadiance(ray.direction))
                );
        }
        bool isInSunSolidAngle(Vector const &vector_) const {
                Vector const vector = normalize (vector_);
                const real_t
                  dotS_ = dot(vector, direction()),
                  dotS = dotS_ > 1 ? 1 : dotS_, // this would be the case e.g. with
                                                // normalize(Vector(1,1,0.1)) as input
                  alpha = acos (dotS),
                  sr = 2 * constants::pi * (1 - std::cos(alpha/2))
                ;
                return sr < (pss.GetSunSolidAngle() * sunSizeFactor);
        }
private:
        background::PssSunSky const &pss;
        real_t sunSizeFactor, sunBrightnessFactor;
};

class PssAtmosphereAdapater : public Atmosphere {
public:
        PssAtmosphereAdapater (background::PssSunSky const &pss)
        : pss(pss)
        , atmosphereBrightnessFactor(1)
        {
        }

        Color color(Ray const &ray) const {
                return Color(
                        atmosphereBrightnessFactor*
                        pss.GetSkySpectralRadiance (ray.direction)
                );
        }
private:
        background::PssSunSky const &pss;
        real_t atmosphereBrightnessFactor;
};

class PssAtmosphericEffectsAdapter : public AtmosphericEffects {
public:
        PssAtmosphericEffectsAdapter (background::PssSunSky const &pss)
        : pss(pss)
        , atmosphericEffectsDistanceFactor(1)
        {
        }
        Color shade (Color const &color, Ray const &ray, real_t distance) const {
                if (!pss.atmosphericEffectsEnabled()) {
                        return color;
                }

                distance *= atmosphericEffectsDistanceFactor;
                if (distance == constants::infinity)
                        distance = 10000000;

                if (ray.direction.y < 0 && ray.position.y + ray.direction.y * distance < 0) {
                        // py + dy * d = 0
                        // dy * d = -py
                        // d      = -py / dy
                        distance = -ray.position.y / ray.direction.y;
                }

                const Vector viewer = vector_cast<Vector>(ray.position);
                const Vector source = vector_cast<Vector>(ray(distance));

                typedef background::PssSunSky::Spectrum SSpectrum;
                SSpectrum attenuation(SSpectrum::noinit), inscatter(SSpectrum::noinit);
                pss.GetAtmosphericEffects (
                        viewer, source,
                        attenuation, inscatter
                );

                return Color (SSpectrum(color) * attenuation)
                       + inscatter;
        }
private:
        background::PssSunSky const &pss;
        real_t atmosphericEffectsDistanceFactor;
};



PssAdapter::PssAdapter (
        shared_ptr<redshift::background::PssSunSky> preetham_,
        real_t sunSizeFactor,
        real_t sunBrightnessFactor,
        real_t atmosphereBrightnessFactor,
        real_t atmosphericFxDistanceFactor
)
: Sky( new PssSunAdapter (*preetham_)
     , new PssAtmosphereAdapater(*preetham_)
     , new PssAtmosphericEffectsAdapter(*preetham_)
     )
, preetham (preetham_)
, sunSizeFactor(sunSizeFactor)
, sunBrightnessFactor(sunBrightnessFactor)
, atmosphereBrightnessFactor(atmosphereBrightnessFactor)
, atmosphericFxDistanceFactor(atmosphericFxDistanceFactor)
{
}
PssAdapter::~PssAdapter() {
        delete atmosphericEffects();
        delete atmosphere();
        delete sun();
}

bool PssAdapter::isInSunSolidAngle (Vector const & vector_) const {
        Vector const vector = normalize (vector_);
        const real_t
                dotS_ = dot(vector, getSunDirection()),
                dotS = dotS_ > 1 ? 1 : dotS_, // this would be the case e.g. with
                                              // normalize(Vector(1,1,0.1)) as input
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
        return Color(atmosphereBrightnessFactor*preetham->GetSkySpectralRadiance (ray.direction))
                /*+ querySun (ray)*/;
}

Color PssAdapter::getSunColor () const {
        return Color(
          sunBrightnessFactor
          *(preetham->GetSunSpectralRadiance()
           +preetham->GetSkySpectralRadiance(preetham->GetSunPosition()))
        );
}

Color PssAdapter::querySun (Ray const &ray) const {
        const Vector &vector = ray.direction;
        const real_t teh_sun = isInSunSolidAngle (vector)?1.f:0.f;
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

        distance *= atmosphericFxDistanceFactor;
        if (distance == constants::infinity)
                distance = 10000000;

        if (ray.direction.y < 0 && ray.position.y + ray.direction.y * distance < 0) {
                // py + dy * d = 0
                // dy * d = -py
                // d      = -py / dy
                distance = -ray.position.y / ray.direction.y;
        }

        const Vector viewer = vector_cast<Vector>(ray.position);
        const Vector source = vector_cast<Vector>(ray(distance));

        typedef background::PssSunSky::Spectrum SSpectrum;
        SSpectrum attenuation(SSpectrum::noinit), inscatter(SSpectrum::noinit);
        preetham->GetAtmosphericEffects (
                viewer, source,
                attenuation, inscatter
        );

        return Color (SSpectrum(color) * attenuation)
               + inscatter;
}

} } // namespace redshift { namespace backgrounds {
