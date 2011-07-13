#ifndef CPUCORE_H_20110713
#define CPUCORE_H_20110713

#include "color.h"

namespace picogen { namespace cracker {

class Ray;
class Scene;

class CpuCoreIntegrator {
public:
        Color operator() (Ray const &, Scene const &) const;
};

} }

#endif // CPUCORE_H_20110713
