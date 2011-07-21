#include "whitted.h"

#include "color.h"
#include "ray.h"
#include "scene.h"
#include "random.h"


namespace picogen { namespace cracker {

Color WhittedIntegrator::operator () (Ray const &ray,
                                      Scene const &scene,
                                      Random &random) const
{
        Direction d = ray.direction();
        return Color::FromRgb(
                d.x()*0.5+0.5,
                d.y()*0.5+0.5,
                d.z()*0.5+0.5
        );
}
} }
