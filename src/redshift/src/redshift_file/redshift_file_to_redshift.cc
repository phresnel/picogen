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

#include "../include/auxiliary/currentdate.hh"
#include "../include/auxiliary/remove_filename_extension.hh"
#include "../include/static_init.hh"

#include "../include/image-export.hh"

#include "../include/meta/compiler.hh"

//{{{{{{{{{
// Color types.
#include "basictypes/rgb.hh"


// TODO reorder

// coordinates/
#include "coordinates/uvcoordinates.hh"
#include "coordinates/imagecoordinates.hh"
#include "coordinates/lenscoordinates.hh"
#include "coordinates/texture2dcoordinates.hh"

// interaction/
#include "interaction/usercommandprocessor.hh"
#include "interaction/passivecommandprocessor.hh"
#include "interaction/progressreporter.hh"

// basictypes/
namespace redshift{class RenderTarget;}
#include "basictypes/rectangle.hh"
#include "basictypes/sample.hh"

// cameras/
#include "cameras/camera.hh"

// basictypes/
#include "basictypes/differentialgeometry.hh"
#include "basictypes/material.hh"
#include "basictypes/intersection.hh"
#include "basictypes/background.hh"
#include "basictypes/volume.hh"
#include "basictypes/film.hh"

// rendertargets/
#include "rendertargets/rendertargetlock.hh"
#include "rendertargets/rendertarget.hh"



// shapes/
#include "shapes/shape.hh"
#include "shapes/closedsphere.hh"

// primitive/
#include "primitives/primitive.hh"
//#include "primitives/heightmap.hh"
//#include "primitives/booleanfield.hh"
#include "primitives/closedsphere.hh"
#include "primitives/lazyquadtree.hh"
#include "primitives/horizonplane.hh"
#include "primitives/waterplane.hh"
#include "primitives/list.hh"

// background/
//#include "backgrounds/visualise-direction.hh"
//#include "backgrounds/monochrome.hh"
//#include "backgrounds/preetham-adapter.hh"
#include "backgrounds/pss-adapter.hh"

// Cameras.
#include "cameras/camera.hh"

// basictypes/
#include "basictypes/transport.hh"
#include "basictypes/scene.hh"
//#include "basictypes/heightmap.hh"
#include "basictypes/bsdf.hh"

// integrators/
#include "integrators/visualize-distance.hh"
#include "integrators/show-surface-normals.hh"
#include "integrators/redshift.hh"
#include "integrators/whitted.hh"
#include "integrators/emission.hh"
#include "integrators/single-scattering.hh"
#include "integrators/null.hh"
#include "integrators/path.hh"

// volume/
#include "volume/homogeneous.hh"
#include "volume/exponential.hh"
#include "volume/list.hh"


// material/
#include "material/lambertian.hh"

//}}}}}}}}}

#include "../include/rendertargets/sdlrendertarget.hh"
#include "../include/rendertargets/colorrendertarget.hh"
#include "../include/cameras/pinhole.hh"
#include "../include/interaction/sdlcommandprocessor.hh"
#include "../include/interaction/rendertarget-copying-reporter.hh"
#include "../include/interaction/film-to-rendertarget-copying-reporter.hh"
#include "../include/basictypes/height-function.hh"
#include "../include/basictypes/quatsch-height-function.hh"
#include "../../actuarius/actuarius.hh"
#include "../include/jobfile.hh"

#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/program_options.hpp>

#include "../include/auxiliary/filename_extension.hh"



redshift::shared_ptr<redshift::Scene>
 sceneDescriptionToScene (
        redshift_file::Scene const &scene,
        redshift::shared_ptr<redshift::Film> film,
        int renderSettingsIndex, int cameraIndex
) {
        using namespace redshift;
        using namespace redshift::camera;
        using namespace redshift::interaction;
        using namespace redshift::primitive;


        shared_ptr<Camera> camera;
        if (scene.cameraCount()) {
                camera = scene.
                         camera(cameraIndex).
                         toRedshiftCamera (
                                film->width(),
                                film->height());
        } else {
                camera = shared_ptr<Camera> (new Pinhole(
                        film->width(),
                        film->height(),
                        1.0f,
                        Transform::translation(0.f,0.1f,0.f)
                ));
        }


        // Add objects.
        primitive::List *list = new List;
        for (unsigned int i=0; i<scene.objectCount(); ++i) {
                list->add (scene.object(i).toPrimitive());
        }
        shared_ptr<Primitive> agg (list);


        // Add volumes.
        volume::List *volumeList = new volume::List;
        for (unsigned int i=0; i<scene.volumeCount(); ++i) {
                volumeList->add (scene.volume(i).toVolume());
        }
        shared_ptr<VolumeRegion> volumeAgg (volumeList);



        // TODO: support arbitrary many backgrounds (Starsky!)
        shared_ptr<Sky> sky;
        if (scene.backgroundCount()) {
                sky = scene.background(0).toSky();
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

        return shared_ptr<Scene> (new Scene (
                film,
                camera,
                agg,
                sky,

                shared_ptr<Integrator>(
                        scene.renderSettings(renderSettingsIndex)
                                .surfaceIntegrator
                                .toSurfaceIntegrator()),

                volumeAgg,
                shared_ptr<VolumeIntegrator>(
                        scene.renderSettings(renderSettingsIndex)
                                .volumeIntegrator
                                .toVolumeIntegrator())
        ));
}



