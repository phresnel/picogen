#include "cpucore.h"
#include <omp.h>
#include <cstdlib>
#include <vector>


namespace picogen { namespace cracker {

namespace {
        bool               initialized = false;
        std::vector<Color> colors;
        unsigned int       num_threads;

        void static_init() {
                num_threads = omp_get_max_threads();

                std::vector<Color>(num_threads).swap (colors);
                for (auto it=colors.begin(), end=colors.end(); it!=end; ++it) {
                        const real rand_max = RAND_MAX;
                        const real r = std::rand() / rand_max,
                                   g = std::rand() / rand_max,
                                   b = std::rand() / rand_max;
                        *it = Color::FromRgb(r,g,b);
                }
                initialized = true;
                #pragma omp flush
        }

        void ensure_initialized() {
                if (initialized) return;
                #pragma omp critical
                {
                        #pragma omp flush (initialized)
                        if (!initialized) static_init();
                }
        }
}

Color CpuCoreIntegrator::operator() (Ray const &, Scene const &) const
{
        ensure_initialized();
        return colors [omp_get_thread_num() % num_threads];
}

} }
