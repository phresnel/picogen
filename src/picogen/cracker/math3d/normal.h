#ifndef NORMAL_HH_20110719
#define NORMAL_HH_20110719

#include "real.h"
#include <cassert>
#include <cmath>

namespace picogen { namespace cracker {

class Vector;

class Normal {
public:
        Normal () = delete;

        explicit operator Vector () const;

        static Normal FromUnnormalized(real x, real y, real z) {
                assert (x!=0 || y!=0 || z!=0);
                const real l = 1 / length(x,y,z);
                x *= l;
                y *= l;
                z *= l;
                return Normal (x,y,z);
        }

        Normal (real x, real y, real z)
                : x_(x), y_(y), z_(z)
        {
                assert (x_!=0 || y_!=0 || z_!=0);
                assert (std::fabs (length() - 1)<=0.0001);
        }

        real x() const { return x_; }
        real y() const { return y_; }
        real z() const { return z_; }

        Vector operator* (real) const;
        Vector operator/ (real f) const;

private:
        void normalize() {
                const real l = 1 / length();
                x_ *= l;
                y_ *= l;
                z_ *= l;
        }

        static real length (real x, real y, real z) {
                return std::sqrt (x*x + y*y + z*z);
        }
        real length () const {
                return std::sqrt (x_*x_ + y_*y_ + z_*z_);
        }
private:
        real x_, y_, z_;
};


} }

#endif // NORMAL_HH_20110719
