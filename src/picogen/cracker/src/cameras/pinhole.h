#ifndef PINHOLE_H_20110708
#define PINHOLE_H_20110708

#include "ray.h"

namespace picogen { namespace cracker {

class PinholeCamera {
public:
        PinholeCamera(real frontPlaneDistance)
                : frontPlaneDistance_(frontPlaneDistance)
        {}

        PinholeCamera () = delete;

        Ray operator() (real u, real v) const {
                assert(u>=0 && u<=1);
                assert(v>=0 && v<=1);

                u = u * 2 - 1;
                v = v * 2 - 1;
                return Ray (Point(0,50,0),
                            Direction::FromUnnormalized(u, v, frontPlaneDistance_));
        }

private:
        real frontPlaneDistance_;
};

} }

#endif // PINHOLE_H_20110708
