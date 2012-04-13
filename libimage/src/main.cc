#include "surface.h"
#include "algorithm.h"

#include <iostream>
int main ()
{
        image::Surface<float> surf(10, 5);

        map_uv (surf, [](float u, float v){
                std::cout << u << ":" << v << ' ';
                return 0;
        });
}
