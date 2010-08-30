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
namespace redshift { namespace scenefile {

        // Rgb.
        struct WavelengthAmplitudePair {
                double wavelength;
                double amplitude;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(wavelength) & pack(amplitude);
                }
        };

        struct Spectrum {
                std::vector<WavelengthAmplitudePair> samples;

                redshift::Color toColor() const {
                        typedef redshift::Color::real_t real_t;
                        typedef std::vector<WavelengthAmplitudePair>::
                                const_iterator iterator;

                        std::vector<real_t> wavelengths;
                        std::vector<real_t> amplitudes;

                        for (iterator it = samples.begin();
                             it != samples.end();
                             ++it
                        ) {
                                wavelengths.push_back(it->wavelength);
                                amplitudes.push_back(it->amplitude);
                        }

                        return Color::FromSampled (
                                        &amplitudes[0],
                                        &wavelengths[0],
                                        samples.size());
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(samples);
                }
        };

        struct Rgb {
                double r,g,b;
                std::vector<WavelengthAmplitudePair> test;

                Rgb (double r, double g, double b) : r(r), g(g), b(b) {}
                Rgb () : r(1), g(1), b(1) {}

                redshift::Color toColor (SpectrumKind kind) const {
                        return Color::FromRGB(r,g,b, kind);
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(r) & pack(g) & pack(b);
                }
        };

        /*struct Color {

                enum Type {
                        RGB, Spectrum
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Rgb rgb;
                scenefile::Spectrum spectrum;

                Color () : type(RGB), rgb(0,1,1) {}
                Color (double r, double g, double b) : type(RGB), rgb(r,g,b) {}

                // to redshift
                redshift::Color toColor(SpectrumKind kind) const {
                        switch (type) {
                        case RGB: return rgb.toColor(kind);
                        case Spectrum: return spectrum.toColor();
                        }
                        throw std::runtime_error("unknown color type in "
                                "scenefile::Color::toColor()");
                }
                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        switch (type) {
                        case RGB:
                                arch & pack(rgb); break;
                        case Spectrum:
                                arch & pack(spectrum); break;
                        }
                }
        };

        struct ColorSum {
                std::vector<Color> colors;

        private:
                Rgb defaultColor;
        public:

                ColorSum () {}
                ColorSum (double r, double g, double b)
                : defaultColor(r,g,b)
                {}

                redshift::Color toColor(SpectrumKind kind) const {
                        typedef std::vector<Color>::const_iterator iterator;
                        using namespace redshift;
                        typedef redshift::Color RC;

                        if (colors.size() == 0)
                                return defaultColor.toColor(kind);

                        RC sum = RC(RC::real_t(0));
                        for (iterator it=colors.begin(); it != colors.end();
                             ++it
                        ) {
                                sum = sum + it->toColor(kind);
                        }
                        return sum;
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(&Color::type, Color::Typenames, colors);
                }
        };*/

        struct Color {

                enum Type {
                        RGB, Spectrum
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Rgb rgb;
                scenefile::Spectrum spectrum;

                Color () : type(RGB), rgb(0,1,1) {}
                Color (double r, double g, double b) : type(RGB), rgb(r,g,b) {}

                // to redshift
                redshift::Color toColor(SpectrumKind kind) const {
                        switch (type) {
                        case RGB: return rgb.toColor(kind);
                        case Spectrum: return spectrum.toColor();
                        }
                        throw std::runtime_error("unknown color type in "
                                "scenefile::Color::toColor()");
                }
                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("type", type, Typenames);

                        switch (type) {
                        case RGB:
                                arch & pack("rgb", rgb);
                                break;
                        case Spectrum:
                                arch & pack("spectrum", spectrum);
                                break;
                        }
                }
        };



        // Transform.
        struct Transform {
                enum Type {
                        move,
                        move_left,
                        move_right,
                        move_up,
                        move_down,
                        move_forward,
                        move_backward,

                        yaw,
                        pitch,
                        roll
                };

                static const actuarius::Enum<Type> Typenames;

                redshift::Transform toRedshiftTransform () const {
                        using redshift::Transform;
                        const double to_radians = redshift::constants::pi/180;
                        switch (type) {
                        case move: return Transform::translation(x,y,z);
                        case move_left:
                        case move_right: return Transform::translation(x,0,0);
                        case move_up:
                        case move_down: return Transform::translation(0,y,0);
                        case move_forward:
                        case move_backward:  return Transform::translation(0,0,z);

                        case yaw:   return Transform::rotationY(angle*to_radians);
                        case pitch: return Transform::rotationX(angle*to_radians);
                        case roll:  return Transform::rotationZ(angle*to_radians);
                        };
                        return redshift::Transform();
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        /*
                        switch (type) {
                        case move:
                                arch & pack(x) & pack(y) & pack(z);
                                break;
                        case move_right: arch & pack(x);            break;
                        case move_left:  arch & pack(x); x = -x;    break;
                        case move_up:    arch & pack(y);            break;
                        case move_down:  arch & pack(y);  y = -y;   break;
                        case move_forward: arch & pack(z);          break;
                        case move_backward:
                                if (Arch::serialize)
                                arch & pack(z); z = -z; break;
                        case yaw:
                        case pitch:
                        case roll:
                                arch & pack(angle);
                                break;
                        };*/

                        double left=0, right=0, up=0, down=0, forward=0, backward=0;

                        if (Arch::serialize) {
                                right = x;   left = -x;
                                up = y;      down = -y;
                                forward = z; backward = -z;
                        }

                        switch (type) {
                        case move:
                                arch & pack(right) & pack(up) & pack(forward);
                                break;
                        case move_right:    arch & pack(right); break;
                        case move_left:     arch & pack(left); break;
                        case move_up:       arch & pack(up); break;
                        case move_down:     arch & pack(down); break;
                        case move_forward:  arch & pack(forward); break;
                        case move_backward:  arch & pack(backward); break;
                        case yaw:
                        case pitch:
                        case roll:
                                arch & pack(angle);
                                break;
                        };

                        if (Arch::deserialize) {
                                x = -left     + right;
                                y = -down     + up;
                                z = -backward + forward;
                        }
                }

                Type type;
                double x,y,z;
                double angle;

                Transform ()
                : type(move), x(0), y(0), z(0), angle(0) {}

                Transform (Transform const &rhs)
                : type(rhs.type), x(rhs.x), y(rhs.y), z(rhs.z), angle(rhs.angle) {}

                Transform &operator = (Transform const &rhs) {
                        type = rhs.type;
                        x = rhs.x;
                        y = rhs.y;
                        z = rhs.z;
                        angle = rhs.angle;
                        return *this;
                }
        };

        class TransformList {
                std::vector<Transform> transforms;
        public:

                TransformList() {}
                TransformList(TransformList const &rhs)
                        : transforms(rhs.transforms) {}
                TransformList &operator = (TransformList const &rhs) {
                        transforms = rhs.transforms;
                        return *this;
                }

                Transform operator [] (int i) const {
                        return transforms [i];
                }

                int size () const {
                        return transforms.size();
                }

                void push_back (Transform const &t) {
                        transforms.push_back (t);
                }

                redshift::Transform toRedshiftTransform () const {
                        typedef std::vector<Transform>::const_iterator iterator;
                        redshift::Transform ret;
                        for (iterator it = transforms.begin(); it!=transforms.end(); ++it) {
                                ret = ret * it->toRedshiftTransform();
                        }
                        return ret;
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack (&Transform::type, Transform::Typenames, transforms);
                }
        };



        struct Material {
                Color color;

                Material () : color(0,1,1) {}

                Material (double r, double g, double b)
                : color(r,g,b)
                {
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack("color", color);
                }
        };

        struct Normal {
                double x,y,z;

                Normal (double x, double y, double z) : x(x), y(y), z(z) {
                        const double len = 1/std::sqrt(x*x + y*y + z*z);
                        x *= len;
                        y *= len;
                        z *= len;
                }
                Normal () : x(0), y(1), z(0) {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(x) & pack(y) & pack(z);
                }
        };
        struct Point {
                double x,y,z;

                Point (double x, double y, double z) : x(x), y(y), z(z) {}
                Point () : x(0), y(0), z(0) {}

                operator redshift::Point () const {
                        return redshift::Point(x,y,z);
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(x) & pack(y) & pack(z);
                }
        };
        struct Vertex {
                Point position;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack("position", position);
                }
        };

        // Object.
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

                inline shared_ptr<Primitive> toPrimitive () const {
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

                inline shared_ptr<BoundPrimitive> toBoundPrimitive () const {
                        switch (type) {
                        case lazy_quadtree: return shared_ptr<BoundPrimitive>();
                        case water_plane: return shared_ptr<BoundPrimitive>();
                        case horizon_plane: return shared_ptr<BoundPrimitive>();
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
                void serialize (Arch &arch) {
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

                        shared_ptr<Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction;

                                std::stringstream errors;

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
                                return shared_ptr<Primitive>(new LazyQuadtree(
                                        heightFunction,
                                        size,
                                        maxRecursion,
                                        lodFactor,
                                        material.color.toColor(ReflectanceSpectrum)
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

                        shared_ptr<Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction =
                                        shared_ptr<redshift::HeightFunction> (
                                                new ::redshift::QuatschHeightFunction(code)
                                        );
                                return shared_ptr<Primitive>(new WaterPlane(
                                        height,
                                        heightFunction,
                                        material.color.toColor(ReflectanceSpectrum)
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

                        shared_ptr<Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<Primitive>(new HorizonPlane(
                                        height,
                                        material.color.toColor(ReflectanceSpectrum)
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

                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<BoundPrimitive>(new ClosedSphere(
                                        redshift::Point(center.x, center.y, center.z),
                                        radius
                                ));
                        }

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };

                struct TriangleParams {
                        Vertex A, B, C;

                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<BoundPrimitive>(new Triangle(
                                        Triangle::Vertex (A.position,
                                                          Triangle::TextureCoordinates(0,0)),
                                        Triangle::Vertex (B.position,
                                                          Triangle::TextureCoordinates(1,0)),
                                        Triangle::Vertex (C.position,
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

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };


                struct LSystemTreeParams {
                        std::string code;
                        unsigned int level;
                        unsigned int slices;

                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<BoundPrimitive>(new LSystemTree(
                                        code.c_str(), level, slices
                                ));
                        }

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                };

                struct BvhParams {
                        std::vector<Object> objects;

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;
                                typedef std::vector<Object>::const_iterator I;

                                primitive::BvhBuilder builder;
                                for (I it = objects.begin(); it != objects.end(); ++it) {
                                        shared_ptr<BoundPrimitive> bp = it->toBoundPrimitive();
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

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
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

                        shared_ptr<Primitive> toPrimitive() const {
                                if (warnings())
                                        return shared_ptr<Primitive>();

                                return shared_ptr<Primitive>(new primitive::Instance(
                                        transforms.toRedshiftTransform(),
                                        objects[0].toPrimitive()
                                ));
                        }

                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
                                if (warnings())
                                        return shared_ptr<BoundPrimitive>();

                                shared_ptr<BoundPrimitive> bp = objects[0].toBoundPrimitive();

                                if (!bp) {
                                        std::cerr << "warning: Primitive is not a BoundPrimitive: '"
                                                  << Typenames[objects[0].type] << "' in InstanceParams::toBoundPrimitive()\n";
                                        return shared_ptr<BoundPrimitive>();
                                }

                                return shared_ptr<BoundPrimitive>(new primitive::BoundInstance(
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

                        shared_ptr<Primitive> toPrimitive() const {
                                return toBoundPrimitive();
                        }
                        shared_ptr<BoundPrimitive> toBoundPrimitive() const {
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

                                return shared_ptr<BoundPrimitive>(new primitive::Forest(
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



        // SurfaceIntegrator.
        struct SurfaceIntegrator {
                enum Type {
                        none,
                        whitted,
                        whitted_ambient,
                        path,
                        debug_distance,
                        debug_normals
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                unsigned int numAmbientSamples;

                SurfaceIntegrator ()
                : type(whitted_ambient), numAmbientSamples(10)
                {}


                shared_ptr<redshift::Integrator> toSurfaceIntegrator() const {
                        typedef shared_ptr<redshift::Integrator> rett;
                        switch (type) {
                        case whitted_ambient:
                                return rett(new RedshiftIntegrator(numAmbientSamples));
                        case whitted:
                                return rett(new WhittedIntegrator());
                        case path:
                                return rett(new PathIntegrator());
                        case none:
                                return rett(new NullIntegrator());
                        case debug_distance:
                                return rett(new VisualizeDistance());
                        case debug_normals:
                                return rett(new ShowSurfaceNormals());
                        };
                        return shared_ptr<Integrator>();
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & pack("type", Typenames, type);

                        switch (type) {
                        case path: break;
                        case whitted: break;
                        case whitted_ambient:
                                arch & pack("ambient-samples", numAmbientSamples);
                                break;
                        case none: break;
                        case debug_distance: break;
                        case debug_normals: break;
                        }
                }
        };



        // Volume.
        struct Volume {
                enum Type {
                        homogeneous,
                        exponential
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                shared_ptr<VolumeRegion> toVolume () const {
                        switch (type) {
                        case homogeneous:
                                return shared_ptr<VolumeRegion> (new volume::Homogeneous(
                                        sigma_a.toColor(IlluminantSpectrum),
                                        sigma_s.toColor(IlluminantSpectrum),
                                        Lve.toColor(IlluminantSpectrum),
                                        hg
                                ));
                        case exponential:
                                return shared_ptr<VolumeRegion> (new volume::Exponential(
                                        sigma_a.toColor(IlluminantSpectrum),
                                        sigma_s.toColor(IlluminantSpectrum),
                                        Lve.toColor(IlluminantSpectrum),
                                        hg,
                                        baseFactor,
                                        exponentFactor,
                                        redshift::Point(min.x, min.y, min.z),
                                        redshift::Vector(up.x, up.y, up.z),
                                        epsilon
                                ));
                        };
                        return shared_ptr<VolumeRegion>();
                }

                Color sigma_a, sigma_s, Lve;
                double hg;

                Normal up;
                Point min;
                real_t baseFactor, exponentFactor;
                double epsilon; // minimum supported density, required to build bounding volumes

                Volume ()
                : sigma_a(0,0,0)
                , sigma_s(0,0,0)
                , Lve(0,0,0)
                , hg(0)
                , up(0,1,0)
                , min(0,0,0)
                , baseFactor(1.f)
                , exponentFactor(0.05)
                , epsilon(0.05)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & actuarius::push_optional(true);
                        switch (type) {
                        case homogeneous:
                                arch & pack("absorption", sigma_a);
                                arch & pack("out-scatter", sigma_s);
                                arch & pack("emission", Lve);
                                arch & pack("phase-function", hg);
                                break;
                        case exponential:
                                arch & pack("absorption", sigma_a);
                                arch & pack("out-scatter", sigma_s);
                                arch & pack("emission", Lve);
                                arch & pack("phase-function", hg);
                                arch & pack("up", up);
                                arch & pack("min", min);
                                arch & pack("base-factor", baseFactor);
                                arch & pack("exponent-factor", exponentFactor);
                                arch & pack("epsilon", epsilon);
                                break;
                        };
                        arch & actuarius::pop_optional;
                }
        };



        // VolumeIntegrator.
        struct VolumeIntegrator {
                enum Type {
                        none,
                        emission,
                        single
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;
                double stepSize, cutoffDistance;

                VolumeIntegrator ()
                : type(none), stepSize(100.), cutoffDistance(5000.)
                {}


                shared_ptr<redshift::VolumeIntegrator> toVolumeIntegrator() const {
                        switch (type) {
                        case emission:
                                return shared_ptr<redshift::VolumeIntegrator>(
                                        new Emission(stepSize, cutoffDistance)
                                );
                        case single:
                                return shared_ptr<redshift::VolumeIntegrator>(
                                        new SingleScattering(stepSize, cutoffDistance)
                                );
                        case none:
                                return shared_ptr<redshift::VolumeIntegrator>(
                                        new NullIntegrator()
                                );
                        };
                        return shared_ptr<redshift::VolumeIntegrator>();
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack("type", Typenames, type);
                        arch & pack("step-size", stepSize);
                        arch & pack("cutoff-distance", cutoffDistance);
                }
        };


        // Background.
        struct Background {
                enum Type {
                        pss_sunsky
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Normal sunDirection;
                double turbidity;
                double sunSizeFactor;
                double sunBrightnessFactor;
                double atmosphereBrightnessFactor;
                double atmosphericFxFactor;
                double overcast;
                Color sunColor;
                Color skyFilter;
                bool atmosphericEffects;


                Background ()
                : type(pss_sunsky)
                , sunDirection(0,0.5,2)
                , turbidity(2.5)
                , sunSizeFactor(1)
                , sunBrightnessFactor(1)
                , atmosphereBrightnessFactor(1)
                , atmosphericFxFactor(1)
                , sunColor(3,3,3)
                , skyFilter(0.05,0.05,0.05)
                , atmosphericEffects(true)
                {}

                shared_ptr<redshift::Sky> toSky() const {
                        using namespace redshift;
                        switch (type) {
                        case pss_sunsky: {
                        #if 1
                                shared_ptr<redshift::background::PssSunSky> preetham (
                                 new background::PssSunSky(
                                        normalize(Vector(sunDirection.x,sunDirection.y,sunDirection.z)),
                                        turbidity,
                                        overcast,
                                        atmosphericEffects
                                ));
                                return shared_ptr<redshift::Sky> (
                                  new backgrounds::PssAdapter (
                                        preetham,
                                        sunSizeFactor, sunBrightnessFactor,
                                        atmosphereBrightnessFactor, atmosphericFxFactor
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

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        switch (type) {
                        case pss_sunsky:
                                arch & actuarius::push_optional(true);
                                arch & pack ("sun-direction", sunDirection);
                                arch & pack ("sun-size-factor", sunSizeFactor);
                                arch & pack ("sun-brightness-factor", sunBrightnessFactor);
                                arch & pack ("atmosphere-brightness-factor", atmosphereBrightnessFactor);
                                arch & pack ("atmospheric-effects-factor", atmosphericFxFactor);
                                arch & pack ("atmospheric-effects", atmosphericEffects);
                                arch & pack ("turbidity", turbidity);
                                arch & pack ("overcast", overcast);
                                /*arch & pack ("sun-color", sunColor);
                                arch & pack ("sky-filter", skyFilter);*/
                                arch & actuarius::pop_optional;
                                break;
                        };
                }
        };



        // RenderSettings.
        struct RenderSettings {
                unsigned int width, height, samplesPerPixel;
                unsigned int min_y, max_y;
                unsigned int userSeed;
                std::string title;
                SurfaceIntegrator surfaceIntegrator;
                VolumeIntegrator volumeIntegrator;

                RenderSettings ()
                : width(800), height(600), samplesPerPixel(1)
                , min_y(0), max_y(0), userSeed(0)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        //arch & pack ("title", title);
                        arch & pack ("width", width);
                        arch & pack ("height", height);
                        arch & actuarius::push_optional(true);
                        arch & pack ("min-y", min_y);
                        arch & pack ("max-y", max_y);
                        arch & pack ("seed", userSeed);
                        arch & pack ("samples-per-pixel", samplesPerPixel);
                        arch & pack ("surface-integrator", surfaceIntegrator);
                        arch & actuarius::pop_optional;
                        if (Arch::deserialize || volumeIntegrator.type != VolumeIntegrator::none) {
                                arch & actuarius::push_optional(true)
                                     & pack ("volume-integrator", volumeIntegrator)
                                     & actuarius::pop_optional
                                ;
                        }
                }
        };

        // FilmSettings.
        struct FilmSettings {
                double colorscale;
                bool convertToSrgb;

                FilmSettings ()
                : colorscale(1)
                , convertToSrgb(false)
                {}

                FilmSettings (FilmSettings const &rhs)
                : colorscale(rhs.colorscale)
                , convertToSrgb(rhs.convertToSrgb)
                {}

                FilmSettings &operator = (FilmSettings const &rhs) {
                        colorscale = rhs.colorscale;
                        convertToSrgb = rhs.convertToSrgb;
                        return *this;
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("color-scale", colorscale);
                        arch & pack ("convert-to-srgb", convertToSrgb);
                }
        };




        // Camera.
        struct Camera {

                enum Type {
                        pinhole,
                        cylindrical,
                        cubemap_left,
                        cubemap_right,
                        cubemap_front,
                        cubemap_back,
                        cubemap_bottom,
                        cubemap_top
                };
                static const actuarius::Enum<Type> Typenames;

                struct PinholeParams {
                        double front;
                        PinholeParams () : front(1) {}
                };
                struct CylindricalParams {
                        double front;
                        CylindricalParams () : front(1) {}
                };



                std::string title;
                TransformList transforms;
                Type type;
                PinholeParams pinholeParams;
                CylindricalParams cylindricalParams;


                Camera () : type(pinhole)
                {}



                shared_ptr<redshift::Camera> toRedshiftCamera (
                        unsigned int width, unsigned int height
                ) const {
                        using redshift::Camera;

                        switch (type) {
                        case pinhole:
                                return shared_ptr<Camera> (new camera::Pinhole(
                                        width, height, pinholeParams.front,
                                        transforms.toRedshiftTransform()));

                        case cylindrical:
                                return shared_ptr<Camera> (new camera::Cylindrical(
                                        width, height, cylindricalParams.front,
                                        transforms.toRedshiftTransform()));

                        case cubemap_left:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::left,
                                        transforms.toRedshiftTransform()));
                        case cubemap_right:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::right,
                                        transforms.toRedshiftTransform()));
                        case cubemap_bottom:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::bottom,
                                        transforms.toRedshiftTransform()));
                        case cubemap_top:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::top,
                                        transforms.toRedshiftTransform()));
                        case cubemap_front:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::front,
                                        transforms.toRedshiftTransform()));
                        case cubemap_back:
                                return shared_ptr<Camera> (new camera::CubeMapFace(
                                        width, height, camera::CubeMapFace::back,
                                        transforms.toRedshiftTransform()));

                        default:
                                throw std::runtime_error("only  pinhole supported");
                        };
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("transform", transforms);
                        arch & pack ("type", Typenames, type);

                        switch (type) {
                        case pinhole:
                                arch & pack ("front", pinholeParams.front);
                                break;
                        case cylindrical:
                                arch & pack ("front", cylindricalParams.front);
                                break;
                        case cubemap_front:
                        case cubemap_back:
                        case cubemap_left:
                        case cubemap_right:
                        case cubemap_bottom:
                        case cubemap_top:
                                break;
                        };
                }
        };



        // Scene = (RenderSettings+)(FilmSettings)(Objects*)
        class Scene {
                std::vector<Object> objects_;
                std::vector<Volume> volumes_;
                std::vector<RenderSettings> renderSettings_;
                std::vector<Camera> cameras_;
                std::vector<Background> backgrounds_;
                FilmSettings filmSettings_;
        public:
                void addRenderSettings (RenderSettings const &rs) {
                        renderSettings_.push_back (rs);
                }
                unsigned int renderSettingsCount() const {
                        return renderSettings_.size();
                }
                RenderSettings const & renderSettings(unsigned int index) const {
                        return renderSettings_[index];
                }
                void pruneRenderSettings () {
                        renderSettings_.clear();
                }

                void addObject (Object const &o) {
                        objects_.push_back (o);
                }
                unsigned int objectCount() const {
                        return objects_.size();
                }
                Object const & object(unsigned int index) const {
                        return objects_[index];
                }


                void addVolume (Volume const &o) {
                        volumes_.push_back (o);
                }
                unsigned int volumeCount() const {
                        return volumes_.size();
                }
                Volume const & volume(unsigned int index) const {
                        return volumes_[index];
                }

                void addBackground (Background const &o) {
                        backgrounds_.push_back (o);
                }
                unsigned int backgroundCount() const {
                        return backgrounds_.size();
                }
                Background const & background(unsigned int index) const {
                        return backgrounds_[index];
                }


                void addCamera (Camera const &o) {
                        cameras_.push_back (o);
                }
                unsigned int cameraCount() const {
                        return cameras_.size();
                }
                Camera const & camera(unsigned int index) const {
                        return cameras_[index];
                }
                void pruneCameras () {
                        cameras_.clear();
                }

                FilmSettings filmSettings () const {
                        return filmSettings_;
                }
                void setFilmSettings (FilmSettings const &fs) {
                        filmSettings_ = fs;
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & pack ("render-settings", &RenderSettings::title, renderSettings_);
                        arch & pack ("cameras", &Camera::title, cameras_);
                        arch & pack ("objects", &Object::type, Object::Typenames, objects_);
                        arch & pack ("volumes", &Volume::type, Volume::Typenames, volumes_);
                        arch & pack ("backgrounds", &Background::type, Background::Typenames, backgrounds_);
                        arch & pack ("film-settings", filmSettings_);
                }
        };
} }


redshift::shared_ptr<redshift::Scene>
 sceneDescriptionToScene (
        redshift::scenefile::Scene const &scene,
        redshift::shared_ptr<redshift::Film> renderBuffer,
        int renderSettingsIndex, int cameraIndex
);

#endif // JOBFILE_HH_INCLUDED_20100318
