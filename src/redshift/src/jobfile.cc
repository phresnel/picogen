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
#include "basictypes/heightmap.hh"
#include "basictypes/bsdf.hh"

// integrators/
#include "integrators/visualize-distance.hh"
#include "integrators/show-surface-normals.hh"
#include "integrators/direct-lighting.hh"
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

#include "../include/jobfile.hh"

namespace redshift { namespace scenefile {

        const actuarius::Enum<Color::Type> Color::Typenames =
                ( actuarius::Nvp<Color::Type>(Color::RGB, "rgb")
                | actuarius::Nvp<Color::Type>(Color::Spectrum, "spectrum")
                );

        const actuarius::Enum<Object::Type> Object::Typenames =
                ( actuarius::Nvp<Object::Type>(Object::water_plane, "water-plane")
                | actuarius::Nvp<Object::Type>(Object::lazy_quadtree, "lazy-quadtree")
                | actuarius::Nvp<Object::Type>(Object::horizon_plane, "horizon-plane")
        );

        const actuarius::Enum<SurfaceIntegrator::Type> SurfaceIntegrator::Typenames =
                ( actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::none, "none")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::whitted_ambient, "whitted_ambient")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::whitted, "whitted")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::path, "path")
                );

        const actuarius::Enum<Volume::Type> Volume::Typenames =
                ( actuarius::Nvp<Volume::Type>(Volume::homogeneous, "homogeneous")
                | actuarius::Nvp<Volume::Type>(Volume::exponential, "exponential")
                );

        const actuarius::Enum<VolumeIntegrator::Type> VolumeIntegrator::Typenames =
                ( actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::none, "none")
                | actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::emission, "emission")
                | actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::single, "single")
                );

        const actuarius::Enum<Background::Type> Background::Typenames =
                ( actuarius::Nvp<Background::Type>(Background::pss_sunsky, "pss-sunsky")
                );

        const actuarius::Enum<Transform::Type> Transform::Typenames =
                ( actuarius::Nvp<Transform::Type>(Transform::move, "move")
                | actuarius::Nvp<Transform::Type>(Transform::move_left, "move-left")
                | actuarius::Nvp<Transform::Type>(Transform::move_right, "move-right")
                | actuarius::Nvp<Transform::Type>(Transform::move_down, "move-down")
                | actuarius::Nvp<Transform::Type>(Transform::move_up, "move-up")
                | actuarius::Nvp<Transform::Type>(Transform::move_backward, "move-backward")
                | actuarius::Nvp<Transform::Type>(Transform::move_forward, "move-forward")
                | actuarius::Nvp<Transform::Type>(Transform::yaw, "yaw")
                | actuarius::Nvp<Transform::Type>(Transform::pitch, "pitch")
                | actuarius::Nvp<Transform::Type>(Transform::roll, "roll")
                );

        const actuarius::Enum<Camera::Type> Camera::Typenames =
                ( actuarius::Nvp<Camera::Type>(Camera::pinhole, "pinhole")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_left, "cubemap-left")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_right, "cubemap-right")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_bottom, "cubemap-bottom")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_top, "cubemap-top")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_front, "cubemap-front")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_back, "cubemap-back")
                );

} }
