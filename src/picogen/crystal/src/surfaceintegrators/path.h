#ifndef PATH_H_INCLUDED_20111223
#define PATH_H_INCLUDED_20111223

#include "../surfaceintegrator.h"

namespace crystal { namespace surfaceintegrators {

class Path : public SurfaceIntegrator
{
public:
    Path() = default;

private:
    Radiance get (Ray const &ray, Scene const &scene) const;
};

} }

#endif // PATH_H_INCLUDED_20111223
