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

#ifndef SCENE_SER_HH_20101013
#define SCENE_SER_HH_20101013

#include "../scene.hh"
#include "../scene.impl.hh"

#include "rendersettings.ser.hh"
#include "camera.ser.hh"
#include "object.ser.hh"
#include "volume.ser.hh"
#include "background.ser.hh"
#include "filmsettings.ser.hh"

namespace picogen { namespace redshift_file {
        template<typename Arch>
        void Scene::serialize (Arch &arch) {
                using actuarius::pack;

                arch & pack ("render-settings", &RenderSettings::title, impl->renderSettings_);
                arch & pack ("cameras", &Camera::title, impl->cameras_);
                arch & pack ("objects", &Object::type, Object::Typenames, impl->objects_);
                arch & pack ("volumes", &Volume::type, Volume::Typenames, impl->volumes_);
                arch & pack ("backgrounds", &Background::type, Background::Typenames, impl->backgrounds_);
                arch & pack ("film-settings", impl->filmSettings_);
        }
} }

#endif // SCENE_SER_HH_20101013
