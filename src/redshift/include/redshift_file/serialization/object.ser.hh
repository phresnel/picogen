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

#ifndef OBJECT_SER_HH_20101013
#define OBJECT_SER_HH_20101013

#include "transform.ser.hh"
#include "material.ser.hh"
#include "color.ser.hh"
#include "vertex.ser.hh"
#include "../object.hh"

namespace redshift_file {
        template<typename Arch>
        void Object::serialize (Arch &arch) {
                using actuarius::pack;

                switch (type) {
                case lazy_quadtree: {
                        /*const Rgb rgb = lazy_quadtree.color.toRgb();
                        r = rgb.r;
                        g = rgb.g;
                        b = rgb.b;*/

                        arch
                        & pack("code", lazyQuadtreeParams.code)
                        & pack("size", lazyQuadtreeParams.size)
                        & pack("max-recursion", lazyQuadtreeParams.maxRecursion)
                        & pack("lod-factor", lazyQuadtreeParams.lodFactor)
                        & pack("material", lazyQuadtreeParams.material)
                        ;
                } break;
                case water_plane:
                        arch
                        & actuarius::push_optional(true)
                        & pack("code", waterPlaneParams.code)
                        & pack("height", waterPlaneParams.height)
                        & pack("material", waterPlaneParams.material)
                        & actuarius::pop_optional
                        ;
                        break;
                case horizon_plane:
                        arch
                        & pack("height", horizonPlaneParams.height)
                        & pack("material", horizonPlaneParams.material)
                        ;
                        break;
                case closed_sphere:
                        arch
                        & pack("center", closedSphereParams.center)
                        & pack("radius", closedSphereParams.radius)
                        & pack("material", closedSphereParams.material)
                        ;
                        break;
                case triangle:
                        arch
                        & pack("vertex", triangleParams.A)
                        & pack("vertex", triangleParams.B)
                        & pack("vertex", triangleParams.C)
                        ;
                        break;
                case bvh:
                        arch
                        & pack(&Object::type, Object::Typenames, bvhParams.objects)
                        ;
                        break;
                case triangle_bvh:
                        arch
                        & pack(&Object::type, Object::Typenames, triangleBvhParams.objects)
                        ;
                        break;
                case lsystemtree:
                        arch
                        & pack("code", lsystemTreeParams.code)
                        & pack("level", lsystemTreeParams.level)
                        & pack("slices", lsystemTreeParams.slices)
                        ;
                        break;
                case instance:
                        arch
                        & pack ("transform", instanceParams.transforms)
                        & pack(&Object::type, Object::Typenames, instanceParams.objects)
                        ;
                        break;
                case forest:
                        arch
                        & pack("height-code", forestParams.heightCode)
                        & pack("distribution-code", forestParams.distributionCode)
                        & pack("target-count", forestParams.targetCount)
                        ;
                        break;
                };
        }
}

#endif // OBJECT_SER_HH_20101013
