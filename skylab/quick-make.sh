g++ -O3 \
        -march=native \
        -freciprocal-math  -fno-signed-zeros -ffast-math -fassociative-math \
        -mfpmath=sse \
        -c src/*.cc
ar -rs skylab.a *.o