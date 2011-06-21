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


#ifndef SUNSKY_SERIALIZATION_HH_20100910
#define SUNSKY_SERIALIZATION_HH_20100910

#include "../sunsky.hh"
#include "cosyscene/serialization/stash.ser.hh"
#include "cosyscene/serialization/direction3d.ser.hh"


namespace picogen { namespace cosyscene {

template<typename Arch>
inline void UtahSky::serialize (Arch &arch) {
        using actuarius::pack;
        arch & actuarius::push_optional(true);
        arch & pack ("sun-direction", sunDirection);
        arch & pack ("sun-size-factor", sunSizeFactor);
        arch & pack ("sun-brightness-factor", sunBrightnessFactor);
        arch & pack ("atmosphere-brightness-factor", atmosphereBrightnessFactor);
        arch & pack ("atmospheric-effects-factor", atmosphericFxFactor);
        arch & pack ("atmospheric-effects", atmosphericEffects);
        arch & pack ("turbidity", turbidity);
        arch & pack ("overcast", overcast);
        arch & actuarius::pop_optional;
}

template<typename Arch>
inline void SunSky::serialize (Arch &arch) {
        using actuarius::pack;
        if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);

        arch & pack("type", kind_, Typenames);

        switch (kind_) {
        case UtahSky: arch & pack ("parameters", utahSky_); break;
        case None: break;
        }
}

} }

#endif // SUNSKY_SERIALIZATION_HH_20100910
