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

#ifndef OBJECT_HH_20101013
#define OBJECT_HH_20101013

#include "material.hh"
#include "vertex.hh"
#include "transform.hh"

#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

namespace picogen { namespace redshift_file {

        class Object;

        struct LazyQuadtreeParams {
                std::string code;
                double size;
                unsigned int maxRecursion;
                double lodFactor;
                Material material;

                LazyQuadtreeParams ()
                : code(
                        "(* 1000\n"
                        "   ([LibnoiseRidgedMulti\n"
                        "     frequency{0.0001}\n"
                        "     octave-count{12}\n"
                        "    ] x y)\n"
                        ")\n"
                )
                , size(50000)
                , maxRecursion(7)
                , lodFactor(0.00125)
                , material(0.7,0.7,0.7)
                {}                       
        };
        struct WaterPlaneParams {
                std::string code;
                double height;
                Material material;

                WaterPlaneParams ()
                : code("(* 0.1 \n"
                       "   (- 1 \n"
                       "      ([LibnoiseBillow\n"
                       "        frequency{0.1}\n"
                       "        octave-count{10}\n"
                       "        persistence{0.5}\n"
                       "       ] x y)"
                       "   )"
                       ")")
                , height(0)
                , material(1,1,1)
                {}                        
        };
        struct HorizonPlaneParams {
                double height;
                Material material;

                HorizonPlaneParams ()
                : height(0)
                , material(1,1,1)
                {}
        };
        struct ClosedSphereParams {
                Point center;
                double radius;
                Material material;

                ClosedSphereParams ()
                : center(0,0,0)
                , radius(1)
                , material(1,1,1)
                {}
        };
        struct TriangleParams {
                Vertex A, B, C;                        
        };
        struct LSystemTreeParams {
                std::string code;
                unsigned int level;
                unsigned int slices;
        };        
        struct BvhParams {
                std::vector<Object> objects;                        
        };
        struct TriangleBvhParams {
                std::vector<Object> objects;                        
        };
        struct InstanceParams {
                std::vector<Object> objects;
                TransformList transforms;                        
                bool warnings() const;
        };
        struct ForestParams {
                std::string heightCode, distributionCode;
                unsigned int targetCount;

                ForestParams()
                : heightCode("0")
                , distributionCode("1")
                , targetCount(10000)
                {}                        
        };




        struct Object {
                enum Type {
                        water_plane,
                        horizon_plane,
                        lazy_quadtree,
                        closed_sphere,
                        triangle,
                        bvh,
                        triangle_bvh,
                        lsystemtree,
                        instance,
                        forest
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;                

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);
        public:
                LazyQuadtreeParams lazyQuadtreeParams;
                WaterPlaneParams waterPlaneParams;
                HorizonPlaneParams horizonPlaneParams;
                ClosedSphereParams closedSphereParams;
                TriangleParams triangleParams;
                BvhParams bvhParams;
                TriangleBvhParams triangleBvhParams;
                LSystemTreeParams lsystemTreeParams;
                InstanceParams instanceParams;
                ForestParams forestParams;
        };
} }

#endif // OBJECT_HH_20101013
