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

#include "scene.hh"
#include "terrain.hh"
#include "sunsky.hh"
#include "navigation.hh"

#include "redshift/include/jobfile.hh"

namespace cosyscene {

Scene::Scene()
: terrain_(new Terrain())
, sunSky_(new SunSky())
, navigation_(new Navigation())
{
        navigation_->toYawPitchRoll(cosyscene::YawPitchRoll());
        sunSky_->toUtahSky(cosyscene::UtahSky());
}


redshift::shared_ptr<redshift::scenefile::Scene> Scene::toRedshiftScene() const {
        using namespace ::redshift;
        shared_ptr<scenefile::Scene> scenePtr(new scenefile::Scene);
        scenefile::Scene &scene = *scenePtr;

        // Camera
        const cosyscene::Navigation &nav = *navigation_;

        scenefile::Camera cam;
        cam.type = scenefile::Camera::pinhole;
        cam.pinholeParams.front = 1;

        switch (nav.kind()) {
        case cosyscene::Navigation::YawPitchRoll: {
                        cosyscene::YawPitchRoll ypr = nav.yawPitchRoll();
                        scenefile::Transform t;
                        t.type = scenefile::Transform::move;
                        t.x = ypr.position.x();
                        t.y = ypr.position.y();
                        t.z = ypr.position.z();
                        cam.transforms.push_back(t);

                        t.type = scenefile::Transform::yaw;
                        t.angle = ypr.yaw;
                        cam.transforms.push_back(t);

                        t.type = scenefile::Transform::pitch;
                        t.angle = ypr.pitch;
                        cam.transforms.push_back(t);

                        t.type = scenefile::Transform::roll;
                        t.angle = ypr.roll;
                        cam.transforms.push_back(t);
                } break;
        case cosyscene::Navigation::None:
                break;
        }
        scene.addCamera(cam);

        // Render Settings
        scenefile::RenderSettings rs;
        rs.width = 320;
        rs.height = 240;
        rs.samplesPerPixel = 1;

        rs.surfaceIntegrator.type = scenefile::SurfaceIntegrator::whitted;
        rs.volumeIntegrator.type = scenefile::VolumeIntegrator::none;

        scene.addRenderSettings(rs);

        // Film settings.
        scenefile::FilmSettings fs;
        fs.colorscale = 0.00005;
        fs.convertToSrgb = false;
        scene.setFilmSettings(fs);

        // Backgrounds.
        {
                scenefile::Background bg;

                SunSky const & ss = *sunSky_;
                switch (ss.kind()) {
                case SunSky::UtahSky: {
                        bg.type = scenefile::Background::pss_sunsky;
                        const UtahSky us = ss.utahSky();
                        bg.atmosphereBrightnessFactor = us.atmosphereBrightnessFactor;
                        bg.atmosphericEffects = us.atmosphericEffects;
                        bg.atmosphericFxFactor = us.atmosphericFxFactor;
                        bg.overcast = us.overcast;
                        bg.sunBrightnessFactor = us.sunBrightnessFactor;
                        bg.sunDirection = scenefile::Normal(us.sunDirection.x(),
                                                            us.sunDirection.y(),
                                                            us.sunDirection.z());
                        bg.sunSizeFactor = us.sunSizeFactor;
                        bg.turbidity = us.turbidity;
                        break;
                }
                case SunSky::None:
                        bg.type = scenefile::Background::pss_sunsky;
                        break;
                }

                scene.addBackground(bg);
        }

        // Terrain.
        {
                scenefile::Object ob;

                TerrainFormation const &t = *terrain_->formation();
                if (TerrainFormation::None != t.kind()) {
                        ob.type = scenefile::Object::lazy_quadtree;

                        switch (t.kind()) {
                        case TerrainFormation::QuatschSource:
                                ob.lazyQuadtreeParams.code = t.quatschSource().code();
                                break;
                        case TerrainFormation::None: /* never the case */ break;
                        }

                        ob.lazyQuadtreeParams.size = 10000;
                        scene.addObject(ob);
                }
        }


        return scenePtr;
}


} // namespace
