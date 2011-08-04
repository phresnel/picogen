#ifndef VECTOR_HH_20110707
#define VECTOR_HH_20110707

#include "real.h"
#include <cassert>

namespace picogen { namespace cracker {

class Normal;
class Direction;

class Vector {
public:
        explicit operator Direction () const;
        explicit operator Normal() const;


        Vector () : x_(0), y_(0), z_(0) {}
        Vector (real x, real y, real z) : x_(x), y_(y), z_(z) {}

        real x() const { return x_; }
        real y() const { return y_; }
        real z() const { return z_; }

        Vector& operator += (Vector const &rhs) {
                x_ += rhs.x_;
                y_ += rhs.y_;
                z_ += rhs.z_;
                return *this;
        }

        Vector& operator -= (Vector const &rhs) {
                x_ -= rhs.x_;
                y_ -= rhs.y_;
                z_ -= rhs.z_;
                return *this;
        }

        Vector& operator *= (real rhs) {
                x_ *= rhs;
                y_ *= rhs;
                z_ *= rhs;
                return *this;
        }

        Vector& operator /= (real rhs) {
                assert (rhs != 0);
                return *this *= 1 / rhs;;
        }

private:

        real x_, y_, z_;
};

inline Vector operator* (Vector, real);



} }

#endif // VECTOR_HH_20110707
