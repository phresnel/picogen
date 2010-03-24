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

#include "redshift.hh"
//#include "../include/rendertargets/sdlrendertarget.hh"
//#include "../include/rendertargets/colorrendertarget.hh"
#include "cameras/pinhole.hh"
//#include "interaction/sdlcommandprocessor.hh"
//#include "../include/interaction/rendertarget-copying-reporter.hh"
#include "basictypes/height-function.hh"
#include "basictypes/quatsch-height-function.hh"

#include "../../actuarius/actuarius.hh"


#include "../../actuarius/actuarius.hh"
#include <cmath>

namespace redshift { namespace scenefile {

        // Rgb.
        struct Rgb {
                double r,g,b;

                Rgb (double r, double g, double b) : r(r), g(g), b(b) {}
                Rgb () : r(1), g(1), b(1) {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(r) & pack(g) & pack(b);
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

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(x) & pack(y) & pack(z);
                }
        };

        // Object.
        struct Object {
                enum Type {
                        water_plane,
                        horizon_plane,
                        lazy_quadtree
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                inline shared_ptr<primitive::Primitive> toPrimitive () const {
                        switch (type) {
                        case lazy_quadtree: return lazyQuadtreeParams.toPrimitive();
                        case water_plane: return waterPlaneParams.toPrimitive();
                        case horizon_plane: return horizonPlaneParams.toPrimitive();
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
                                & pack("color", lazyQuadtreeParams.color)
                                ;
                        } break;
                        case water_plane:
                                arch
                                & pack("code", waterPlaneParams.code)
                                & pack("height", waterPlaneParams.height)
                                & pack("color", waterPlaneParams.color)
                                ;
                                break;
                        case horizon_plane:
                                arch
                                & pack("height", horizonPlaneParams.height)
                                & pack("color", horizonPlaneParams.color)
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
                        Rgb color;

                        LazyQuadtreeParams ()
                        : code("(+"
                                "  (* 600 (- 1 (abs ([LayeredNoise2d filter{cosine} seed{57} frequency{0.001} layercount{3} persistence{0.4}] x y))))"
                                "  (* 70 ([LayeredNoise2d filter{cosine} seed{542} frequency{0.01} layercount{10} persistence{0.5}] x y))"
                                "  -400"
                                ") ")
                        , size(10000)
                        , maxRecursion(7)
                        , lodFactor(0.00125)
                        , color(0.7,0.7,0.7)
                        {}

                        shared_ptr<primitive::Primitive> toPrimitive() const {
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
                                return shared_ptr<primitive::Primitive>(new LazyQuadtree(
                                        heightFunction,
                                        size,
                                        maxRecursion,
                                        lodFactor,
                                        redshift::Color::FromRGB(color.r, color.g, color.b)
                                ));
                        }
                };
                struct WaterPlaneParams {
                        std::string code;
                        double height;
                        Rgb color;

                        WaterPlaneParams ()
                        : code("(* 0.05 ([LayeredNoise2d filter{cosine} seed{13} frequency{0.02} layercount{10} persistence{0.63}] x y))")
                        , height(0)
                        , color(1,1,1)
                        {}

                        shared_ptr<primitive::Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                shared_ptr<redshift::HeightFunction> heightFunction =
                                        shared_ptr<redshift::HeightFunction> (
                                                new ::redshift::QuatschHeightFunction(code)
                                        );
                                return shared_ptr<primitive::Primitive>(new WaterPlane(
                                        height,
                                        heightFunction,
                                        redshift::Color::FromRGB(color.r,color.g,color.b)
                                ));
                        }
                };
                struct HorizonPlaneParams {
                        double height;
                        Rgb color;

                        HorizonPlaneParams ()
                        : height(0)
                        , color(1,1,1)
                        {}

                        shared_ptr<primitive::Primitive> toPrimitive() const {
                                using namespace redshift;
                                using namespace redshift::primitive;

                                return shared_ptr<primitive::Primitive>(new HorizonPlane(
                                        height,
                                        redshift::Color::FromRGB(color.r,color.g,color.b)
                                ));
                        }
                };
        public:
                LazyQuadtreeParams lazyQuadtreeParams;
                WaterPlaneParams waterPlaneParams;
                HorizonPlaneParams horizonPlaneParams;


        };



        // SurfaceIntegrator.
        struct SurfaceIntegrator {
                enum Type {
                        redshift
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                SurfaceIntegrator ()
                : type(redshift)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & pack("type", Typenames, type);
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
                                        redshift::Color::FromRGB(sigma_a.r,sigma_a.g,sigma_a.b),
                                        redshift::Color::FromRGB(sigma_s.r,sigma_s.g,sigma_s.b),
                                        redshift::Color::FromRGB(Lve.r,Lve.g,Lve.b),
                                        hg
                                ));
                        case exponential:
                                return shared_ptr<VolumeRegion> (new volume::Exponential(
                                        redshift::Color::FromRGB(sigma_a.r,sigma_a.g,sigma_a.b),
                                        redshift::Color::FromRGB(sigma_s.r,sigma_s.g,sigma_s.b),
                                        redshift::Color::FromRGB(Lve.r,Lve.g,Lve.b),
                                        hg,
                                        baseFactor,
                                        exponentFactor,
                                        redshift::Point(min.x, min.y, min.z),
                                        redshift::Vector(up.x, up.y, up.z)
                                ));
                        };
                        return shared_ptr<VolumeRegion>();
                }

                Rgb sigma_a, sigma_s, Lve;
                double hg;

                Normal up; Point min;
                real_t baseFactor, exponentFactor;

                Volume ()
                : sigma_a(0,0,0)
                , sigma_s(0,0,0)
                , Lve(0,0,0)
                , hg(0)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

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
                                break;
                        };
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
                                return shared_ptr<redshift::VolumeIntegrator>();
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
                Rgb sunColor;
                Rgb skyFilter;


                Background ()
                : type(pss_sunsky)
                , sunDirection(0,0.5,2)
                , turbidity(2.5)
                , sunSizeFactor(1)
                , sunColor(3,3,3)
                , skyFilter(0.05,0.05,0.05)
                {}

                shared_ptr<redshift::Background> toBackground() const {
                        using namespace redshift;
                        switch (type) {
                        case pss_sunsky: {
                        #if 1
                                shared_ptr<redshift::background::PssSunSky> preetham (
                                 new background::PssSunSky(
                                        normalize(Vector(sunDirection.x,sunDirection.y,sunDirection.z)),
                                        turbidity,
                                        true
                                ));
                                return shared_ptr<redshift::Background> (
                                  new backgrounds::PssAdapter (
                                        preetham, sunSizeFactor
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
                        return shared_ptr<redshift::Background>();
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        switch (type) {
                        case pss_sunsky:
                                arch & pack ("sun-direction", sunDirection);
                                arch & pack ("sun-size-factor", sunSizeFactor);
                                arch & pack ("turbidity", turbidity);
                                arch & pack ("sun-color", sunColor);
                                arch & pack ("sky-filter", skyFilter);
                                break;
                        };
                }
        };



        // RenderSettings.
        struct RenderSettings {
                unsigned int width, height, samplesPerPixel;
                std::string title;
                SurfaceIntegrator surfaceIntegrator;
                VolumeIntegrator volumeIntegrator;

                RenderSettings ()
                : width(800), height(600), samplesPerPixel(1)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        //arch & pack ("title", title);
                        arch & pack ("width", width);
                        arch & pack ("height", height);
                        arch & pack ("samples-per-pixel", samplesPerPixel);
                        arch & pack ("surface-integrator", surfaceIntegrator);
                        if (Arch::deserialize || volumeIntegrator.type != VolumeIntegrator::none)
                                arch & pack ("volume-integrator", volumeIntegrator);
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
                                case move:
                                case move_left:
                                case move_right:
                                case move_up:
                                case move_down:
                                case move_forward:
                                case move_backward:  return Transform::translation(x,y,z);

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
                                switch (type) {
                                case move:
                                        arch & pack(x) & pack(y) & pack(z);
                                        break;
                                case move_right: arch & pack(x); break;
                                case move_left:  arch & pack(x); x = -x; break;
                                case move_up:    arch & pack(y); break;
                                case move_down:  arch & pack(y);  y = -y; break;
                                case move_forward: arch & pack(z); break;
                                case move_backward: arch & pack(z); z = -z; break;
                                case yaw:
                                case pitch:
                                case roll:
                                        arch & pack(angle);
                                        break;
                                };
                        }

                        Type type;
                        double x,y,z;
                        double angle;
                };

                std::string title;
                std::vector<Transform> transforms;

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
                        arch & pack ("transform", &Transform::type, Transform::Typenames, transforms);
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
        redshift::RenderTarget::Ptr renderBuffer
);

#endif // JOBFILE_HH_INCLUDED_20100318
