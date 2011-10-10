#ifndef DIRECTION_INL_H_20110715
#define DIRECTION_INL_H_20110715

#include "vector.h"
#include "direction.h"
#include "normal.h"

namespace picogen { namespace cracker {

inline Direction::operator Vector () const {
        return Vector(x_, y_, z_);
}

inline Direction::operator Normal () const {
        return Normal(x_, y_, z_);
}

inline Direction Direction::operator- () const {
        return Direction (-x_, -y_, -z_);
}

} }

#endif // DIRECTION_INL_H_20110715
