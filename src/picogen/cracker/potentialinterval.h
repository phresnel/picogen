#ifndef POTENTIALINTERVAL__20110715
#define POTENTIALINTERVAL__20110715

#include "interval.h"

namespace picogen { namespace cracker {

class PotentialInterval {
public:
        PotentialInterval()
        : interval_(0,1), empty_(true) {
        }

        PotentialInterval (Interval const &interval)
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

} }

#endif //  POTENTIALINTERVAL__20110715

