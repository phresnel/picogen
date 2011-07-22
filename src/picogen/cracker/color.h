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
        static Color Black   ();

        Color() : r_(0), g_(0), b_(0) {}

        real r() const { return r_; }
        real g() const { return g_; }
        real b() const { return b_; }


        Color& operator+= (Color const &rhs) {
                r_ += rhs.r_;
                g_ += rhs.g_;
                b_ += rhs.b_;
                return *this;
        }

        Color& operator *= (real f) {
                r_ *= f;
                g_ *= f;
                b_ *= f;
                return *this;
        }

        Color& operator *= (Color const &rhs) {
                r_ *= rhs.r_;
                g_ *= rhs.g_;
                b_ *= rhs.b_;
                return *this;
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

inline Color operator* (Color lhs, real rhs) {
        return lhs *= rhs;
}
inline Color operator* (real lhs, Color rhs) {
        return rhs *= lhs;
}
inline Color operator* (Color lhs, Color const &rhs) {
        return lhs *= rhs;
}

inline Color operator+ (Color lhs, Color const &rhs) {
        return lhs += rhs;
}


} }

#endif // COLOR_H
