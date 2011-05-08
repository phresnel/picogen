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

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO: check if boost reports on cerr or cout
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "ignore_strict_aliasing" // because of boost::optional

#include "redshift/include/auxiliary/currentdate.hh"
#include "redshift/include/auxiliary/remove_filename_extension.hh"
#include "redshift/include/static_init.hh"

#include "redshift/include/image-export.hh"

#include "redshift/include/meta/compiler.hh"

//{{{{{{{{{
// Color types.
#include "redshift/include/basictypes/rgb.hh"


// TODO reorder

// coordinates/
#include "redshift/include/coordinates/uvcoordinates.hh"
#include "redshift/include/coordinates/imagecoordinates.hh"
#include "redshift/include/coordinates/lenscoordinates.hh"
#include "redshift/include/coordinates/texture2dcoordinates.hh"

// interaction/
#include "redshift/include/interaction/usercommandprocessor.hh"
#include "redshift/include/interaction/passivecommandprocessor.hh"
#include "redshift/include/interaction/progressreporter.hh"

// basictypes/
namespace redshift{class RenderTarget;}
#include "redshift/include/basictypes/rectangle.hh"
#include "redshift/include/basictypes/sample.hh"

// cameras/
#include "redshift/include/cameras/camera.hh"

// basictypes/
#include "redshift/include/basictypes/differentialgeometry.hh"
#include "redshift/include/basictypes/material.hh"
#include "redshift/include/basictypes/intersection.hh"
#include "redshift/include/basictypes/background.hh"
#include "redshift/include/basictypes/volume.hh"
#include "redshift/include/basictypes/film.hh"

// rendertargets/
#include "redshift/include/rendertargets/rendertargetlock.hh"
#include "redshift/include/rendertargets/rendertarget.hh"



// shapes/
#include "redshift/include/shapes/shape.hh"
#include "redshift/include/shapes/closedsphere.hh"

// primitive/
#include "redshift/include/primitives/primitive.hh"
//#include "primitives/heightmap.hh"
//#include "primitives/booleanfield.hh"
#include "redshift/include/primitives/closedsphere.hh"
#include "redshift/include/primitives/lazyquadtree.hh"
#include "redshift/include/primitives/horizonplane.hh"
#include "redshift/include/primitives/waterplane.hh"
#include "redshift/include/primitives/list.hh"

// background/
//#include "backgrounds/visualise-direction.hh"
//#include "backgrounds/monochrome.hh"
//#include "backgrounds/preetham-adapter.hh"
#include "redshift/include/backgrounds/pss-adapter.hh"

// Cameras.
#include "redshift/include/cameras/camera.hh"

// basictypes/
#include "redshift/include/basictypes/transport.hh"
#include "redshift/include/basictypes/scene.hh"
//#include "basictypes/heightmap.hh"
#include "redshift/include/basictypes/bsdf.hh"

// integrators/
#include "redshift/include/integrators/visualize-distance.hh"
#include "redshift/include/integrators/show-surface-normals.hh"
#include "redshift/include/integrators/redshift.hh"
#include "redshift/include/integrators/whitted.hh"
#include "redshift/include/integrators/emission.hh"
#include "redshift/include/integrators/single-scattering.hh"
#include "redshift/include/integrators/null.hh"
#include "redshift/include/integrators/path.hh"

// volume/
#include "redshift/include/volume/homogeneous.hh"
#include "redshift/include/volume/exponential.hh"
#include "redshift/include/volume/list.hh"


// bxdf/
#include "bxdf/lambertian.hh"

//}}}}}}}}}

#include "redshift/include/rendertargets/sdlrendertarget.hh"
#include "redshift/include/rendertargets/colorrendertarget.hh"
#include "redshift/include/cameras/pinhole.hh"
#include "redshift/include/interaction/sdlcommandprocessor.hh"
#include "redshift/include/interaction/rendertarget-copying-reporter.hh"
#include "redshift/include/interaction/film-to-rendertarget-copying-reporter.hh"
#include "redshift/include/basictypes/height-function.hh"
#include "redshift/include/basictypes/quatsch-height-function.hh"
#include "actuarius/actuarius.hh"
#include "redshift/include/jobfile.hh"

#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/program_options.hpp>

#include "redshift/include/auxiliary/filename_extension.hh"

#include "redshift/include/redshift_file/to_redshift/to_redshift.hh"


redshift::shared_ptr<redshift::Scene>
 sceneDescriptionToScene (
        redshift_file::Scene const &scene,
        redshift::shared_ptr<redshift::Film> film,
        int renderSettingsIndex, int cameraIndex
) {
        return redshift_file::toRedshift (scene, film,
                                          renderSettingsIndex, cameraIndex);
}

namespace redshift_file {

redshift::shared_ptr<redshift::Scene>
 toRedshift (
        redshift_file::Scene const &scene,
        redshift::shared_ptr<redshift::Film> film,
        int renderSettingsIndex, int cameraIndex
) {
        using namespace redshift;
        using namespace redshift::camera;
        using namespace redshift::interaction;
        using namespace redshift::primitive;


        shared_ptr<redshift::Camera> camera;
        if (scene.cameraCount()) {
                camera = toRedshift (scene.camera(cameraIndex),
                                     film->width(),
                                     film->height());
        } else {
                camera = shared_ptr<redshift::Camera> (
                    new redshift::camera::Pinhole(
                        film->width(),
                        film->height(),
                        1.0f,
                        redshift::Transform::translation(0.f,0.1f,0.f)
                ));
        }


        // Add objects.
        redshift::primitive::List *list = new List;
        for (unsigned int i=0; i<scene.objectCount(); ++i) {
                list->add (toRedshift (scene.object(i)));
        }
        shared_ptr<redshift::Primitive> agg (list);


        // Add volumes.
        volume::List *volumeList = new volume::List;
        for (unsigned int i=0; i<scene.volumeCount(); ++i) {
                volumeList->add (toRedshift(scene.volume(i)));
        }
        shared_ptr<redshift::VolumeRegion> volumeAgg (volumeList);



        // TODO: support arbitrary many backgrounds (Starsky!)
        shared_ptr<redshift::Sky> sky;
        if (scene.backgroundCount()) {
                sky = toRedshift (scene.background(0));
        } else {

                /*shared_ptr<background::PssSunSky> pss (new background::PssSunSky(
                        30, // [in] lat Latitude (0-360)
                        30,			// [in] long Longitude (-90,90) south to north
                        0,			// [in] sm  Standard Meridian
                        90,			// [in] jd  Julian Day (1-365)
                        8.0,			// [in] tod Time Of Day (0.0,23.99) 14.25 = 2:15PM
                        7,			// [in] turb  Turbidity (1.0,30+) 2-6 are most useful for clear days.
                        true			// [in] initAtmEffects  if atm effects are not initialized, bad things will
                                                // happen if you try to use them....
                ));*/
                shared_ptr<background::PssSunSky> pss (new background::PssSunSky(
                        Vector(1,.25,1),
                        7,
                        0,
                        true
                ));
                sky = shared_ptr<redshift::Sky> (
                        new backgrounds::PssAdapter (pss,1,1,1,1));
        }
        // ----------

        return redshift::shared_ptr<redshift::Scene> (new redshift::Scene (
                film,
                camera,
                agg,
                sky,

                redshift::shared_ptr<redshift::Integrator>(
                        toRedshift(scene.renderSettings(renderSettingsIndex)
                                   .surfaceIntegrator)),

                volumeAgg,
                redshift::shared_ptr<redshift::VolumeIntegrator>(
                        toRedshift(scene.renderSettings(renderSettingsIndex)
                                   .volumeIntegrator))
        ));
}


redshift::shared_ptr<redshift::Sky> toRedshift (Background const &bg) {
        using namespace redshift;
        switch (bg.type) {
        case redshift_file::Background::pss_sunsky: {
        #if 1
                shared_ptr<redshift::background::PssSunSky> preetham (
                 new background::PssSunSky(
                        normalize(Vector(bg.sunDirection.x,bg.sunDirection.y,bg.sunDirection.z)),
                        bg.turbidity,
                        bg.overcast,
                        bg.atmosphericEffects
                ));
                return redshift::shared_ptr<redshift::Sky> (
                  new backgrounds::PssAdapter (
                        preetham,
                        bg.sunSizeFactor, bg.sunBrightnessFactor,
                        bg.atmosphereBrightnessFactor, bg.atmosphericFxFactor
                ));
        #else
                shared_ptr<redshift::background::Preetham> preetham (
                 new redshift::background::Preetham());

                preetham->setSunDirection(Vector(sunDirection.x,sunDirection.y,sunDirection.z));
                preetham->setTurbidity(turbidity);
                preetham->setSunColor(Color::FromRGB(sunColor.r,sunColor.g,sunColor.b));
                preetham->setColorFilter(Color::FromRGB(skyFilter.r,skyFilter.g,skyFilter.b));
                preetham->enableFogHack (false, 0.00025f, 150000);
                preetham->invalidate();
                return shared_ptr<redshift::Background> (
                        new backgrounds::PreethamAdapter (preetham)
                );
        #endif
        } break;
        };
        return shared_ptr<redshift::Sky>();
}



redshift::shared_ptr<redshift::Camera> toRedshift (Camera const & camera,
                                                   unsigned int width,
                                                   unsigned int height)
{
        switch (camera.type) {
        case Camera::pinhole:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::Pinhole(
                        width, height, camera.pinholeParams.front,
                        toRedshift(camera.transforms)));

        case Camera::cylindrical:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::Cylindrical(
                        width, height, camera.cylindricalParams.front,
                        toRedshift(camera.transforms)));

        case Camera::cubemap_left:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::left,
                        toRedshift(camera.transforms)));
        case Camera::cubemap_right:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::right,
                        toRedshift(camera.transforms)));
        case Camera::cubemap_bottom:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::bottom,
                        toRedshift(camera.transforms)));
        case Camera::cubemap_top:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::top,
                        toRedshift(camera.transforms)));
        case Camera::cubemap_front:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::front,
                        toRedshift(camera.transforms)));
        case Camera::cubemap_back:
                return redshift::shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                        width, height, redshift::camera::CubeMapFace::back,
                        toRedshift(camera.transforms)));
        default:
                throw std::runtime_error("only  pinhole supported");
        };
}





redshift::Color toRedshift (Spectrum const &spectrum) {
        typedef redshift::Color::real_t real_t;
        typedef std::vector<WavelengthAmplitudePair>::const_iterator iterator;

        std::vector<real_t> wavelengths;
        std::vector<real_t> amplitudes;

        for (iterator it = spectrum.samples.begin();
             it != spectrum.samples.end();
             ++it
        ) {
                wavelengths.push_back(it->wavelength);
                amplitudes.push_back(it->amplitude);
        }

        return redshift::Color::FromSampled (
                        &amplitudes[0],
                        &wavelengths[0],
                        spectrum.samples.size());
}

redshift::Color toRedshift (Rgb const &rgb, redshift::SpectrumKind kind) {
        return redshift::Color::FromRGB(rgb.r, rgb.g, rgb.b, kind);
}

redshift::Color toRedshift (Color const &color, redshift::SpectrumKind kind) {
        switch (color.type) {
        case Color::RGB:      return toRedshift (color.rgb, kind);
        case Color::Spectrum: return toRedshift (color.spectrum);
        }
        throw std::runtime_error("unknown color type in "
                "toRedshift (Color const &color, redshift::SpectrumKind kind)");
}



redshift::Point toRedshift (Point const &point) {
        return redshift::Point(point.x, point.y, point.z);
}
redshift::Normal toRedshift (Normal const &normal) {
        return redshift::Normal(normal.x, normal.y, normal.z);
}


redshift::shared_ptr<redshift::Integrator> toRedshift(SurfaceIntegrator const &si) {
        typedef shared_ptr<redshift::Integrator> rett;
        switch (si.type) {
        case SurfaceIntegrator::whitted_ambient:
                return rett(new redshift::RedshiftIntegrator(si.numAmbientSamples));
        case SurfaceIntegrator::whitted:
                return rett(new redshift::WhittedIntegrator());
        case SurfaceIntegrator::path:
                return rett(new redshift::PathIntegrator());
        case SurfaceIntegrator::none:
                return rett(new redshift::NullIntegrator());
        case SurfaceIntegrator::debug_distance:
                return rett(new redshift::VisualizeDistance());
        case SurfaceIntegrator::debug_normals:
                return rett(new redshift::ShowSurfaceNormals());
        };
        return shared_ptr<redshift::Integrator>();
}



redshift::Transform toRedshift (Transform const &transform)  {
        typedef redshift::Transform RedT;
        const double to_radians = redshift::constants::pi/180;
        switch (transform.type) {
        case Transform::move: return RedT::translation(transform.x,transform.y,transform.z);
        case Transform::move_left:
        case Transform::move_right: return RedT::translation(transform.x,0,0);
        case Transform::move_up:
        case Transform::move_down: return RedT::translation(0,transform.y,0);
        case Transform::move_forward:
        case Transform::move_backward:  return RedT::translation(0,0,transform.z);

        case Transform::yaw:   return RedT::rotationY(transform.angle*to_radians);
        case Transform::pitch: return RedT::rotationX(transform.angle*to_radians);
        case Transform::roll:  return RedT::rotationZ(transform.angle*to_radians);

        case Transform::scale:  return RedT::scaling(transform.x, transform.y, transform.z);
        };
        return RedT();
}
redshift::Transform toRedshift (TransformList const &transformList) {
        redshift::Transform ret;
        for (TransformList::const_iterator it = transformList.begin();
             it!=transformList.end();
             ++it
        ) {
                ret = ret * toRedshift(*it);
        }
        return ret;
}



redshift::shared_ptr<redshift::VolumeRegion> toRedshift (Volume const &volume) {
        switch (volume.type) {
        case Volume::homogeneous:
                return shared_ptr<redshift::VolumeRegion> (
                   new redshift::volume::Homogeneous(
                        toRedshift (volume.sigma_a, redshift::IlluminantSpectrum),
                        toRedshift (volume.sigma_s, redshift::IlluminantSpectrum),
                        toRedshift (volume.Lve, redshift::IlluminantSpectrum),
                        volume.hg
                ));
        case Volume::exponential:
                return shared_ptr<redshift::VolumeRegion> (
                   new redshift::volume::Exponential(
                        toRedshift (volume.sigma_a, redshift::IlluminantSpectrum),
                        toRedshift (volume.sigma_s, redshift::IlluminantSpectrum),
                        toRedshift (volume.Lve, redshift::IlluminantSpectrum),
                        volume.hg,
                        volume.baseFactor,
                        volume.exponentFactor,
                        toRedshift (volume.min),
                        redshift::vector_cast<redshift::Vector>(toRedshift (volume.up)),
                        volume.epsilon
                ));
        };
        return shared_ptr<redshift::VolumeRegion>();
}

redshift::shared_ptr<redshift::VolumeIntegrator> toRedshift(VolumeIntegrator const &integrator) {
        switch (integrator.type) {
        case VolumeIntegrator::emission:
                return shared_ptr<redshift::VolumeIntegrator>(
                        new redshift::Emission(integrator.stepSize,
                                               integrator.cutoffDistance));
        case VolumeIntegrator::single:
                return shared_ptr<redshift::VolumeIntegrator>(
                    new redshift::SingleScattering(integrator.stepSize,
                                                   integrator.cutoffDistance));
        case VolumeIntegrator::none:
                return shared_ptr<redshift::VolumeIntegrator>(
                        new redshift::NullIntegrator()
                );
        };
        return shared_ptr<redshift::VolumeIntegrator>();
}

}