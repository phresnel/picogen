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

#ifndef SCENE_HH_20101013
#define SCENE_HH_20101013

namespace redshift_file {
        // Scene = (RenderSettings+)(FilmSettings)(Objects*)
        class Scene {
                std::vector<Object> objects_;
                std::vector<Volume> volumes_;
                std::vector<RenderSettings> renderSettings_;
                std::vector<Camera> cameras_;
                std::vector<Background> backgrounds_;
                FilmSettings filmSettings_;
        public:
                void addRenderSettings (RenderSettings const &rs) {
                        renderSettings_.push_back (rs);
                }
                unsigned int renderSettingsCount() const {
                        return renderSettings_.size();
                }
                RenderSettings renderSettings(unsigned int index) const {
                        return renderSettings_[index];
                }
                RenderSettings & renderSettings(unsigned int index) {
                        return renderSettings_[index];
                }
                void pruneRenderSettings () {
                        renderSettings_.clear();
                }

                void addObject (Object const &o) {
                        objects_.push_back (o);
                }
                unsigned int objectCount() const {
                        return objects_.size();
                }
                Object const & object(unsigned int index) const {
                        return objects_[index];
                }


                void addVolume (Volume const &o) {
                        volumes_.push_back (o);
                }
                unsigned int volumeCount() const {
                        return volumes_.size();
                }
                Volume const & volume(unsigned int index) const {
                        return volumes_[index];
                }

                void addBackground (Background const &o) {
                        backgrounds_.push_back (o);
                }
                unsigned int backgroundCount() const {
                        return backgrounds_.size();
                }
                Background const & background(unsigned int index) const {
                        return backgrounds_[index];
                }


                void addCamera (Camera const &o) {
                        cameras_.push_back (o);
                }
                unsigned int cameraCount() const {
                        return cameras_.size();
                }
                Camera const & camera(unsigned int index) const {
                        return cameras_[index];
                }
                void pruneCameras () {
                        cameras_.clear();
                }

                FilmSettings filmSettings () const {
                        return filmSettings_;
                }
                void setFilmSettings (FilmSettings const &fs) {
                        filmSettings_ = fs;
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);
        };
}

#endif // SCENE_HH_20101013
