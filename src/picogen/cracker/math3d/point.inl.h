#ifndef POINT_INL_H_20110715
#define POINT_INL_H_20110715

#include "point.h"

namespace picogen { namespace cracker {

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

#endif // POINT_INL_H_20110715
