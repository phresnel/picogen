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

#ifndef BACKGROUND_HH_20101013
#define BACKGROUND_HH_20101013

namespace redshift_file {
        struct Background {
                enum Type {
                        pss_sunsky
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Normal sunDirection;
                double turbidity;
                double sunSizeFactor;
                double sunBrightnessFactor;
                double atmosphereBrightnessFactor;
                double atmosphericFxFactor;
                double overcast;
                Color sunColor;
                Color skyFilter;
                bool atmosphericEffects;


                Background ()
                : type(pss_sunsky)
                , sunDirection(0,0.5,2)
                , turbidity(2.5)
                , sunSizeFactor(1)
                , sunBrightnessFactor(1)
                , atmosphereBrightnessFactor(1)
                , atmosphericFxFactor(1)
                , sunColor(3,3,3)
                , skyFilter(0.05,0.05,0.05)
                , atmosphericEffects(true)
                {}

                shared_ptr<redshift::Sky> toSky() const {
                        using namespace redshift;
                        switch (type) {
                        case pss_sunsky: {
                        #if 1
                                shared_ptr<redshift::background::PssSunSky> preetham (
                                 new background::PssSunSky(
                                        normalize(Vector(sunDirection.x,sunDirection.y,sunDirection.z)),
                                        turbidity,
                                        overcast,
                                        atmosphericEffects
                                ));
                                return shared_ptr<redshift::Sky> (
                                  new backgrounds::PssAdapter (
                                        preetham,
                                        sunSizeFactor, sunBrightnessFactor,
                                        atmosphereBrightnessFactor, atmosphericFxFactor
                                ));
                        #else
                                shared_ptr<redshift::background::Preetham> preetham (
                                 new redshift::background::Preetham());

                                preetham->setSunDirection(Vector(sunDirection.x,sunDirection.y,sunDirection.z));
                                preetham->setTurbidity(turbidity);
                                preetham->setSunColor(Color::FromRGB(sunColor.r,sunColor.g,sunColor.b));
                                preetham->setColorFilter(Color::FromRGB(skyFilter.r,skyFilter.g,skyFilter.b));
                                preetham->enableFogHack (false, 0.00025f, 150000);
                                preetham->invalidate();
                                return shared_ptr<redshift::Background> (
                                        new backgrounds::PreethamAdapter (preetham)
                                );
                        #endif
                        } break;
                        };
                        return shared_ptr<redshift::Sky>();
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        switch (type) {
                        case pss_sunsky:
                                arch & actuarius::push_optional(true);
                                arch & pack ("sun-direction", sunDirection);
                                arch & pack ("sun-size-factor", sunSizeFactor);
                                arch & pack ("sun-brightness-factor", sunBrightnessFactor);
                                arch & pack ("atmosphere-brightness-factor", atmosphereBrightnessFactor);
                                arch & pack ("atmospheric-effects-factor", atmosphericFxFactor);
                                arch & pack ("atmospheric-effects", atmosphericEffects);
                                arch & pack ("turbidity", turbidity);
                                arch & pack ("overcast", overcast);
                                /*arch & pack ("sun-color", sunColor);
                                arch & pack ("sky-filter", skyFilter);*/
                                arch & actuarius::pop_optional;
                                break;
                        };
                }
        };
}

#endif // BACKGROUND_HH_20101013
