#ifndef POTENTIALINTERSECTION_H_20110708
#define POTENTIALINTERSECTION_H_20110708

#include "intersection.h"

namespace picogen { namespace cracker {

class PotentialIntersection {
public:
        PotentialIntersection()
        : intersection_(0), empty_(true) {
        }

        PotentialIntersection (Intersection const &intersection)
        : intersection_(intersection), empty_(false) {
        }

        explicit operator bool() const {
                return !empty_;
        }

        Intersection intersection() const {
                assert (!empty_);
                return intersection_;
        }

private:
        Intersection intersection_;
        bool empty_;
};

} }

#endif // POTENTIALINTERSECTION_H_20110708
