#ifndef CONSTANT_H_INCLUDED_20111129
#define CONSTANT_H_INCLUDED_20111129

#include "../surfaceintegrator.h"

namespace crystal { namespace surfaceintegrators {

class Constant : public SurfaceIntegrator
{
public:
    Constant();

private:
    Radiance get (Ray const &ray) const;
};

} }

#endif // CONSTANT_H_INCLUDED_20111129
