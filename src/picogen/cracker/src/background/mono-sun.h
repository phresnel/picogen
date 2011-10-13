#ifndef MONO_SUN_H_INCLUDED_20111013
#define MONO_SUN_H_INCLUDED_20111013

#include "sun.h"

namespace picogen { namespace cracker {

class MonoSun : public Sun {
public:
        MonoSun (Color const &co, Direction const &dir);

private:
        Ray deterministicShadowRay_ (const Point&) const ;
        Color radiance_ () const ;
private:
        Color     color_;
        Direction direction_;
};

} }

#endif // MONO_SUN_H_INCLUDED_20111013
