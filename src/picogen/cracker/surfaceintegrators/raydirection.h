#ifndef RAYDIRECTION_H_20110708
#define RAYDIRECTION_H_20110708

#include "color.h"
#include "ray.h"
#include "scene.h"

namespace picogen { namespace cracker {

class Random;

class RayDirectionIntegrator {
public:
        Color operator() (Ray const &ray,
                          Scene const &,
                          Random &) const
        {
                Direction d = ray.direction();
                return Color::FromRgb(
                        d.x()*0.5+0.5,
                        d.y()*0.5+0.5,
                        d.z()*0.5+0.5
                );
        }
};

} }

#endif // RAYDIRECTION_H_20110708
