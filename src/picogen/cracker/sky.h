#ifndef SKY_H_20110919
#define SKY_H_20110919

#include "math3d.h"
#include "color.h"

namespace picogen { namespace cracker {

class Ray;

class Sky {
public:
        Sky ();

        Color radiance(Ray const &) const;
private:
};

} }

#endif // SKY_H_20110919
