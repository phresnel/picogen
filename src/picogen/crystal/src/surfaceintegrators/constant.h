#ifndef CONSTANT_H_INCLUDED_20111129
#define CONSTANT_H_INCLUDED_20111129

#include "../surfaceintegrator.h"

namespace crystal { namespace surfaceintegrators {

class Constant : public SurfaceIntegrator
{
public:
    Constant(Radiance const &surfaceColor, Radiance const &skyColor);

private:
    Radiance get (Ray const &ray, Scene const &scene) const;
    Radiance const surfaceColor_, skyColor_;
};

} }

#endif // CONSTANT_H_INCLUDED_20111129
