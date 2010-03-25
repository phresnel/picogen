g++ -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT \
        $(sdl-config --libs --cflags) \
        rng-test.cc
