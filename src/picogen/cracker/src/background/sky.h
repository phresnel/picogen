#ifndef SKY_H_20110919
#define SKY_H_20110919

#include "math3d.h"
#include "color.h"

namespace picogen { namespace cracker {

class Ray;

class Sky {
public:
        Sky ();
        virtual ~Sky() {}

        Color radiance(Ray const &ray) const { return this->radiance_(ray); }
private:
        virtual Color radiance_(Ray const &) const = 0;
};

class MonoSky : public Sky {
public:
        MonoSky (Color const &col) : color_(col) {}
private:
        Color radiance_ (Ray const &) const { return color_; }
        Color color_;
};

class PssSky : public Sky {
public:
private:
        Color radiance_ (const Ray &) const ;
};

} }

#endif // SKY_H_20110919
