#ifndef SUN_H_20110720
#define SUN_H_20110720

#include "math3d.h"

namespace picogen { namespace cracker {

class Ray;

class Sun {
public:
        Sun (Direction const &dir);

        Ray deterministicShadowRay(Point const &from) const;
private:
        Direction direction_;
};

} }

#endif // SUN_H_20110720
