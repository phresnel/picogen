#ifndef WHITTED_H_INCLUDED_20111129
#define WHITTED_H_INCLUDED_20111129

#include "../surfaceintegrator.h"

namespace crystal { namespace surfaceintegrators {

class Whitted : public SurfaceIntegrator
{
public:
    Whitted() = default;

private:
    Radiance get (Ray const &ray, Scene const &scene) const;
};

} }

#endif // WHITTED_H_INCLUDED_20111129
