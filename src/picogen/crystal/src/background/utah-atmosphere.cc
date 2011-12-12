#include "utah-atmosphere.h"
#include "utah-sky/sunsky.hh"
#include "crystal_math.h"

namespace crystal { namespace background {

UtahAtmosphere::UtahAtmosphere (std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky)
        : pssSunSky_(pssSunSky)
{
}

AtmosphericEffects UtahAtmosphere::atmosphericEffects_ (const Point &viewer_,
                                   const Point &source_) const
{
        const redshift::background::PssSunSky &pss = *pssSunSky_;

        const redshift::Vector viewer (viewer_.x, viewer_.y, viewer_.z);
        const redshift::Vector source (source_.x, source_.y, source_.z);

        crystal::redshift::background::PssSunSky::Spectrum att, insc;
        pss.GetAtmosphericEffects (viewer, source, att, insc);

        const redshift::color::RGB att_rgb  = att.toRGB();
        const redshift::color::RGB insc_rgb = insc.toRGB();

        return AtmosphericEffects (
                        Radiance::FromRgb (att_rgb.R, att_rgb.G, att_rgb.B),
                        Radiance::FromRgb (insc_rgb.R, insc_rgb.G, insc_rgb.B));
}


} }
