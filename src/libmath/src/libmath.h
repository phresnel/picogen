#ifndef MATH_H_INCLUDED_20120420
#define MATH_H_INCLUDED_20120420

#include <cmath>
#include <typeinfo>

namespace math {

using std::sqrt;

template <typename Real>
class Vector3d {
public:
        typedef Real real_type;

        real_type x, y, z;

        Vector3d () : x(0), y(0), z(0) {}
        Vector3d (real_type x, real_type y, real_type z) : x(x), y(y), z(z) {}
};

template <typename Real>
inline Real dot (Vector3d<Real> const &lhs, Vector3d<Real> const &rhs)
{
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template <typename Real>
inline Real length_sq (Vector3d<Real> const &op)
{
        return dot (op, op);
}

template <typename Real>
inline Real length (Vector3d<Real> const &rhs)
{;
        return sqrt (length_sq (rhs));
}

}

#endif // MATH_H_INCLUDED_20120420
