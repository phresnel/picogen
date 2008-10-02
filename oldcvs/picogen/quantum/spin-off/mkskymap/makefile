
CXX_FLAGS = -O3 -s   -DMKSKYMAP_STANDALONE  
CXX = g++ $(CXX_FLAGS)
OUTPUTNAME = mkskymap
OBJECTFILES = mkskymap.o functional.o LayeredNoise2d.o mtwister.o prng.o MersenneTwister.o 
#SDL = $(sdl-config --cflags --libs)
SDL = -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -L/usr/lib -lSDL

prog: $(OBJECTFILES)
	$(CXX) $(SDL) $(OBJECTFILES) -o $(OUTPUTNAME) 

mkskymap.o: ../../src/submain/mkskymap.cc 
	$(CXX) $(SDL) -c ../../src/submain/mkskymap.cc 
        
functional.o: ../../src/picosrc/misc/functional.cc 
	$(CXX) -c ../../src/picosrc/misc/functional.cc 

LayeredNoise2d.o: ../../src/picosrc/misc/functional/LayeredNoise2d.cc 
	$(CXX) -c ../../src/picosrc/misc/functional/LayeredNoise2d.cc 

mtwister.o: ../../include/picogen/foreign/libcoyotl/mtwister.cpp 
	$(CXX) -c ../../include/picogen/foreign/libcoyotl/mtwister.cpp 

prng.o: ../../include/picogen/foreign/libcoyotl/prng.cpp 
	$(CXX) -c ../../include/picogen/foreign/libcoyotl/prng.cpp 

MersenneTwister.o: ../../src/picosrc/generators/rng/MersenneTwister.cc
	$(CXX) -c ../../src/picosrc/generators/rng/MersenneTwister.cc

.PHONY: clean

clean:
	rm $(OBJECTFILES)
