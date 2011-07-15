#ifndef INTERVAL_H_20110715
#define INTERVAL_H_20110715

#include "real.h"
#include <cassert>

namespace picogen { namespace cracker {

        class Interval {
        public:
                Interval() = delete;
                Interval (real min, real max)
                        : min_(min), max_(max)
                {
                        assert (min < max);
                }

                real min() const { return min_; }
                real max() const { return max_; }
                real size() const { return max_ - min_; }

                void merge (real f) {
                        if (f < min_) min_ = f;
                        if (f < max_) max_ = f;
                }

        private:
                real min_, max_;

        };

} }

#endif // INTERVAL_H_20110715

