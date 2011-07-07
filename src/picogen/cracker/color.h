#ifndef COLOR_H
#define COLOR_H

#include "real.h"

namespace picogen { namespace cracker {

class Color
{
public:
        static Color FromRgb (real r, real g, real b);

        Color() : r_(0), g_(0), b_(0) {}

        real r() const { return r_; }
        real g() const { return g_; }
        real b() const { return b_; }

private:
        Color (real r, real g, real b) : r_(r), g_(g), b_(b) {}

private:
        real r_, g_, b_;
};

} }

#endif // COLOR_H
