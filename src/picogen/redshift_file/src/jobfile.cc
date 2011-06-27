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

#include "../include/jobfile.hh"

namespace picogen { namespace redshift_file {

        const actuarius::Enum<Color::Type> Color::Typenames =
                ( actuarius::Nvp<Color::Type>(Color::RGB, "rgb")
                | actuarius::Nvp<Color::Type>(Color::Spectrum, "spectrum")
                );

        const actuarius::Enum<Object::Type> Object::Typenames =
                ( actuarius::Nvp<Object::Type>(Object::water_plane, "water-plane")
                | actuarius::Nvp<Object::Type>(Object::lazy_quadtree, "lazy-quadtree")
                | actuarius::Nvp<Object::Type>(Object::horizon_plane, "horizon-plane")
                | actuarius::Nvp<Object::Type>(Object::closed_sphere, "sphere")
                | actuarius::Nvp<Object::Type>(Object::triangle, "triangle")
                | actuarius::Nvp<Object::Type>(Object::bvh, "bvh")
                | actuarius::Nvp<Object::Type>(Object::triangle_bvh, "triangle-bvh")
                | actuarius::Nvp<Object::Type>(Object::lsystemtree, "lsystem-tree")
                | actuarius::Nvp<Object::Type>(Object::instance, "instance")
                | actuarius::Nvp<Object::Type>(Object::forest, "forest")
        );

        const actuarius::Enum<SurfaceIntegrator::Type> SurfaceIntegrator::Typenames =
                ( actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::none, "none")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::whitted_ambient, "whitted_ambient")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::whitted, "whitted")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::path, "path")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::debug_distance, "debug-distance")
                | actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::debug_normals, "debug-normals")
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
                | actuarius::Nvp<Transform::Type>(Transform::scale, "scale")
                );

        const actuarius::Enum<Camera::Type> Camera::Typenames =
                ( actuarius::Nvp<Camera::Type>(Camera::pinhole, "pinhole")
                | actuarius::Nvp<Camera::Type>(Camera::cylindrical, "cylindrical")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_left, "cubemap-left")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_right, "cubemap-right")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_bottom, "cubemap-bottom")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_top, "cubemap-top")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_front, "cubemap-front")
                | actuarius::Nvp<Camera::Type>(Camera::cubemap_back, "cubemap-back")
                );

} }
