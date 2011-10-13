#include "utah-sun.h"


namespace picogen { namespace cracker {

Ray UtahSun::deterministicShadowRay_(const Point &from) const {
        return Ray (from, sunDirection_);
}

Color UtahSun::radiance_() const {
        return sunRadiance_;
}

} }

