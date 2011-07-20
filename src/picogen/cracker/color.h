#ifndef COLOR_H
#define COLOR_H

#include "real.h"
#include <cassert>

namespace picogen { namespace cracker {

namespace detail {
        class OptionalColor;
}

class Color
{
public:
        static Color FromRgb (real r, real g, real b);

        Color() : r_(0), g_(0), b_(0) {}

        real r() const { return r_; }
        real g() const { return g_; }
        real b() const { return b_; }

        // Friend in this case is better, because whether under the hood
        // we use rgb or not is unspecified. Otherwise, non-friends would
        // have to rely on the r,g,b member functions, which are possibly
        // just approximations.
        friend Color operator* (Color const &, real f);
        friend Color operator* (real f, Color const &);

        Color operator+ (Color const &rhs) const {
                return Color (r_+rhs.r_,
                              g_+rhs.g_,
                              b_+rhs.b_);
        }

        typedef detail::OptionalColor Optional;

private:
        Color (real r, real g, real b) : r_(r), g_(g), b_(b) {}

private:
        real r_, g_, b_;
};


namespace detail {
        class OptionalColor {
        public:
                OptionalColor() : empty_(true) {}
                OptionalColor (Color const &color) : empty_(false), color_(color) {}
                explicit operator bool () const { return !empty_; }
                Color color() const { assert(!empty_); return color_; }
        private:
                bool empty_;
                Color color_;
        };
}

inline Color operator* (Color const &col, real f) {
        return Color(col.r_*f, col.g_*f, col.b_*f);
}
inline Color operator* (real f, Color const &col) {
        return Color(f*col.r_, f*col.g_, f*col.b_);
}

} }

#endif // COLOR_H
