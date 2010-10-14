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

#include "primitives/primitive.hh"
#include "primitives/instance.hh"
#include "primitives/boundprimitive.hh"
#include "primitives/boundinstance.hh"
#include "primitives/closedsphere.hh"
#include "primitives/lazyquadtree.hh"
#include "primitives/horizonplane.hh"
#include "primitives/waterplane.hh"
#include "primitives/list.hh"
#include "primitives/triangle.hh"
#include "primitives/trianglebvh.hh"
#include "primitives/lsystemtree.hh"
#include "primitives/forest.hh"

#include "material/matte.hh"
#include "material/leaf0.hh"
#include "material/brdftobtdf.hh"
#include "texture/constant.hh"
#include "texture/image.hh"

#include "../include/basictypes/height-function.hh"
#include "../include/basictypes/quatsch-height-function.hh"

#include "material.hh"
#include "vertex.hh"
#include "transform.hh"
#include "basictypes/rgb.hh"

#include "color_to_redshift.hh"

#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

namespace redshift_file {

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

                inline shared_ptr<redshift::Primitive> toPrimitive () const {
                        switch (type) {
                        case lazy_quadtree: return lazyQuadtreeParams.toPrimitive();
                        case water_plane: return waterPlaneParams.toPrimitive();
                        case horizon_plane: return horizonPlaneParams.toPrimitive();
                        case closed_sphere: return closedSphereParams.toPrimitive();
                        case triangle: return triangleParams.toPrimitive();
                        case bvh: return bvhParams.toPrimitive();
                        case triangle_bvh: return triangleBvhParams.toPrimitive();
                        case lsystemtree: return lsystemTreeParams.toPrimitive();
                        case instance: return instanceParams.toPrimitive();
                        case forest: return forestParams.toPrimitive();
                        };
                        throw std::exception();
                }

                inline shared_ptr<redshift::BoundPrimitive> toBoundPrimitive () const {
                        switch (type) {
                        case lazy_quadtree: return shared_ptr<redshift::BoundPrimitive>();
                        case water_plane: return shared_ptr<redshift::BoundPrimitive>();
                        case horizon_plane: return shared_ptr<redshift::BoundPrimitive>();
                        case closed_sphere: return closedSphereParams.toBoundPrimitive();
                        case triangle: return triangleParams.toBoundPrimitive();
                        case bvh: return bvhParams.toBoundPrimitive();
                        case triangle_bvh: return triangleBvhParams.toBoundPrimitive();
                        case lsystemtree: return lsystemTreeParams.toBoundPrimitive();
                        case instance: return instanceParams.toBoundPrimitive();
                        case forest: return forestParams.toBoundPrimitive();
                        };
                        throw std::exception();
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);
        private:
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

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction;

                                std::stringstream errors;
                                /*
                                redshift::primitive::LazyQuadtree::LazyQuadtree(boost::shared_ptr<redshift::HeightFunction>&, 
                                                                                double, 
                                                                                const unsigned int&, 
                                                                                double, 
                                                                                redshift::Color)
                                
                                redshift::primitive::LazyQuadtree::LazyQuadtree(boost::shared_ptr<const redshift::HeightFunction>,
                                                                                redshift::real_t,
                                                                                unsigned int,
                                                                                redshift::real_t,
                                                                                int)
                                */

                                try {
                                        heightFunction =
                                         shared_ptr<redshift::HeightFunction> (
                                                new ::redshift::QuatschHeightFunction(code, errors)
                                        );
                                } catch (quatsch::general_exception const &ex) {
                                        // we are anyways replacing quatsch, so let's
                                        // do it kissy
                                        throw quatsch::general_exception(
                                                ex.getMessage() + ":\n\n"
                                                + errors.str()
                                        );
                                }
                                return shared_ptr<redshift::Primitive>(new LazyQuadtree(
                                        heightFunction,
                                        (redshift::real_t)size,
                                        maxRecursion,
                                        (redshift::real_t)lodFactor,
                                        toRedshift(material.color, ReflectanceSpectrum)
                                ));
                        }
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

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction =
                                        shared_ptr<redshift::HeightFunction> (
                                                new ::redshift::QuatschHeightFunction(code)
                                        );
                                return shared_ptr<redshift::Primitive>(new WaterPlane(
                                        height,
                                        heightFunction,
                                        toRedshift (material.color, ReflectanceSpectrum)
                                ));
                        }
                };
                struct HorizonPlaneParams {
                        double height;
                        Material material;

                        HorizonPlaneParams ()
                        : height(0)
                        , material(1,1,1)
                        {}

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<redshift::Primitive>(new HorizonPlane(
                                        height,
                                        toRedshift (material.color, ReflectanceSpectrum)
                                ));
                        }
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

                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<redshift::BoundPrimitive>(new ClosedSphere(
                                        redshift::Point(center.x, center.y, center.z),
                                        radius
                                ));
                        }

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };

                struct TriangleParams {
                        Vertex A, B, C;

                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;
                                using redshift::BoundPrimitive;
                                
                                typedef shared_ptr<redshift::BoundPrimitive> foo;
                                
                                redshift::Material *material = new material::Matte(
                                        shared_ptr<ColorTexture>(
                                                new texture::ConstantColor(redshift::Color::FromRGB(
                                                        0.25, 0.25, 1., ReflectanceSpectrum))
                                        ),
                                        shared_ptr<ScalarTexture>(
                                                new texture::ConstantScalar(0)
                                        )
                                );
                                
                                Triangle* triangle = new Triangle(
                                        Triangle::Vertex (A.position,
                                                          Triangle::TextureCoordinates(0,0)),
                                        Triangle::Vertex (B.position,
                                                          Triangle::TextureCoordinates(1,0)),
                                        Triangle::Vertex (C.position,
                                                          Triangle::TextureCoordinates(0,1)),
                                        shared_ptr<redshift::Material>(material)
                                );

                                return shared_ptr<redshift::BoundPrimitive>(triangle);
                        }

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };


                struct LSystemTreeParams {
                        std::string code;
                        unsigned int level;
                        unsigned int slices;

                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<redshift::BoundPrimitive>(new LSystemTree(
                                        code.c_str(), level, slices
                                ));
                        }

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };

                struct BvhParams {
                        std::vector<Object> objects;

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;
                                typedef std::vector<Object>::const_iterator I;

                                primitive::BvhBuilder builder;
                                for (I it = objects.begin(); it != objects.end(); ++it) {
                                        shared_ptr<redshift::BoundPrimitive> bp = it->toBoundPrimitive();
                                        if (bp.get()) {
                                                builder.add (bp);
                                        } else {
                                                std::cerr << "warning: unsupported primitive within Bvh: '"
                                                        << Typenames[it->type] << "'\n";
                                        }
                                }
                                return builder.toBvh();
                        }
                };


                struct TriangleBvhParams {
                        std::vector<Object> objects;

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;
                                typedef std::vector<Object>::const_iterator I;

                                primitive::TriangleBvhBuilder builder;
                                for (I it = objects.begin(); it != objects.end(); ++it) {
                                        if (it->type != triangle) {
                                                std::cerr << "warning: only triangles are allowed "
                                                        "within a triangle-bvh, but found a '"
                                                        << Typenames[it->type] << "'\n";
                                                continue;
                                        }
                                        builder.add(Triangle(
                                                Triangle::Vertex (it->triangleParams.A.position,
                                                                  Triangle::TextureCoordinates(0,0)),
                                                Triangle::Vertex (it->triangleParams.B.position,
                                                                  Triangle::TextureCoordinates(1,0)),
                                                Triangle::Vertex (it->triangleParams.C.position,
                                                                  Triangle::TextureCoordinates(0,1)),
                                                shared_ptr<redshift::Material>(new material::Matte(
                                                        shared_ptr<ColorTexture>(
                                                                new texture::ConstantColor(redshift::Color::FromRGB(
                                                                        0.25, 0.25, 1., ReflectanceSpectrum))
                                                        ),
                                                        shared_ptr<ScalarTexture>(
                                                                new texture::ConstantScalar(0)
                                                        )
                                                ))
                                        ));
                                }
                                return builder.toTriangleBvh();
                        }
                };


                struct InstanceParams {
                        std::vector<Object> objects;
                        TransformList transforms;

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                if (warnings())
                                        return shared_ptr<redshift::Primitive>();

                                return shared_ptr<redshift::Primitive>(new redshift::primitive::Instance(
                                        transforms.toRedshiftTransform(),
                                        objects[0].toPrimitive()
                                ));
                        }

                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                if (warnings())
                                        return shared_ptr<redshift::BoundPrimitive>();

                                shared_ptr<redshift::BoundPrimitive> bp = objects[0].toBoundPrimitive();

                                if (!bp) {
                                        std::cerr << "warning: Primitive is not a BoundPrimitive: '"
                                                  << Typenames[objects[0].type] << "' in InstanceParams::toBoundPrimitive()\n";
                                        return shared_ptr<redshift::BoundPrimitive>();
                                }

                                return shared_ptr<redshift::BoundPrimitive>(new redshift::primitive::BoundInstance(
                                        transforms.toRedshiftTransform(),
                                        bp
                                ));
                        }
                private:
                        bool warnings() const {
                                if (objects.size() > 1) {
                                        std::cerr << "warning: multiple objects in instance, "
                                                "but only one object per instance is allowed\n";
                                }
                                if (objects.size() == 0) {
                                        std::cerr << "warning: zero objects in instance, "
                                                "but an object is mandatory\n";
                                        return true;
                                }
                                return false;
                        }
                };

                struct ForestParams {
                        std::string heightCode, distributionCode;
                        unsigned int targetCount;

                        ForestParams()
                        : heightCode("0")
                        , distributionCode("1")
                        , targetCount(10000)
                        {}

                        shared_ptr<redshift::Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<redshift::BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction;
                                shared_ptr<redshift::DistributionFunction> distFunction;

                                std::stringstream errors;

                                try {
                                        heightFunction =
                                         shared_ptr<redshift::HeightFunction> (
                                                new ::redshift::QuatschHeightFunction(heightCode, errors)
                                        );
                                } catch (quatsch::general_exception const &ex) {
                                        // we are anyways replacing quatsch, so let's
                                        // do it kissy
                                        throw quatsch::general_exception(
                                                ex.getMessage() + ":\n\n"
                                                + errors.str()
                                        );
                                }
                                try {
                                        distFunction =
                                         shared_ptr<redshift::DistributionFunction> (
                                                new ::redshift::QuatschDistributionFunction(distributionCode, errors)
                                        );
                                } catch (quatsch::general_exception const &ex) {
                                        // we are anyways replacing quatsch, so let's
                                        // do it kissy
                                        throw quatsch::general_exception(
                                                ex.getMessage() + ":\n\n"
                                                + errors.str()
                                        );
                                }

                                return shared_ptr<redshift::BoundPrimitive>(new primitive::Forest(
                                        heightFunction,
                                        distFunction,
                                        targetCount
                                ));
                        }
                };
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
}

#endif // OBJECT_HH_20101013
