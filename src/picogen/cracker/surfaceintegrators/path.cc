#include "path.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
#include "scene.h"
#include "random.h"
#include "material.h"

namespace picogen { namespace cracker {

Color PathIntegrator::operator() (Ray const &,
                                  Scene const &,
                                  Random &) const {
        return Color::Gray(0.4);
}

} }
