#ifndef COMBINER_H_20110713
#define COMBINER_H_20110713

#include "color.h"
#include <string>

namespace picogen { namespace cracker {

class Ray;
class Scene;

template <typename IntegratorA, typename IntegratorB>
class CombinerIntegrator {
public:
        CombinerIntegrator() = delete;
        CombinerIntegrator(IntegratorA a, IntegratorB b, real lerp)
                : a(a), b(b), lerp(lerp)
        {}

        Color operator() (Ray const &ray, Scene const &scene) const {
                return (1-lerp)*a(ray,scene)
                     + lerp*b(ray,scene);
        }

private:
        IntegratorA a;
        IntegratorB b;
        real lerp;
};

template <typename IntA, typename IntB>
inline
CombinerIntegrator<IntA, IntB> combine (IntA a, IntB b, real lerp) {
        return CombinerIntegrator<IntA,IntB>(a,b, lerp);
}

} }

#endif // COMBINER_H_20110713
