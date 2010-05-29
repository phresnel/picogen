g++ -lSDL -Wall -Wextra \
        -DKALLISTO_CHECK_BOUNDS \
        ../redshift/src/basictypes/spectrum.cc \
        ../redshift/src/constants.cc \
        ../redshift/src/backgrounds/preetham-shirley-smits/tospectrum.cc \
        ../redshift/src/backgrounds/preetham-shirley-smits/atmconstants.cc \
        ../redshift/src/backgrounds/preetham-shirley-smits/sunconstants.cc \
        ../redshift/src/backgrounds/preetham-shirley-smits/sunsky.cc \
        preetham-shirley-smits.cc
