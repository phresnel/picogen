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

#ifndef VECTOR3D_HH_20100910
#define VECTOR3D_HH_20100910

#include "cosyfloat.hh"

namespace cosyscene {


class Vector3d {
public:
        Vector3d () ;
        Vector3d (CosyFloat x, CosyFloat y, CosyFloat z);

        CosyFloat x() const;
        CosyFloat y() const;
        CosyFloat z() const;

        template<typename Arch>
        inline void serialize (Arch &arch);

private:
        CosyFloat x_, y_, z_;
};
inline bool operator == (Vector3d const & lhs, Vector3d const & rhs) {
        return lhs.x() == rhs.x()
            && lhs.y() == rhs.y()
            && lhs.z() == rhs.z();
}
CosyFloat length (Vector3d);


} // namespace cosyscene

#endif // VECTOR3D_HH_20100910
