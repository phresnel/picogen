#include "color.h"
#include <iostream>

int main ()
{
        typedef color::basic_rgb<float> rgb;
        rgb x = rgb::gray(-2);
        x = clamp(x, 0.5f, 0.6f);

        std::cout << x.r << ", " << x.g << ", " << x.b << '\n';
}
