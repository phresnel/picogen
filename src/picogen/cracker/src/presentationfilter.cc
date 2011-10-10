#include "presentationfilter.h"
#include "color.h"

namespace picogen { namespace cracker {

Color PresentationFilter::operator() (Color const &col) const {
        return col * 0.0001;
}
} }
