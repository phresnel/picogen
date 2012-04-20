#ifndef PPM_H_INCLUDED_20120419
#define PPM_H_INCLUDED_20120419

#include <ostream>
#include <stdexcept>
#include <tuple>

namespace image {

// PixelFun: p(x,y) -> tuple<float,float,float>{[0..1), [0..1), [0..1)}
template <typename PixelFun>
inline
void write_ppm (std::ostream &os, int width, int height, PixelFun p)
{
        // Example from wikipedia:
        /*
        P3
        # The P3 means colors are in ASCII, then 3 columns and 2 rows,
        # then 255 for max color, then RGB triplets
        3 2
        255
        255   0   0     0 255   0     0   0 255
        255 255   0   255 255 255     0   0   0
        */

        if (width <= 0)  throw std::out_of_range ("ppm::write_ppm: width must be > 0");
        if (height <= 0) throw std::out_of_range ("ppm::write_ppm: height must be > 0");

        os << "P3\n"
           << width << ' ' << height << '\n'
           << "255\n";

        for (int y=0; y!=height; ++y) {
                for (int x=0; x!=width; ++x) {
                        auto const rgb = p(x,y);
                        const int r_ = 255*std::get<0>(rgb),
                                  g_ = 255*std::get<1>(rgb),
                                  b_ = 255*std::get<2>(rgb);
                        const int r = (r_<0) ? 0 : (r_>255) ? 255 : r_,
                                  g = (g_<0) ? 0 : (g_>255) ? 255 : g_,
                                  b = (b_<0) ? 0 : (b_>255) ? 255 : b_;
                        // The double whitespace is not by standard but for debugging
                        // convenience when looking at the output file.
                        os << r << ' ' << g << ' ' << b << "  ";
                }
                // Not sure if this is required, but for debugging convenience we'll use it anyways.
                os << '\n';
        }
}

}

#endif // PPM_H_INCLUDED_20120419
