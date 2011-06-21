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

#include <ostream>
#include <fstream>
#include <string>

#include "actuarius/actuarius.hh"
#include "cosyscene/serialization/scene.ser.hh"

namespace picogen { namespace cosyscene {

void save_scene (const cosyscene::Scene &scene_, std::ostream &fs_) {
        actuarius::OArchive(fs_) & actuarius::pack("cosy", scene_);
}

void save_scene (const Scene &scene, std::string const &name) {
        std::ofstream fs(name.c_str());
        if (!fs.good()) {
                throw std::runtime_error(
                        "File '" + name + "' could not be opened for saving.");
        }
        save_scene (scene, fs);
}

void load_scene (cosyscene::Scene &scene, std::istream &fs) {
        scene = cosyscene::Scene();
        actuarius::IArchive (fs) & actuarius::pack("cosy", scene);
}

void load_scene (Scene &scene, std::string const &name) {
        std::ifstream fs(name.c_str());
        if (!fs.good()) {
                throw std::runtime_error(
                        "File '" + name + "' could not be opened for loading.");
        }
        load_scene (scene, fs);
}

} }
