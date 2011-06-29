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
#ifndef JOB_HH
#define JOB_HH

#include "../../actuarius/actuarius.hh"
#include "jobfile.hh"
#include <sstream>
#undef main

struct RenderJob {

        std::string title;
        redshift::scenefile::Scene scene;

        unsigned int cameraIndex;
        unsigned int renderSettingsIndex;

        RenderJob() :
                title(""),
                cameraIndex(0),
                renderSettingsIndex(0)
        {}

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack("scene", scene);
                arch & pack("title", title);
                arch & pack("render-settings-index", renderSettingsIndex);
                arch & pack("camera-index", cameraIndex);
        }

        redshift::scenefile::RenderSettings getRenderSettings () const {
                return scene.renderSettings(renderSettingsIndex);
        }
        redshift::scenefile::Camera getCamera () const {
                return scene.camera(cameraIndex);
        }
};

struct Status {
        unsigned int currentScanline;

        Status()
        : currentScanline(0)
        {}

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack("current-scanline", currentScanline);
        }
};


class QFile;
class QString;

RenderJob *loadFromFile (QFile &);
void prepareJob(RenderJob &, QString const &statusDir);
Status readStatus (QString const &statusDir);
void writeStatus (Status const &, QString const &statusDir);

#endif // JOB_HH
