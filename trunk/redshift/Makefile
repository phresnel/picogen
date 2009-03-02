



# TODO: add missing target "basictypes"

# obj directories
OBJDIR := ./obj
SHAPEOBJDIR := $(OBJDIR)/shapes
ACCELERATORSOBJDIR := $(OBJDIR)/accelerators

# bin directories
BINDIR := ./bin

# include directories
KALLISTOBASE := -I../
INCLUDEDIRS  := $(KALLISTOBASE)

# libs
SDLLIB := -lSDL
LIBS   := $(SDLLIB)


# compiler flags
CXXFLAGS += -Wall -pedantic
# CXXFLAGS += -Wextra -Wshadow -pedantic
# CXXFLAGS += -fwhole-program -funit-at-a-time
CXXFLAGS += -O3 -march=native
CXXFLAGS += -freciprocal-math  -fno-signed-zeros
CXXFLAGS += -ffast-math -march=native -fassociative-math
CXXFLAGS += $(INCLUDEDIRS)

LINK := g++-4.3 $(LIBS)
AR   := ar -r
CXX  := g++-4.3 -c $(CXXFLAGS)
#  -fipa-pta -fipa-struct-reorg # gcc dies on them
AMALGAM := g++-4.3 $(CXXFLAGS) \
	-combine -fwhole-program -funit-at-a-time -DAMALGAM \
	 -fipa-cp -fipa-matrix-reorg  \
	$(LIBS)

###############################################################################
## TARGETS ####################################################################
###############################################################################

###############################################################################
# main
###############################################################################
.PHONY: redshift
redshift: init-dir-tree redshift_ 

redshift_: $(OBJDIR)/main.o $(OBJDIR)/shapes.a $(OBJDIR)/accelerators.a
	$(LINK) -o $(BINDIR)/redshift $^

$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<

###############################################################################
# amalgam
###############################################################################

amalgam: init-dir-tree
	cat special/amalgam-files | xargs cat > tmp/amalgam.cc
	$(AMALGAM) -o ./bin/redshift-amalgam tmp/amalgam.cc

###############################################################################
# all
###############################################################################
.PHONY: all
all: redshift amalgam


###############################################################################
# shapes
###############################################################################
.PHONY: shapes
shapes: init-dir-tree $(OBJDIR)/shapes.a

$(OBJDIR)/shapes.a: $(SHAPEOBJDIR)/closedsphere.o
	$(AR) -o $@ $^

$(SHAPEOBJDIR)/closedsphere.o: ./src/shapes/closedsphere.cc
	$(CXX) -o $@ $<


###############################################################################
# accelerators
###############################################################################
.PHONY: accelerators
accelerators: init-dir-tree $(OBJDIR)/accelerators.a

$(OBJDIR)/accelerators.a: $(ACCELERATORSOBJDIR)/accelerators-bvh.o
	$(AR) -o $@ $^

$(ACCELERATORSOBJDIR)/accelerators-bvh.o: ./src/accelerators/bvh.cc
	$(CXX) -o $@ $<


###############################################################################
# initialize
###############################################################################
# not really used
#.PHONY: IIII_
#IIII_: init-dir-tree


.PHONY: init-dir-tree
init-dir-tree:
	mkdir -p obj/accelerators
	mkdir -p obj/basictypes
	mkdir -p obj/shapes
	mkdir -p bin
	mkdir -p tmp

# do not apply when on svn!
.PHONY: disintegrate
disintegrate: clean
	rm -r ./obj/* || echo "./obj was clean"
	rm -r ./bin/* || echo "./bin was clean"
	rm -r ./tmp/* || echo "./tmp was clean"

###############################################################################
# cleanup
###############################################################################

.PHONY: clean
clean:
	rm -f ./tmp/amalgam.cc
	rm -f ./bin/redshift 
	rm -f ./bin/redshift-amalgam
	rm -f $(OBJDIR)/*.o	
	rm -f $(OBJDIR)/*.a
	rm -f $(SHAPEOBJDIR)/*.o
	rm -f $(ACCELERATORSOBJDIR)/*.o

