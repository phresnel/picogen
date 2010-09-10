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


#ifndef SUNSKY_HH_20100910
#define SUNSKY_HH_20100910

#include "stash.hh"
#include <string>
#include <actuarius/bits/enum.hh>

#include "direction3d.hh"

namespace cosyscene {
class UtahSky;
class SunSky;

class UtahSky {
public:
        UtahSky() :
                sunDirection(0,0.5,1),
                turbidity(2.3),
                sunSizeFactor(1),
                sunBrightnessFactor(1),
                atmosphereBrightnessFactor(1),
                atmosphericFxFactor(1),
                overcast(0),
                atmosphericEffects(0)
        {}

        template<typename Arch>
        void serialize (Arch &arch);

        Direction3d sunDirection;

        double turbidity;

        double sunSizeFactor;
        double sunBrightnessFactor;

        double atmosphereBrightnessFactor;
        double atmosphericFxFactor;

        double overcast;
        bool   atmosphericEffects;

};
inline bool operator == (UtahSky const &lhs, UtahSky const &rhs) {
        return lhs.sunDirection == rhs.sunDirection
            && lhs.turbidity == rhs.turbidity
            && lhs.sunSizeFactor == rhs.sunSizeFactor
            && lhs.sunBrightnessFactor == rhs.sunBrightnessFactor
            && lhs.atmosphereBrightnessFactor == rhs.atmosphereBrightnessFactor
            && lhs.overcast == rhs.overcast
            && lhs.atmosphericEffects == rhs.atmosphericEffects
        ;
}


class SunSky
{
public:
        enum Kind {
                None,
                UtahSky
        };

        SunSky();

        Kind kind() const;
        void reset();
        void toUtahSky (cosyscene::UtahSky const &qs);
        cosyscene::UtahSky utahSky() const;

        const Stash<SunSky>& getStash() const;
        Stash<SunSky>& getStash();

        void setStash(Stash<SunSky> const &);

        void stash();
        void clearStash();

        bool data_equals(SunSky const &rhs) const;

        template<typename Arch>
        void serialize (Arch &arch);
        static const actuarius::Enum<Kind> Typenames;
private:
        Stash<SunSky> stash_;
        Kind kind_;

        cosyscene::UtahSky utahSky_;
};

} // namespace cosyscene

#endif // SUNSKY_HH_20100910
