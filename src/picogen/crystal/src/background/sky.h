#ifndef SKY_H_20110919
#define SKY_H_20110919

#include "crystal_math.h"

namespace crystal { class Ray; }

namespace crystal { namespace background {

class Sky {
public:
        Sky ();
        virtual ~Sky() {}

        Radiance radiance(Ray const &ray) const { return this->radiance_(ray); }
private:
        virtual Radiance radiance_(Ray const &) const = 0;
};

} }

#endif // SKY_H_20110919
