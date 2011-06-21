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

#include "filmsettings.hh"
#include "shared_ptr.hh"

namespace redshift_file {
        class Object;
        class Volume;
        class RenderSettings;
        class Camera;
        class Background;
        
        class SceneImpl;
        
        // Scene = (RenderSettings+)(FilmSettings)(Objects*)
        class Scene {                
        public:
                Scene();
                Scene (Scene const &);
                Scene& operator= (Scene const &);

                void addRenderSettings (RenderSettings const &rs);
                unsigned int renderSettingsCount() const;
                RenderSettings renderSettings(unsigned int index) const;
                RenderSettings & renderSettings(unsigned int index);
                void pruneRenderSettings ();

                void addObject (Object const &o);
                unsigned int objectCount() const;
                Object const & object(unsigned int index) const;

                void addVolume (Volume const &o);
                unsigned int volumeCount() const;
                Volume const & volume(unsigned int index) const;

                void addBackground (Background const &o);
                unsigned int backgroundCount() const;                
                Background const & background(unsigned int index) const;
                
                void addCamera (Camera const &o);
                unsigned int cameraCount() const;                
                Camera const & camera(unsigned int index) const;
                void pruneCameras ();
                
                FilmSettings filmSettings () const;
                void setFilmSettings (FilmSettings const &fs);

                // Serialization.
                template<typename Arch> void serialize (Arch &arch);
                
                // For saving away a const-referenced-scene. Feels way
                // better than a const_cast.
                template <typename Arch>
                void serialize (Arch &arch) const {
                        Scene tmp = *this;
                        return tmp.serialize(arch);
                }
                
        private:
                boost::shared_ptr<SceneImpl> impl;                
        };
}

#endif // SCENE_HH_20101013
