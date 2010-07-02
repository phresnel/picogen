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

#include "../../include/basictypes/spectrum.hh"
#include "../../include/backgrounds/pss-adapter.hh"
#include "../../include/backgrounds/preetham-shirley-smits/sunsky.hh"

namespace redshift { namespace backgrounds {

class PssSunAdapter : public Sun {
public:
        PssSunAdapter (
                background::PssSunSky const &pss,
                real_t sunSizeFactor,
                real_t sunBrightnessFactor
        )
        : pss(pss)
        , sunSizeFactor(sunSizeFactor), sunBrightnessFactor(sunBrightnessFactor)
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
        PssAtmosphereAdapater (
                background::PssSunSky const &pss,
                real_t atmosphereBrightnessFactor
        )
        : pss(pss)
        , atmosphereBrightnessFactor(atmosphereBrightnessFactor)
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
        PssAtmosphericEffectsAdapter (
                background::PssSunSky const &pss,
                real_t atmosphericEffectsDistanceFactor
        )
        : pss(pss)
        , atmosphericEffectsDistanceFactor(atmosphericEffectsDistanceFactor)
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
: Sky( new PssSunAdapter (*preetham_, sunSizeFactor, sunBrightnessFactor)
     , new PssAtmosphereAdapater(*preetham_, atmosphereBrightnessFactor)
     , new PssAtmosphericEffectsAdapter(*preetham_, atmosphericFxDistanceFactor)
     )
{
}
PssAdapter::~PssAdapter() {
        delete atmosphericEffects();
        delete atmosphere();
        delete sun();
}

} } // namespace redshift { namespace backgrounds {
