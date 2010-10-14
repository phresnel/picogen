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

#ifndef JOBFILE_HH_INCLUDED_20100318
#define JOBFILE_HH_INCLUDED_20100318



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
#include "primitives/bvh.hh"
#include "primitives/trianglebvh.hh"
#include "primitives/triangle.hh"
#include "primitives/lsystemtree.hh"
#include "primitives/instance.hh"
#include "primitives/boundinstance.hh"
#include "primitives/forest.hh"

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
#include "bxdf/lambertian.hh"
#include "material/matte.hh"

// texture/
#include "texture/constant.hh"

//}}}}}}}}}




//#include "../include/rendertargets/sdlrendertarget.hh"
//#include "../include/rendertargets/colorrendertarget.hh"
#include "cameras/cylindrical.hh"
#include "cameras/pinhole.hh"
#include "cameras/cubemapface.hh"
//#include "interaction/sdlcommandprocessor.hh"
//#include "../include/interaction/rendertarget-copying-reporter.hh"
#include "basictypes/height-function.hh"
#include "basictypes/quatsch-height-function.hh"

#include "backgrounds/pss-adapter.hh"
#include "backgrounds/preetham-shirley-smits/sunsky.hh"

#include "../../actuarius/actuarius.hh"
#include <cmath>


#include <iostream>
#include <cstdio>
namespace redshift_file {
        using redshift::shared_ptr;
        using redshift::real_t;
}

#include "redshift_file/color.hh"
#include "redshift_file/transform.hh"
#include "redshift_file/material.hh"
#include "redshift_file/normal.hh"
#include "redshift_file/point.hh"
#include "redshift_file/vertex.hh"
#include "redshift_file/object.hh"

#include "redshift_file/surfaceintegrator.hh"

#include "redshift_file/volume.hh"
#include "redshift_file/volumeintegrator.hh"

#include "redshift_file/background.hh"

#include "redshift_file/rendersettings.hh"
#include "redshift_file/filmsettings.hh"

#include "redshift_file/camera.hh"

#include "redshift_file/scene.hh"


redshift::shared_ptr<redshift::Scene>
 sceneDescriptionToScene (
        redshift_file::Scene const &scene,
        redshift::shared_ptr<redshift::Film> renderBuffer,
        int renderSettingsIndex, int cameraIndex
);

#endif // JOBFILE_HH_INCLUDED_20100318
