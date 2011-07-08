#ifndef POINT_HH_20110707
#define POINT_HH_20110707

#include "real.h"

namespace picogen { namespace cracker {

class Point {
public:
        Point () : x_(0), y_(0), z_(0) {}
        Point (real x, real y, real z) : x_(x), y_(y), z_(z) {}

        real x() const { return x_; }
        real y() const { return y_; }
        real z() const { return z_; }

private:

        real x_, y_, z_;
};

} }

#endif // POINT_HH_20110707
