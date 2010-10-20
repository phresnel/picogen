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

#ifndef NAVIGATION_HH_20100915
#define NAVIGATION_HH_20100915

#include "stash.hh"
#include "point3d.hh"
#include <string>
#include <actuarius/bits/enum.hh>

namespace cosyscene {

class YawPitchRoll {
public:
        YawPitchRoll() : yaw(0), pitch(0), roll(0) {}

        double yaw, pitch, roll;
        Point3d position;

        template<typename Arch>
        void serialize (Arch &arch);
};
inline bool operator == (YawPitchRoll const &lhs, YawPitchRoll const &rhs) {
        return lhs.position == rhs.position
            && lhs.yaw   == rhs.yaw
            && lhs.pitch == rhs.pitch
            && lhs.roll  == rhs.roll;
}



class Navigation : public StashableMixin<Navigation>
{
public:
        enum Kind {
                None,
                YawPitchRoll
        };

        Navigation();

        Kind kind() const;
        void reset ();
        void toYawPitchRoll (cosyscene::YawPitchRoll const &);
        cosyscene::YawPitchRoll yawPitchRoll() const;

        template<typename Arch>
        void serialize (Arch &arch);
        static const actuarius::Enum<Kind> Typenames;

        bool data_equals(Navigation const &rhs) const;
private:
        Kind kind_;

        cosyscene::YawPitchRoll yawPitchRoll_;
};

} // namespace cosyscene

#endif // NAVIGATION_HH_20100915
