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

#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

#include "normal.hh"
#include "color.hh"

namespace picogen { namespace redshift_file {
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

                template<typename Arch>  void serialize (Arch &arch);
        };
} }

#endif // BACKGROUND_HH_20101013
