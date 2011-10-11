#ifndef SUN_H_20110720
#define SUN_H_20110720

#include "math3d.h"
#include "color.h"
#include "ray.h"

namespace picogen { namespace cracker {

class Ray;

class Sun {
public:
        virtual ~Sun() {}

        Ray deterministicShadowRay(Point const &from) const {
                return this->deterministicShadowRay_ (from);
        }

        Color radiance() const {
                return this->radiance_();
        }
private:
        virtual Ray deterministicShadowRay_ (const Point &from) const = 0;
        virtual Color radiance_() const = 0;
};

} }

namespace picogen { namespace cracker {

class MonoSun : public Sun {
public:
        MonoSun (Color const &co, Direction const &dir);

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Color radiance_ () const ;
private:
        Color     color_;
        Direction direction_;
};

} }


namespace picogen { namespace cracker {

class UtahSun {
public:

private:

};

} }

#endif // SUN_H_20110720
