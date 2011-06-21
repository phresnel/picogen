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

#ifndef CAMERA_SERIALIZATION_HH_20110117
#define CAMERA_SERIALIZATION_HH_20110117

#include "../camera.hh"


namespace picogen { namespace cosyscene {

template<typename Arch>
inline void PinholeCamera::serialize (Arch &arch) {
        arch & actuarius::pack ("front-plane-distance", frontPlaneDistance_);
}

template<typename Arch>
inline void CylindricalCamera::serialize (Arch &arch) {
        arch & actuarius::pack ("front-plane-distance", frontPlaneDistance_);
}

template<typename Arch>
inline void CubemapFaceCamera::serialize (Arch &arch) {
        arch & actuarius::pack ("face", face_, Typenames);
}

template<typename Arch>
inline void Camera::serialize (Arch &arch) {
        using actuarius::pack;

        if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);

        arch & pack ("type", kind_, Typenames);

        switch (kind_) {
        case pinhole:
                arch & pack ("parameters", pinholeCamera_); break;
                break;
        case cylindrical:
                arch & pack ("parameters", cylindricalCamera_); break;
                break;
        case cubemap_face:
                arch & pack ("parameters", cubemapFaceCamera_); break;
                break;
        }
}


} }

#endif // CAMERA_SERIALIZATION_HH_20110117
