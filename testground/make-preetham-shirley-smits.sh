g++ -lSDL -Wall -Wextra \
        -DKALLISTO_CHECK_BOUNDS \
        ../redshift/src/basictypes/spectrum.cc \
        ../redshift/src/constants.cc \
        ../skylab/src/tospectrum.cc \
        ../skylab/src/atmconstants.cc \
        ../skylab/src/sunconstants.cc \
        ../skylab/src/sunsky.cc \
        preetham-shirley-smits.cc
