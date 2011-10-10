#ifndef UTAH_SKY_H_INCLUDED_20111010
#define UTAH_SKY_H_INCLUDED_20111010

#include "sky.h"

namespace picogen { namespace cracker {

class UtahSky : public Sky {
public:
        UtahSky ();
private:
        Color radiance_ (const Ray &) const ;
};

} }

#endif // UTAH_SKY_H_INCLUDED_20111010
