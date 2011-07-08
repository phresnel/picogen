#ifndef RAY_HH_20110707
#define RAY_HH_20110707

#include <cassert>
#include "point.h"
#include "direction.h"

namespace picogen { namespace cracker {

class Ray {
public:
        Ray (Point const &origin, Direction const &direction)
                : origin_(origin), direction_(direction)
        {
        }

        Ray () = delete; // Can't because of direction.

        Point origin()     const { return origin_; }
        Direction direction() const { return direction_; }


private:
        Point origin_;
        Direction direction_;
};

} }

#endif
