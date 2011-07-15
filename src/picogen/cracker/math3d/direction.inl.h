#ifndef DIRECTION_INL_H_20110715
#define DIRECTION_INL_H_20110715

#include "vector.h"
#include "direction.h"

namespace picogen { namespace cracker {

inline Direction::operator Vector () const {
        return Vector(x_, y_, z_);
}

} }

#endif // DIRECTION_INL_H_20110715
