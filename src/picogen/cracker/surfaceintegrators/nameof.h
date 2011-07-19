#ifndef NAMEOF_H_INCLUDED_20110713
#define NAMEOF_H_INCLUDED_20110713

#include "combiner.h"
#include "cpucore.h"
#include "primarydistance.h"
#include "surfacenormal.h"
#include "raydirection.h"

#include <string>
#include <sstream>

namespace picogen { namespace cracker {

inline std::string nameof (CpuCoreIntegrator) {
        return "CpuCore";
}
inline std::string nameof (PrimaryDistanceIntegrator) {
        return "PrimaryDistance";
}
inline std::string nameof (RayDirectionIntegrator) {
        return "RayDirection";
}
inline std::string nameof (SurfaceNormalIntegrator) {
        return "SurfaceNormal";
}
template<typename L, typename R>
inline std::string nameof(CombinerIntegrator<L,R> c) {
        std::stringstream ss;
        ss << "Combiner{"
           << nameof(L()) << ","
           << nameof(R()) << ","
           << c.lerp() << "}";
        return ss.str();
}


} }

#endif // NAMEOF_H_INCLUDED_20110713
