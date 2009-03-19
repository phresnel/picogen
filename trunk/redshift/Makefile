
#------------------------------------------------------------------------------
# obj directories
#------------------------------------------------------------------------------
OBJDIR := ./obj

ACCELERATORSOBJDIR  := $(OBJDIR)/accelerators
APPOBJDIR           := $(OBJDIR)/app
BASICTYPESOBJDIR    := $(OBJDIR)/basictypes
CAMERASOBJDIR       := $(OBJDIR)/cameras
INTERACTIONOBJDIR   := $(OBJDIR)/interaction
PRIMITIVESOBJDIR    := $(OBJDIR)/primitives
RENDERTARGETSOBJDIR := $(OBJDIR)/rendertargets
SHAPEOBJDIR         := $(OBJDIR)/shapes
SAMPLERSOBJDIR      := $(OBJDIR)/samplers


#------------------------------------------------------------------------------
# bin directories
#------------------------------------------------------------------------------
BINDIR := ./bin

#------------------------------------------------------------------------------
# include directories
#------------------------------------------------------------------------------
KALLISTOBASE := -I../
#SFMLINCLUDE := -I../externs/SFML/include
INCLUDEDIRS  := $(KALLISTOBASE) 

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
COMPILE := ""
ifeq ($(COMPILE), "FAST_COMPILE")
else ifeq ($(COMPILE), "DEBUG")
        CXXFLAGS += -O0 -g
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

redshift_:      $(OBJDIR)/main.o      \
                $(OBJDIR)/constants.o \
                $(OBJDIR)/app.a \
                $(OBJDIR)/accelerators.a \
                \
                $(OBJDIR)/cameras.a \
                $(OBJDIR)/interaction.a \
                $(OBJDIR)/primitives.a \
                $(OBJDIR)/rendertargets.a \
                $(OBJDIR)/shapes.a \
                $(OBJDIR)/samplers.a \
                \
                $(OBJDIR)/basictypes.a
	$(LINK) -o $(BINDIR)/redshift $^

$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<
	
$(OBJDIR)/constants.o: ./src/constants.cc
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
                         \
                        $(BASICTYPESOBJDIR)/intersection.o \
                        $(BASICTYPESOBJDIR)/differentialgeometry.o \
                        $(BASICTYPESOBJDIR)/scene.o
	$(AR) -o $@ $^

$(BASICTYPESOBJDIR)/material.o: ./src/basictypes/material.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/scene.o: ./src/basictypes/scene.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/differentialgeometry.o:  ./src/basictypes/differentialgeometry.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/intersection.o: ./src/basictypes/intersection.cc
	$(CXX) -o $@ $<


###############################################################################
# app
###############################################################################
.PHONY: app
app: init-dir-tree $(OBJDIR)/app.a

$(OBJDIR)/app.a: 
	$(AR) -o $@ $^


###############################################################################
# samplers
###############################################################################
.PHONY: samplers
samplers: init-dir-tree $(OBJDIR)/samplers.a

$(OBJDIR)/samplers.a: \
                     $(SAMPLERSOBJDIR)/sampler.o 
	$(AR) -o $@ $^

$(SAMPLERSOBJDIR)/sampler.o: \
                     ./src/samplers/sampler.cc
	$(CXX) -o $@ $<


###############################################################################
# interaction
###############################################################################
.PHONY: interaction
interaction: init-dir-tree $(OBJDIR)/interaction.a

$(OBJDIR)/interaction.a: \
                     $(INTERACTIONOBJDIR)/sdlcommandprocessor.o 
	$(AR) -o $@ $^

$(INTERACTIONOBJDIR)/sdlcommandprocessor.o: \
                     ./src/interaction/sdlcommandprocessor.cc
	$(CXX) -o $@ $<


###############################################################################
# primitives
###############################################################################
.PHONY: primitives
primitives: init-dir-tree $(OBJDIR)/primitives.a

$(OBJDIR)/primitives.a: \
                     $(PRIMITIVESOBJDIR)/closedsphere.o \
                     $(PRIMITIVESOBJDIR)/heightmap.o 
	$(AR) -o $@ $^

$(PRIMITIVESOBJDIR)/closedsphere.o: \
                     ./src/primitives/closedsphere.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/heightmap.o: \
                     ./src/primitives/heightmap.cc
	$(CXX) -o $@ $<


###############################################################################
# cameras
###############################################################################
.PHONY: cameras
cameras: init-dir-tree $(OBJDIR)/cameras.a

$(OBJDIR)/cameras.a: \
                     $(CAMERASOBJDIR)/pinhole.o 
	$(AR) -o $@ $^

$(CAMERASOBJDIR)/pinhole.o: \
                     ./src/cameras/pinhole.cc
	$(CXX) -o $@ $<



###############################################################################
# render targets
###############################################################################
.PHONY: rendertargets
rendertargets: init-dir-tree $(OBJDIR)/rendertargets.a

$(OBJDIR)/rendertargets.a: \
                     $(RENDERTARGETSOBJDIR)/sdlrendertarget.o \
                     $(RENDERTARGETSOBJDIR)/colorrendertarget.o
	$(AR) -o $@ $^

$(RENDERTARGETSOBJDIR)/sdlrendertarget.o: \
                     ./src/rendertargets/sdlrendertarget.cc
	$(CXX) -o $@ $<

$(RENDERTARGETSOBJDIR)/colorrendertarget.o: \
                     ./src/rendertargets/colorrendertarget.cc
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
	mkdir -p $(ACCELERATORSOBJDIR)
	mkdir -p $(APPOBJDIR)
	mkdir -p $(BASICTYPESOBJDIR)	
	mkdir -p $(CAMERASOBJDIR)
	mkdir -p $(INTERACTIONOBJDIR)
	mkdir -p $(PRIMITIVESOBJDIR)
	mkdir -p $(RENDERTARGETSOBJDIR)
	mkdir -p $(SAMPLERSOBJDIR)
	mkdir -p $(SHAPEOBJDIR)		
	mkdir -p ./tmp/amalgam

###############################################################################
# cleanup
###############################################################################

# do apply only when you know what you do!
.PHONY: disintegrate
disintegrate: 
	rm -r ./obj/*
	rm -r ./bin/*
	rm -r ./tmp/*


.PHONY: clean
clean:
	rm -f ./tmp/amalgam/amalgam.cc
	rm -f ./bin/redshift 
	rm -f ./bin/redshift-amalgam
	rm -f $(OBJDIR)/*.a
	rm -f $(ACCELERATORSOBJDIR)/*.o
	rm -f $(APPOBJDIR)/*.o
	rm -f $(BASICTYPESOBJDIR)/*.o
	rm -f $(CAMERASOBJDIR)/*.o
	rm -f $(INTERACTIONOBJDIR)/*.o
	rm -f $(OBJDIR)/*.o	
	rm -f $(PRIMITIVESOBJDIR)/*.o
	rm -f $(RENDERTARGETSOBJDIR)/*.o
	rm -f $(SAMPLERSOBJDIR)/*.o
	rm -f $(SHAPEOBJDIR)/*.o		


