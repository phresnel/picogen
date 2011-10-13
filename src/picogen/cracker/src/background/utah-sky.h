#ifndef UTAH_SKY_H_INCLUDED_20111010
#define UTAH_SKY_H_INCLUDED_20111010

#include "sky.h"
#include "utah-sky/sunsky.hh"

namespace picogen { namespace cracker {

class UtahSky : public Sky {
public:
        UtahSky (std::shared_ptr<picogen::redshift::background::PssSunSky>);
private:
        Color radiance_ (const Ray &) const ;

private:
        std::shared_ptr<picogen::redshift::background::PssSunSky> pssSunSky_;
};

} }

#endif // UTAH_SKY_H_INCLUDED_20111010
