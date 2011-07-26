#ifndef COMBINER_H_20110713
#define COMBINER_H_20110713

#include "color.h"
#include <string>

namespace picogen { namespace cracker {

class Ray;
class Scene;
class Random;

template <typename IntegratorA, typename IntegratorB>
class InterlacerIntegrator {
public:
        InterlacerIntegrator() = delete;
        InterlacerIntegrator(IntegratorA a, IntegratorB b)
                : a(a), b(b)
        {
        }

        Color operator() (Ray const &ray, Scene const &scene,
                          Random &random) const {
                assert(false);
                //if (x%2) return a(ray,scene,random);
                //return b(ray,scene,random);
        }

private:
        IntegratorA a;
        IntegratorB b;
};

template <typename IntA, typename IntB>
inline
InterlacerIntegrator<IntA, IntB> combine (IntA a, IntB b) {
        return InterlacerIntegrator<IntA,IntB>(a,b);
}

} }

#endif // COMBINER_H_20110713
