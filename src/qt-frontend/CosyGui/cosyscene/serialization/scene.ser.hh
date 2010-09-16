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


#ifndef SCENE_INL_HH_20100902
#define SCENE_INL_HH_20100902

#include "../scene.hh"

#include "cosyscene/serialization/terrain.ser.hh"
#include "cosyscene/serialization/sunsky.ser.hh"
#include "cosyscene/serialization/navigation.ser.hh"

namespace cosyscene {
template<typename Arch>
inline void Scene::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack("terrain", *terrain_);
        arch & pack("sunsky", *sunSky_);
        arch & pack("navigation", *navigation_);
}
}

#endif // SCENE_INL_HH
