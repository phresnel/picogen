#ifndef UTAH_SKY_H_INCLUDED_20111010
#define UTAH_SKY_H_INCLUDED_20111010

#include "sky.h"
#include "utah-sky/sunsky.hh"

namespace crystal { namespace background {

class UtahSky : public Sky {
public:
        UtahSky (std::shared_ptr<const crystal::redshift::background::PssSunSky>);
private:
        Radiance radiance_ (const Ray &) const ;

private:
        std::shared_ptr<const crystal::redshift::background::PssSunSky> pssSunSky_;
};

} }

#endif // UTAH_SKY_H_INCLUDED_20111010
