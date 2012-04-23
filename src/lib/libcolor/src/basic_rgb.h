#ifndef BASIC_RGB_H_INCLUDED_20120420
#define BASIC_RGB_H_INCLUDED_20120420

namespace color {

template <typename Real>
struct basic_rgb {
        typedef Real real_type;

        real_type r, g, b;

        basic_rgb (real_type r, real_type g, real_type b) : r(r), g(g), b(b) {}
        basic_rgb () = delete;

        static basic_rgb gray (real_type white)
        {
                return basic_rgb (white, white, white);
        }
        static basic_rgb white() { return gray(1); }
        static basic_rgb black() { return gray(0); }
};


template <typename Real, typename Min, typename Max>
inline
basic_rgb<Real> clamp (basic_rgb<Real> in, Min min, Max max)
{
        return {
                in.r<min ? min : in.r>max ? max : in.r,
                in.g<min ? min : in.g>max ? max : in.g,
                in.b<min ? min : in.b>max ? max : in.b,
        };
}

template <typename Real>
inline
basic_rgb<Real> clamp (basic_rgb<Real> in)
{
        return clamp (in, Real(0), Real(1));
}

}

#endif // BASIC_RGB_H_INCLUDED_20120420

