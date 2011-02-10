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

#ifndef CAMERA_HH_20110117
#define CAMERA_HH_20110117

#include "stash.hh"
#include "redshift/include/smart_ptr.hh"
#include <actuarius/bits/enum.hh>

namespace cosyscene {

class PinholeCamera {
public:
        PinholeCamera();

        double frontPlaneDistance() const;
        void setFrontPlaceDistance(double);

        template<typename Arch> void serialize (Arch &arch);
private:
        double frontPlaneDistance_;
};
bool operator ==(PinholeCamera const&, PinholeCamera const&);

class CylindricalCamera {
public:
        CylindricalCamera();

        double frontPlaneDistance() const;
        void setFrontPlaceDistance(double);

        template<typename Arch> void serialize (Arch &arch);
private:
        double frontPlaneDistance_;
};
bool operator ==(CylindricalCamera const&, CylindricalCamera const&);

class CubemapFaceCamera {
public:
        enum Face {
                left, right,
                front, back,
                bottom, top
        };
        static const actuarius::Enum<Face> Typenames;

        CubemapFaceCamera();

        Face face() const;
        void setFace (Face face);

        template<typename Arch> void serialize (Arch &arch);
private:
        Face face_;
};
bool operator ==(CubemapFaceCamera const&, CubemapFaceCamera const&);



class Camera;
bool operator == (Camera const &lhs, Camera const &rhs);
class Camera : public StashableMixin<Camera> {
public:
        enum Kind {
                pinhole,
                cylindrical,
                cubemap_face
        };
        static const actuarius::Enum<Kind> Typenames;

        Camera();

        Kind kind() const;

        void to (Kind);
        void toPinholeCamera     (PinholeCamera     const& = PinholeCamera());
        void toCylindricalCamera (CylindricalCamera const& = CylindricalCamera());
        void toCubemapFaceCamera (CubemapFaceCamera const& = CubemapFaceCamera());

        PinholeCamera     pinholeCamera     () const;
        CylindricalCamera cylindricalCamera () const;
        CubemapFaceCamera cubemapFaceCamera () const;

        bool data_equals(Camera const &rhs) const { return *this == rhs; }

        template<typename Arch> void serialize (Arch &arch);

private:
        PinholeCamera     pinholeCamera_;
        CylindricalCamera cylindricalCamera_;
        CubemapFaceCamera cubemapFaceCamera_;
        Kind kind_;
};

} // namespace cosyscene

#endif // CAMERA_HH_20110117
