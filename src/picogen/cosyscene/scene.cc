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
#include "water.hh"
#include "sunsky.hh"
#include "navigation.hh"
#include "camera.hh"
#include "rendersettings.hh"
#include "filmsettings.hh"
#include "material.hh"

#include "redshift_file/jobfile.hh"
#include "quatsch-preprocessor/parsing.hh"

namespace picogen { namespace cosyscene {

Scene::Scene()
: terrain_(new Terrain())
, water_(new Water())
, sunSky_(new SunSky())
, navigation_(new Navigation())
, camera_(new Camera())
, renderSettings_(new TwinRenderSettings())
, filmSettings_(new FilmSettings())
{
        navigation_->toYawPitchRoll(cosyscene::YawPitchRoll());
        sunSky_->toUtahSky(cosyscene::UtahSky());
}




redshift_file::Material toRedshift (cosyscene::Material const &material) {
        redshift_file::Material ret;

        switch (material.kind()) {
        case Material::None:
                ret.color = redshift_file::Color(1,1,1);
                break;
        case Material::Monochrome: {
                        cosyscene::Color mono = material.monochrome();
                        redshift_file::Color redcol;


                        switch (mono.kind()) {
                        case cosyscene::Color::Rgb:
                                redcol = redshift_file::Color(mono.rgb().r().toDouble(),
                                                              mono.rgb().g().toDouble(),
                                                              mono.rgb().b().toDouble());
                                break;
                        case cosyscene::Color::Spectrum:
                                redcol.type = redshift_file::Color::Spectrum;
                                redshift_file::Spectrum &redspec = redcol.spectrum;
                                cosyscene::Spectrum cospec = mono.spectrum();

                                for (size_t i=0; i<cospec.size(); ++i) {
                                        redshift_file::WavelengthAmplitudePair wa;
                                        wa.wavelength = cospec[i].wavelength().toDouble();
                                        wa.amplitude = cospec[i].amplitude().toDouble();

                                        redspec.samples.push_back(wa);
                                }
                                break;
                        }

                        ret.color = redcol;
                } break;
        }

        return ret;
}



std::string Scene::finalTerrainCode() const {
        const TerrainFormation &formation = *terrain_->formation();

        switch (formation.kind()) {
        case TerrainFormation::QuatschSource:
                return formation
                       .quatschSource()
                       .code();
                break;
        case TerrainFormation::None: /* never the case */
                break;
        case TerrainFormation::QuatschPreset:
                return formation
                       .quatschPreset()
                       .getPreprocessedCode();
                break;
        }

        return "0";
}



redshift::shared_ptr<redshift_file::Scene> Scene::toRedshiftScene(
        bool usePreviewSettings
) const {
        using namespace redshift;
        shared_ptr<redshift_file::Scene> scenePtr(new redshift_file::Scene);
        redshift_file::Scene &scene = *scenePtr;


        TwinRenderSettings const &twinRenderSettings = *renderSettings_;
        cosyscene::RenderSettings const &renderSettings = usePreviewSettings ?
                                               *twinRenderSettings.preview() :
                                               *twinRenderSettings.production();
        cosyscene::Camera const &camera = *camera_;

        // Camera
        const cosyscene::Navigation &nav = *navigation_;

        redshift_file::Camera cam;

        switch (camera.kind()) {
        case cosyscene::Camera::pinhole:
                cam.type = redshift_file::Camera::pinhole;
                cam.pinholeParams.front = camera.pinholeCamera()
                                          .frontPlaneDistance().toDouble();
                break;
        case cosyscene::Camera::cylindrical:
                cam.type = redshift_file::Camera::cylindrical;
                cam.cylindricalParams.front = camera.cylindricalCamera()
                                              .frontPlaneDistance().toDouble();
                break;
        case cosyscene::Camera::cubemap_face:
                switch (camera.cubemapFaceCamera().face()) {
                case cosyscene::CubemapFaceCamera::front:
                        cam.type = redshift_file::Camera::cubemap_front;
                        break;
                case cosyscene::CubemapFaceCamera::back:
                        cam.type = redshift_file::Camera::cubemap_back;
                        break;
                case cosyscene::CubemapFaceCamera::left:
                        cam.type = redshift_file::Camera::cubemap_left;
                        break;
                case cosyscene::CubemapFaceCamera::right:
                        cam.type = redshift_file::Camera::cubemap_right;
                        break;
                case cosyscene::CubemapFaceCamera::bottom:
                        cam.type = redshift_file::Camera::cubemap_bottom;
                        break;
                case cosyscene::CubemapFaceCamera::top:
                        cam.type = redshift_file::Camera::cubemap_top;
                        break;
                }
        }

        switch (nav.kind()) {
        case cosyscene::Navigation::YawPitchRoll: {
                        cosyscene::YawPitchRoll ypr = nav.yawPitchRoll();
                        redshift_file::Transform t;
                        t.type = redshift_file::Transform::move;
                        t.x = ypr.position.x().toDouble();
                        t.y = ypr.position.y().toDouble();
                        t.z = ypr.position.z().toDouble();
                        cam.transforms.push_back(t);

                        t.type = redshift_file::Transform::yaw;
                        t.angle = ypr.yaw.toDouble() * (180/redshift::constants::pi);

                        cam.transforms.push_back(t);

                        // the negation has historical reasons within redshift
                        t.type = redshift_file::Transform::pitch;
                        t.angle = -ypr.pitch.toDouble() * (180/redshift::constants::pi);
                        cam.transforms.push_back(t);

                        // the negation has historical reasons within redshift
                        t.type = redshift_file::Transform::roll;
                        t.angle = -ypr.roll.toDouble() * (180/redshift::constants::pi);
                        cam.transforms.push_back(t);
                } break;
        case cosyscene::Navigation::None:
                break;
        }
        scene.addCamera(cam);

        // Render Settings
        redshift_file::RenderSettings rs;
        rs.width = renderSettings.width();
        rs.height = renderSettings.height();
        rs.samplesPerPixel = renderSettings.samplesPerPixel();
        rs.userSeed = renderSettings.randomSeed();

        rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::whitted;

        switch (renderSettings.surfaceIntegrator().kind()) {
        case SurfaceIntegrator::none:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::none;
                break;
        case SurfaceIntegrator::whitted:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::whitted;
                break;
        case SurfaceIntegrator::whitted_ambient:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::whitted_ambient;
                rs.surfaceIntegrator.numAmbientSamples = renderSettings.
                                                         surfaceIntegrator().
                                                         whittedAmbientIntegrator().
                                                         numAmbientSamples();
                break;
        case SurfaceIntegrator::path:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::path;
                break;
        case SurfaceIntegrator::debug_distance:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::debug_distance;
                break;
        case SurfaceIntegrator::debug_normals:
                rs.surfaceIntegrator.type = redshift_file::SurfaceIntegrator::debug_normals;
                break;
        }

        //rs.surfaceIntegrator.numAmbientSamples = renderSettings
        rs.volumeIntegrator.type = redshift_file::VolumeIntegrator::none;

        scene.addRenderSettings(rs);

        // Film settings.
        redshift_file::FilmSettings fs;
        fs.colorscale = filmSettings_->brightnessFactor().toDouble();
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
                        bg.atmosphereBrightnessFactor = us.atmosphereBrightnessFactor.toDouble();
                        bg.atmosphericEffects = us.atmosphericEffects;
                        bg.atmosphericFxFactor = us.atmosphericFxFactor.toDouble();
                        bg.overcast = us.overcast.toDouble();
                        bg.sunBrightnessFactor = us.sunBrightnessFactor.toDouble();
                        bg.sunDirection = redshift_file::Normal(
                                                            us.sunDirection.x().toDouble(),
                                                            us.sunDirection.y().toDouble(),
                                                            us.sunDirection.z().toDouble());
                        bg.sunSizeFactor = us.sunSizeFactor.toDouble();
                        bg.turbidity = us.turbidity.toDouble();
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
                Material const &material = *terrain_->material();
                TerrainFitting const &fitting = *terrain_->fitting();

                if (TerrainFormation::None != formation.kind()) {
                        ob.type = redshift_file::Object::lazy_quadtree;

                        // Formation
                        ob.lazyQuadtreeParams.code = finalTerrainCode();

                        // Fitting
                        ob.lazyQuadtreeParams.size = fitting.lazyQuadtreeVisibleExtent().toDouble();
                        ob.lazyQuadtreeParams.maxRecursion = fitting.lazyQuadtreeMaxRecursion();
                        ob.lazyQuadtreeParams.lodFactor = fitting.detailCoefficient().toDouble();

                        if (renderSettings.maxLazyQuadtreeDepth() != 0) {
                                ob.lazyQuadtreeParams.maxRecursion =
                                        renderSettings.maxLazyQuadtreeDepth();
                        }


                        ob.lazyQuadtreeParams.material = toRedshift(material);


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



        // Water.
        {
                redshift_file::Object ob;

                const TerrainFormation &formation = *water_->formation();
                const Material         &material  = *water_->material();
                const WaterFitting     &fitting   = *water_->fitting();

                if (TerrainFormation::None != formation.kind()) {
                        ob.type = redshift_file::Object::water_plane;

                        // Formation
                        switch (formation.kind()) {
                        case TerrainFormation::QuatschSource:
                                ob.waterPlaneParams.code = formation
                                                             .quatschSource()
                                                             .code();
                                break;
                        case TerrainFormation::None: /* never the case */
                                break;
                        case TerrainFormation::QuatschPreset:
                                ob.waterPlaneParams.code =
                                                formation
                                                .quatschPreset()
                                                .getPreprocessedCode();
                                break;
                        }

                        // Fitting
                        ob.waterPlaneParams.height = fitting.seaLevel().toDouble();
                        ob.waterPlaneParams.material = toRedshift(material);
                        scene.addObject(ob);
                }
        }


        return scenePtr;
}


} }
