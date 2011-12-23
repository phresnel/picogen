#ifndef MULTIPLY_H_INCLUDED_20111223
#define MULTIPLY_H_INCLUDED_20111223
#include "crystal_math.h"
namespace crystal { namespace pixel_shaders {
class Multiply {
public:
        Multiply (std::initializer_list<std::function<Radiance(Radiance)> >
                  shaders_)
                : shaders_(shaders_)
        {
        }

        Radiance operator() (Radiance const &rad) const {
                Radiance ret = rad;
                for (auto shader : shaders_) {
                        ret = shader (ret);
                }
                return ret;
        }
private:
        std::vector<std::function<Radiance(Radiance)> > shaders_;
};
} }
#endif
