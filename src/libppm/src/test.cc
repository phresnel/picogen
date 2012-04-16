#include <ostream>
#include <stdexcept>
#include <tuple>

namespace ppm {

// PixelFun: p(x,y) -> tuple<float,float,float>{[0..1), [0..1), [0..1)}
template <typename PixelFun>
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

#include <iostream>
#include <vector>
int main ()
{
        struct rgb {
                float r,g,b;
                rgb (float r, float g, float b) : r(r), g(g), b(b) {}
                rgb() = default;
        };
        std::vector<rgb> img(3*3);
        img[0 + 0] = rgb(0,0,0); img[0 + 1] = rgb(0,0,0.5); img[0 + 2] = rgb(0,0,1);
        img[3 + 0] = rgb(0.5,0,0); img[6 + 1] = rgb(0.5,0,0.5); img[3 + 2] = rgb(0.5,0,1);
        img[6 + 0] = rgb(1,0,0); img[6 + 1] = rgb(1,0,0.5); img[6 + 2] = rgb(1,0,1);

        ppm::write_ppm (std::cout, 3, 3,
                        [&img] (int x, int y) { auto const &rgb =  img[y*3 + x];
                                                return std::make_tuple(rgb.r, rgb.g, rgb.b); });
}
