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

        real operator[] (unsigned int index) const {
                assert (index<=2);

                return x_ * (index==0)
                     + y_ * (index==1)
                     + z_ * (index==2);
        }
private:

        real x_, y_, z_;
};


inline Point lerp (Point const &lhs, Point const &rhs,
                   real lerp_)
{
        return Point (lhs.x()*(1-lerp_) + rhs.x()*lerp_,
                      lhs.y()*(1-lerp_) + rhs.y()*lerp_,
                      lhs.z()*(1-lerp_) + rhs.z()*lerp_);
}


inline Point min (Point const &lhs, Point const &rhs) {
        return Point (std::min(lhs.x(), rhs.x()),
                      std::min(lhs.y(), rhs.y()),
                      std::min(lhs.z(), rhs.z()));
}


inline Point max (Point const &lhs, Point const &rhs) {
        return Point (std::max(lhs.x(), rhs.x()),
                      std::max(lhs.y(), rhs.y()),
                      std::max(lhs.z(), rhs.z()));
}

} }

#endif // POINT_HH_20110707
