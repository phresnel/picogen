
#include "utah-sky/spectrum.cc"
#include "utah-sky/sunsky.cc"
#include "utah-sky/sunconstants.cc"
#include "utah-sky/atmconstants.cc"
#include "utah-sky/constants.cc"
#include "utah-sky/tospectrum.cc"


#include "utah-sky/static_init.hh"
extern void cracker_static_init() {
        picogen::redshift::static_init();
}
