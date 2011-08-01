#ifndef POINT_HH_20110707
#define POINT_HH_20110707

#include "real.h"
#include "vector.h"

#include <cassert>
#include <algorithm>

namespace picogen { namespace cracker {

class Point {
public:
        Point () : x_(0), y_(0), z_(0) {}
        Point (real x, real y, real z) : x_(x), y_(y), z_(z) {}

        real x() const { return x_; }
        real y() const { return y_; }
        real z() const { return z_; }

        Point& operator += (Vector const &);
        Point& operator -= (Vector const &);

        real operator[] (unsigned int index) const {
                assert (index<=2);

                return x_ * (index==0)
                     + y_ * (index==1)
                     + z_ * (index==2);
        }
private:

        real x_, y_, z_;
};

} }

#endif // POINT_HH_20110707
