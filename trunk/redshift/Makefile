
KALLISTOBASE := -I../
INCLUDEDIRS  := $(KALLISTOBASE)

SDLLIB := -lSDL
LIBS   := $(SDLLIB)

CXXFLAGS += -Wall -Wextra -Wshadow -pedantic 
# CXXFLAGS += -fwhole-program -funit-at-a-time
CXXFLAGS += -O3 -march=native
CXXFLAGS += -freciprocal-math  -fno-signed-zeros
CXXFLAGS += -ffast-math -march=native -fassociative-math
CXXFLAGS += $(INCLUDEDIRS) 

LINK := g++-4.3 $(LIBS)
CXX  := g++-4.3 -c $(CXXFLAGS)

redshift: main.o
	$(LINK) -o $@ $<

main.o: ./src/main.cc
	$(CXX) -o $@ $<

.PHONY: clean 
clean:
	rm -f redshift *.o

