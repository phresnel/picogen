#ifndef NORMAL_INL_H_20110719
#define NORMAL_INL_H_20110719

#include "vector.h"
#include "normal.h"

namespace picogen { namespace cracker {

inline Normal::operator Vector () const {
        return Vector(x_, y_, z_);
}

} }

#endif // NORMAL_INL_H_20110719
