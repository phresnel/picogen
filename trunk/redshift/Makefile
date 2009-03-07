#------------------------------------------------------------------------------
# obj directories
#------------------------------------------------------------------------------
OBJDIR := ./obj
BASICTYPESOBJDIR := $(OBJDIR)/basictypes
APPOBJDIR := $(OBJDIR)/app
SHAPEOBJDIR := $(OBJDIR)/shapes
ACCELERATORSOBJDIR := $(OBJDIR)/accelerators

#------------------------------------------------------------------------------
# bin directories
#------------------------------------------------------------------------------
BINDIR := ./bin

#------------------------------------------------------------------------------
# include directories
#------------------------------------------------------------------------------
KALLISTOBASE := -I../
SFMLINCLUDE := -I../externs/SFML/include
INCLUDEDIRS  := $(KALLISTOBASE) $(SFMLINCLUDE)

#------------------------------------------------------------------------------
# libs
#------------------------------------------------------------------------------
SDLLIBS := `sdl-config --static-libs`
# looks like i am to dumb to properly bind to sfml
#SFMLLIBS := -L../externs/SFML/lib/
#SFMLLIBS += ../externs/SFML/lib/libsfml-graphics-s.a
#SFMLLIBS += ../externs/SFML/lib/libsfml-window-s.a
#SFMLLIBS += ../externs/SFML/lib/libsfml-system-s.a
#SFMLLIBS := -lsfml-graphics
#SFMLLIBS += -lsfml-window
#SFMLLIBS += -lsfml-system
LIBS   := $(SDLLIBS)

#------------------------------------------------------------------------------
# compiler flags
#------------------------------------------------------------------------------
COMPILE := "FAST_COMPILE"
ifeq ($(OPTIMISE), "FAST_COMPILE")
else
	CXXFLAGS += -O3 -march=native
	CXXFLAGS += -freciprocal-math  -fno-signed-zeros
	CXXFLAGS += -ffast-math -march=native -fassociative-math
endif

CXXFLAGS += -Wall -Wno-unused -pedantic
# CXXFLAGS += -Wextra -Wshadow -pedantic
CXXFLAGS += $(INCLUDEDIRS)
CXXFLAGS += `sdl-config --cflags`

LINK := g++-4.3 $(LIBS)
AR   := ar -r
CXX  := g++-4.3 -c $(CXXFLAGS) $(LIBS)
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

redshift_: $(OBJDIR)/main.o \
                $(OBJDIR)/app.a \
                $(OBJDIR)/shapes.a \
                $(OBJDIR)/accelerators.a \
                $(OBJDIR)/basictypes.a
	$(LINK) -o $(BINDIR)/redshift $^

$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<

###############################################################################
# amalgam
###############################################################################

amalgam: init-dir-tree obtain-amalgam	
	$(AMALGAM) -o ./bin/redshift-amalgam tmp/amalgam/amalgam.cc
	
obtain-amalgam: init-dir-tree
	cat special/amalgam-files | xargs cat > tmp/amalgam/amalgam.cc

###############################################################################
# all
###############################################################################
.PHONY: all
all: redshift amalgam



###############################################################################
# basictypes
###############################################################################
.PHONY: basictypes
basictypes: init-dir-tree $(OBJDIR)/basictypes.a

$(OBJDIR)/basictypes.a: $(BASICTYPESOBJDIR)/material.o \
                        $(BASICTYPESOBJDIR)/primitive.o \
                        $(BASICTYPESOBJDIR)/redshift-render.o \
                        $(BASICTYPESOBJDIR)/scene.o \
                        $(BASICTYPESOBJDIR)/shape.o
	$(AR) -o $@ $^

$(BASICTYPESOBJDIR)/material.o: ./src/basictypes/material.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/primitive.o: ./src/basictypes/primitive.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/redshift-render.o: ./src/basictypes/redshift-render.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/scene.o: ./src/basictypes/scene.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/shape.o: ./src/basictypes/shape.cc
	$(CXX) -o $@ $<



###############################################################################
# app
###############################################################################
.PHONY: app
shapes: init-dir-tree $(APPOBJDIR)/app.a

$(OBJDIR)/app.a: $(APPOBJDIR)/sdlrendertarget.o
	$(AR) -o $@ $^

$(APPOBJDIR)/sdlrendertarget.o: ./src/app/sdlrendertarget.cc
	$(CXX) -o $@ $<




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

.PHONY: init-dir-tree
init-dir-tree:
	mkdir -p $(OBJDIR)
	mkdir -p $(APPOBJDIR)
	mkdir -p $(SHAPEOBJDIR)
	mkdir -p $(ACCELERATORSOBJDIR)
	mkdir -p $(BASICTYPESOBJDIR)	
	mkdir -p ./tmp/amalgam

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
	rm -f ./tmp/amalgam/amalgam.cc
	rm -f ./bin/redshift 
	rm -f ./bin/redshift-amalgam
	rm -f $(OBJDIR)/*.o	
	rm -f $(OBJDIR)/*.a
	rm -f $(APPOBJDIR)/*.o
	rm -f $(SHAPEOBJDIR)/*.o
	rm -f $(ACCELERATORSOBJDIR)/*.o
	rm -f $(BASICTYPESOBJDIR)/*.o

