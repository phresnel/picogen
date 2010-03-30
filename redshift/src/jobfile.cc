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
                ( actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::redshift, "redshift")
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

        const actuarius::Enum<Camera::Transform::Type> Camera::Transform::Typenames =
                ( actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move, "move")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_left, "move-left")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_right, "move-right")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_down, "move-down")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_up, "move-up")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_backward, "move-backward")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::move_forward, "move-forward")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::yaw, "yaw")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::pitch, "pitch")
                | actuarius::Nvp<Camera::Transform::Type>(Camera::Transform::roll, "roll")
                );


} }