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
#include "rendersettings.hh"

#include "redshift/include/jobfile.hh"
#include "quatsch-preprocessor/parsing.hh"

namespace cosyscene {

Scene::Scene()
: terrain_(new Terrain())
, sunSky_(new SunSky())
, navigation_(new Navigation())
, renderSettings_(new TwinRenderSettings())
{
        navigation_->toYawPitchRoll(cosyscene::YawPitchRoll());
        sunSky_->toUtahSky(cosyscene::UtahSky());
}



redshift::shared_ptr<redshift_file::Scene> Scene::toRedshiftScene() const {
        using namespace ::redshift;
        shared_ptr<redshift_file::Scene> scenePtr(new redshift_file::Scene);
        redshift_file::Scene &scene = *scenePtr;

        // Camera
        const cosyscene::Navigation &nav = *navigation_;

        redshift_file::Camera cam;
        cam.type = redshift_file::Camera::pinhole;
        cam.pinholeParams.front = 1;

        switch (nav.kind()) {
        case cosyscene::Navigation::YawPitchRoll: {
                        cosyscene::YawPitchRoll ypr = nav.yawPitchRoll();
                        redshift_file::Transform t;
                        t.type = redshift_file::Transform::move;
                        t.x = ypr.position.x();
                        t.y = ypr.position.y();
                        t.z = ypr.position.z();
                        cam.transforms.push_back(t);

                        t.type = redshift_file::Transform::yaw;
                        t.angle = ypr.yaw;
                        cam.transforms.push_back(t);

                        t.type = redshift_file::Transform::pitch;
                        t.angle = ypr.pitch;
                        cam.transforms.push_back(t);

                        t.type = redshift_file::Transform::roll;
                        t.angle = ypr.roll;
                        cam.transforms.push_back(t);
                } break;
        case cosyscene::Navigation::None:
                break;
        }
        scene.addCamera(cam);

        // Render Settings
        redshift_file::RenderSettings rs;
        rs.width = 320;
        rs.height = 240;
        rs.samplesPerPixel = 1;

        rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::whitted;
        rs.volumeIntegrator.type = redshift_file::VolumeIntegrator::none;

        scene.addRenderSettings(rs);

        // Film settings.
        redshift_file::FilmSettings fs;
        fs.colorscale = 0.00005;
        fs.convertToSrgb = false;
        scene.setFilmSettings(fs);

        // Backgrounds.
        {
                redshift_file::Background bg;

                SunSky const & ss = *sunSky_;
                switch (ss.kind()) {
                case SunSky::UtahSky: {
                        bg.type = redshift_file::Background::pss_sunsky;
                        const UtahSky us = ss.utahSky();
                        bg.atmosphereBrightnessFactor = us.atmosphereBrightnessFactor;
                        bg.atmosphericEffects = us.atmosphericEffects;
                        bg.atmosphericFxFactor = us.atmosphericFxFactor;
                        bg.overcast = us.overcast;
                        bg.sunBrightnessFactor = us.sunBrightnessFactor;
                        bg.sunDirection = redshift_file::Normal(
                                                            us.sunDirection.x(),
                                                            us.sunDirection.y(),
                                                            us.sunDirection.z());
                        bg.sunSizeFactor = us.sunSizeFactor;
                        bg.turbidity = us.turbidity;
                        break;
                }
                case SunSky::None:
                        bg.type = redshift_file::Background::pss_sunsky;
                        break;
                }

                scene.addBackground(bg);
        }

        // Terrain.
        {
                redshift_file::Object ob;

                TerrainFormation const &formation = *terrain_->formation();
                TerrainMaterial const &material = *terrain_->material();
                if (TerrainFormation::None != formation.kind()) {
                        ob.type = redshift_file::Object::lazy_quadtree;

                        // Formation
                        switch (formation.kind()) {
                        case TerrainFormation::QuatschSource:
                                ob.lazyQuadtreeParams.code = formation
                                                             .quatschSource()
                                                             .code();
                                break;
                        case TerrainFormation::None: /* never the case */
                                break;
                        case TerrainFormation::QuatschPreset:
                                ob.lazyQuadtreeParams.code =
                                                formation
                                                .quatschPreset()
                                                .getPreprocessedCode();
                                break;
                        }

                        // Fitting
                        ob.lazyQuadtreeParams.size = 10000;

                        // Material
                        switch (material.kind()) {
                        case TerrainMaterial::None:
                                ob.lazyQuadtreeParams.material.color = redshift_file::Color(1,1,1);
                                break;
                        case TerrainMaterial::Monochrome: {
                                        cosyscene::Color mono = material.monochrome();
                                        redshift_file::Color redcol;


                                        switch (mono.kind()) {
                                        case cosyscene::Color::Rgb:
                                                redcol = redshift_file::Color(mono.rgb().r(),
                                                                              mono.rgb().g(),
                                                                              mono.rgb().b());
                                                break;
                                        case cosyscene::Color::Spectrum:
                                                redcol.type = redshift_file::Color::Spectrum;
                                                redshift_file::Spectrum &redspec = redcol.spectrum;
                                                cosyscene::Spectrum cospec = mono.spectrum();

                                                for (size_t i=0; i<cospec.size(); ++i) {
                                                        redshift_file::WavelengthAmplitudePair wa;
                                                        wa.wavelength = cospec[i].wavelength();
                                                        wa.amplitude = cospec[i].amplitude();

                                                        redspec.samples.push_back(wa);
                                                }
                                                break;
                                        }

                                        ob.lazyQuadtreeParams.material.color = redcol;
                                } break;
                        }

                        redshift_file::Object inst;
                        inst.type = redshift_file::Object::instance;
                        inst.instanceParams.objects.push_back(ob);

                        redshift_file::Transform scale;
                        scale.type = redshift_file::Transform::scale;
                        scale.x = 1;
                        scale.y = 1;
                        scale.z = 1;
                        inst.instanceParams.transforms.push_back(scale);
                        scene.addObject(inst);
                }
        }

        return scenePtr;
}


} // namespace
