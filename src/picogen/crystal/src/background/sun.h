#ifndef SUN_H_20110720
#define SUN_H_20110720

#include "crystal_math.h"

namespace crystal { namespace background {

class Sun {
public:
        virtual ~Sun() {}

        Ray deterministicShadowRay(Point const &from) const {
                return this->deterministicShadowRay_ (from);
        }

        Radiance radiance() const {
                return this->radiance_();
        }
private:
        virtual Ray deterministicShadowRay_ (const Point &from) const = 0;
        virtual Radiance radiance_() const = 0;
};

} }


#endif // SUN_H_20110720
