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


#ifndef COSYSCENE_HH_20100902
#define COSYSCENE_HH_20100902

#include "redshift/include/smart_ptr.hh"
#include "stash.hh"

namespace redshift_file {
         class Scene;
}

namespace cosyscene {

class Terrain;
class Water;
class SunSky;
class Navigation;
class RenderSettings;
class TwinRenderSettings;
class FilmSettings;
class Camera;

class Scene
{
public:
        Scene();

        redshift::shared_ptr<redshift_file::Scene> toRedshiftScene(
                        bool usePreviewSettings=true) const;
        std::string finalTerrainCode() const;

        redshift::shared_ptr<Terrain> terrain() const {
                return terrain_;
        }
        redshift::shared_ptr<Water> water() const {
                return water_;
        }
        redshift::shared_ptr<SunSky> sunSky() const {
                return sunSky_;
        }
        redshift::shared_ptr<Navigation> navigation() const {
                return navigation_;
        }

        redshift::shared_ptr<Camera> camera() const {
                return camera_;
        }

        redshift::shared_ptr<TwinRenderSettings> renderSettings() const {
                return renderSettings_;
        }

        redshift::shared_ptr<FilmSettings> filmSettings() const {
                return filmSettings_;
        }

        template <typename Arch>
        void serialize (Arch &arch); // inl/scene.inl.hh

        template <typename Arch>
        void serialize (Arch &arch) const {
                Scene tmp = *this; // feels better than const_cast
                return tmp.serialize(arch);
        }

private:
        redshift::shared_ptr<Terrain> terrain_;
        redshift::shared_ptr<Water> water_;
        redshift::shared_ptr<SunSky> sunSky_;
        redshift::shared_ptr<Navigation> navigation_;
        redshift::shared_ptr<Camera> camera_;

        redshift::shared_ptr<TwinRenderSettings> renderSettings_;
        redshift::shared_ptr<FilmSettings> filmSettings_;
};
}

#endif // COSYSCENE_HH_20100902
