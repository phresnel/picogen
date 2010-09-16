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

#ifndef POINT3D_HH_20100915
#define POINT3D_HH_20100915

namespace cosyscene {


class Point3d {
public:
        Point3d () ;
        Point3d (double x, double y, double z);

        double x() const;
        double y() const;
        double z() const;

        template<typename Arch>
        inline void serialize (Arch &arch);

private:
        double x_, y_, z_;
};
inline bool operator == (Point3d const & lhs, Point3d const & rhs) {
        return lhs.x() == rhs.x()
            && lhs.y() == rhs.y()
            && lhs.z() == rhs.z();
}


} // namespace cosyscene

#endif // POINT3D_HH_20100915
