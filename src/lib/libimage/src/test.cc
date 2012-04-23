
#include "ppm.h"
#include <iostream>

int main ()
{
        image::write_ppm (std::cout, 240, 240, [](int x, int y) {
                int c = x ^ y;
                return std::make_tuple(c/255.f, c/255.f, c/255.f);
        });

}
