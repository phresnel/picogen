
#include "background/utah-sky/spectrum.cc"
#include "background/utah-sky/sunsky.cc"
#include "background/utah-sky/sunconstants.cc"
#include "background/utah-sky/atmconstants.cc"
#include "background/utah-sky/constants.cc"
#include "background/utah-sky/tospectrum.cc"


#include "background/utah-sky/static_init.hh"
extern void cracker_static_init() {
        picogen::redshift::static_init();
}
