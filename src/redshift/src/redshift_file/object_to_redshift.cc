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

#include "redshift_file/object_to_redshift.hh"
#include "redshift_file/color_to_redshift.hh"
#include "redshift_file/object.hh"

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

#include "basictypes/rgb.hh"

namespace redshift_file {

redshift::shared_ptr<redshift::Primitive> toPrimitive(LazyQuadtreeParams const &ob) {
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
                        new ::redshift::QuatschHeightFunction(ob.code, errors)
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
                (redshift::real_t)ob.size,
                ob.maxRecursion,
                (redshift::real_t)ob.lodFactor,
                toRedshift(ob.material.color, ReflectanceSpectrum)
        ));
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(WaterPlaneParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;

        shared_ptr<redshift::HeightFunction> heightFunction =
                shared_ptr<redshift::HeightFunction> (
                        new ::redshift::QuatschHeightFunction(ob.code)
                );
        return shared_ptr<redshift::Primitive>(new WaterPlane(
                ob.height,
                heightFunction,
                toRedshift (ob.material.color, ReflectanceSpectrum)
        ));
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(HorizonPlaneParams const &ob)  {
        using namespace redshift;
        using namespace redshift::primitive;

        return shared_ptr<redshift::Primitive>(new HorizonPlane(
                ob.height,
                toRedshift (ob.material.color, ReflectanceSpectrum)
        ));
}   

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(ClosedSphereParams const &ob)  {
        using namespace redshift;
        using namespace redshift::primitive;

        return shared_ptr<redshift::BoundPrimitive>(new ClosedSphere(
                redshift::Point(ob.center.x, ob.center.y, ob.center.z),
                ob.radius
        ));
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(ClosedSphereParams const &ob) {
        return toBoundPrimitive(ob);
}

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(TriangleParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;
        using redshift::BoundPrimitive;
        
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
                Triangle::Vertex (ob.A.position,
                                  Triangle::TextureCoordinates(0,0)),
                Triangle::Vertex (ob.B.position,
                                  Triangle::TextureCoordinates(1,0)),
                Triangle::Vertex (ob.C.position,
                                  Triangle::TextureCoordinates(0,1)),
                shared_ptr<redshift::Material>(material)
        );

        return shared_ptr<redshift::BoundPrimitive>(triangle);
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(TriangleParams const &ob) {
        return toBoundPrimitive(ob);
}

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(LSystemTreeParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;

        return shared_ptr<redshift::BoundPrimitive>(new LSystemTree(
                ob.code.c_str(), ob.level, ob.slices
        ));
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(LSystemTreeParams const &ob) {
        return toBoundPrimitive(ob);
}

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(BvhParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;
        typedef std::vector<Object>::const_iterator I;

        primitive::BvhBuilder builder;
        for (I it = ob.objects.begin(); it != ob.objects.end(); ++it) {
                shared_ptr<redshift::BoundPrimitive> bp = toRedshift (*it);
                if (bp.get()) {
                        builder.add (bp);
                } else {
                        std::cerr << "warning: unsupported primitive within Bvh: '"
                                << Object::Typenames[it->type] << "'\n";
                }
        }
        return builder.toBvh();
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(BvhParams const &ob) {
        return toBoundPrimitive(ob);
}

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(TriangleBvhParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;
        typedef std::vector<Object>::const_iterator I;

        primitive::TriangleBvhBuilder builder;
        for (I it = ob.objects.begin(); it != ob.objects.end(); ++it) {
                if (it->type != Object::triangle) {
                        std::cerr << "warning: only triangles are allowed "
                                "within a triangle-bvh, but found a '"
                                << Object::Typenames[it->type] << "'\n";
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

redshift::shared_ptr<redshift::Primitive> toPrimitive(TriangleBvhParams const &ob) {
        return toBoundPrimitive(ob);
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(InstanceParams const &ob) {
        if (ob.warnings())
                return shared_ptr<redshift::Primitive>();

        return shared_ptr<redshift::Primitive>(new redshift::primitive::Instance(
                ob.transforms.toRedshiftTransform(),
                toRedshift (ob.objects[0])
        ));
}

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(InstanceParams const &ob) {
        if (ob.warnings())
                return shared_ptr<redshift::BoundPrimitive>();

        shared_ptr<redshift::BoundPrimitive> bp = 
                toRedshift (ob.objects[0]);

        if (!bp) {
                std::cerr << "warning: Primitive is not a BoundPrimitive: '"
                          << Object::Typenames[ob.objects[0].type] << "' in InstanceParams::toBoundPrimitive()\n";
                return shared_ptr<redshift::BoundPrimitive>();
        }

        return shared_ptr<redshift::BoundPrimitive>(new redshift::primitive::BoundInstance(
                ob.transforms.toRedshiftTransform(),
                bp
        ));
}

bool InstanceParams::warnings() const {
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

redshift::shared_ptr<redshift::BoundPrimitive> toBoundPrimitive(ForestParams const &ob) {
        using namespace redshift;
        using namespace redshift::primitive;

        shared_ptr<redshift::HeightFunction> heightFunction;
        shared_ptr<redshift::DistributionFunction> distFunction;

        std::stringstream errors;

        try {
                heightFunction =
                 shared_ptr<redshift::HeightFunction> (
                        new ::redshift::QuatschHeightFunction(ob.heightCode, errors)
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
                        new ::redshift::QuatschDistributionFunction(ob.distributionCode, errors)
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
                ob.targetCount
        ));
}

redshift::shared_ptr<redshift::Primitive> toPrimitive(ForestParams const &ob) {
        return toBoundPrimitive(ob);
}



detail::RedshiftPrimitiveOrBoundPrimitive toRedshift (Object const &ob) {
        return detail::RedshiftPrimitiveOrBoundPrimitive(ob);
}

detail::RedshiftPrimitiveOrBoundPrimitive::operator redshift::shared_ptr<redshift::Primitive> () const {
        switch (object.type) {
        case Object::lazy_quadtree: return toPrimitive (object.lazyQuadtreeParams);
        case Object::water_plane: return toPrimitive (object.waterPlaneParams);
        case Object::horizon_plane: return toPrimitive(object.horizonPlaneParams);
        case Object::closed_sphere: return toPrimitive(object.closedSphereParams);
        case Object::triangle: return toPrimitive(object.triangleParams);
        case Object::bvh: return toPrimitive(object.bvhParams);
        case Object::triangle_bvh: return toPrimitive(object.triangleBvhParams);
        case Object::lsystemtree: return toPrimitive(object.lsystemTreeParams);
        case Object::instance: return toPrimitive(object.instanceParams);
        case Object::forest: return toPrimitive(object.forestParams);
        };
        throw std::runtime_error("RedshiftPrimitiveOrBoundPrimitive::operator shared_ptr<Primitive> (), unknown primitive type");
}

detail::RedshiftPrimitiveOrBoundPrimitive::operator redshift::shared_ptr<redshift::BoundPrimitive> () const {
        switch (object.type) {
        case Object::lazy_quadtree: return redshift::shared_ptr<redshift::BoundPrimitive>();
        case Object::water_plane: return redshift::shared_ptr<redshift::BoundPrimitive>();
        case Object::horizon_plane: return redshift::shared_ptr<redshift::BoundPrimitive>();
        case Object::closed_sphere: return toBoundPrimitive(object.closedSphereParams);
        case Object::triangle: return toBoundPrimitive(object.triangleParams);
        case Object::bvh: return toBoundPrimitive(object.bvhParams);
        case Object::triangle_bvh: return toBoundPrimitive(object.triangleBvhParams);
        case Object::lsystemtree: return toBoundPrimitive(object.lsystemTreeParams);
        case Object::instance: return toBoundPrimitive(object.instanceParams);
        case Object::forest: return toBoundPrimitive(object.forestParams);
        };
        throw std::runtime_error("RedshiftPrimitiveOrBoundPrimitive::operator shared_ptr<BoundPrimitive> (), unknown primitive type");
}


}
