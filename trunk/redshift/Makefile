
OBJDIR := ./obj
BINDIR := ./bin

KALLISTOBASE := -I../
INCLUDEDIRS  := $(KALLISTOBASE)

SDLLIB := -lSDL
LIBS   := $(SDLLIB)

CXXFLAGS += -Wall -pedantic
# CXXFLAGS += -Wextra -Wshadow -pedantic
# CXXFLAGS += -fwhole-program -funit-at-a-time
CXXFLAGS += -O3 -march=native
CXXFLAGS += -freciprocal-math  -fno-signed-zeros
CXXFLAGS += -ffast-math -march=native -fassociative-math
CXXFLAGS += $(INCLUDEDIRS)

LINK := g++-4.3 $(LIBS)
CXX  := g++-4.3 -c $(CXXFLAGS)

###############################################################################
# main
###############################################################################
redshift: $(OBJDIR)/main.o accelerators
	$(LINK) -o $(BINDIR)/$@ $<

$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<


###############################################################################
# accelerators
###############################################################################
.PHONY: accelerators
accelerators: $(OBJDIR)/accelerators-bvh.o


$(OBJDIR)/accelerators-bvh.o: ./src/accelerators/bvh.cc
	$(CXX) -o $@ $<

###############################################################################
# cleanup
###############################################################################

.PHONY: clean
clean:
	rm -f bin/redshift obj/*.o

