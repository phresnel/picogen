#ifndef MONO_SUN_H_INCLUDED_20111013
#define MONO_SUN_H_INCLUDED_20111013

#include "sun.h"

namespace crystal { namespace background {

class MonoSun : public Sun {
public:
        MonoSun (Radiance const &co, Direction const &dir);

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Radiance radiance_ () const ;
private:
        Radiance  color_;
        Direction direction_;
};

} }

#endif // MONO_SUN_H_INCLUDED_20111013
