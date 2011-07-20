#ifndef INTERVAL_H_20110715
#define INTERVAL_H_20110715

#include "real.h"
#include <cassert>

namespace picogen { namespace cracker {

namespace detail {
        class OptionalInterval;
}

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

        typedef detail::OptionalInterval Optional;

private:
        real min_, max_;

};


namespace detail {

        class OptionalInterval {
        public:
                OptionalInterval()
                : interval_(0,1), empty_(true) {
                }

                OptionalInterval (Interval const &interval)
                : interval_(interval), empty_(false) {
                }

                explicit operator bool() const {
                        return !empty_;
                }

                Interval interval() const {
                        assert (!empty_);
                        return interval_;
                }
        private:
                Interval interval_;
                bool empty_;
        };
}
} }



#endif // INTERVAL_H_20110715

