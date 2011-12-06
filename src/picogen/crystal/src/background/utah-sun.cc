#include "utah-sun.h"


namespace crystal { namespace background {

Ray UtahSun::deterministicShadowRay_(const Point &from) const {
        return Ray (from, sunDirection_);
}

Radiance UtahSun::radiance_() const {
        return sunRadiance_;
}

} }

