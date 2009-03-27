
#------------------------------------------------------------------------------
# obj directories
#------------------------------------------------------------------------------
LIBDIR := ./lib
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
	CXXFLAGS += -mfpmath=sse
endif

CXXFLAGS += -Wall -Wno-unused -pedantic
# CXXFLAGS += -Wextra -Wshadow -pedantic
CXXFLAGS += $(INCLUDEDIRS)
CXXFLAGS += `sdl-config --cflags`

LINK := g++-4.3 $(LIBS)
AR   := ar -rs 
CXX  := g++-4.3 -c $(CXXFLAGS) $(LIBS)
#  -fipa-pta -fipa-struct-reorg # gcc dies on them
AMALGAM := g++-4.3 $(CXXFLAGS) \
	-combine -fwhole-program -funit-at-a-time -DAMALGAM \
	 -fipa-cp -fipa-matrix-reorg  \
	$(LIBS)


OBJFILES :=
# main.o is intentionally excluded, and must be included by main targets 
OBJFILES += $(OBJDIR)/constants.o
OBJFILES += $(BASICTYPESOBJDIR)/material.o
OBJFILES += $(BASICTYPESOBJDIR)/scene.o
OBJFILES += $(BASICTYPESOBJDIR)/differentialgeometry.o
OBJFILES += $(BASICTYPESOBJDIR)/intersection.o
OBJFILES += $(SAMPLERSOBJDIR)/sampler.o
OBJFILES += $(SHAPEOBJDIR)/closedsphere.o
OBJFILES += $(INTERACTIONOBJDIR)/sdlcommandprocessor.o
OBJFILES += $(PRIMITIVESOBJDIR)/heightmap.o
OBJFILES += $(PRIMITIVESOBJDIR)/closedsphere.o
OBJFILES += $(CAMERASOBJDIR)/pinhole.o
OBJFILES += $(ACCELERATORSOBJDIR)/accelerators-bvh.o
OBJFILES += $(RENDERTARGETSOBJDIR)/colorrendertarget.o
OBJFILES += $(RENDERTARGETSOBJDIR)/sdlrendertarget.o


###############################################################################
## MAIN TARGETS ###############################################################
###############################################################################

.PHONY: all
all: redshift amalgam lib


.PHONY: redshift
redshift: init-dir-tree redshift_

redshift_: $(OBJDIR)/main.o $(OBJFILES)                
	$(LINK) -o $(BINDIR)/redshift $^

	

.PHONY: lib
lib: init-dir-tree lib_

lib_: $(OBJDIR)/lib-main.o $(OBJFILES)                
	$(AR) -o $(LIBDIR)/libpicogen.a $^



amalgam: init-dir-tree obtain-amalgam	
	$(AMALGAM) -o ./bin/redshift-amalgam tmp/amalgam/amalgam.cc
	
obtain-amalgam: init-dir-tree
	cat special/amalgam-files | xargs cat > tmp/amalgam/amalgam.cc



###############################################################################
# main, constants
###############################################################################


$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<
	
$(OBJDIR)/lib-main.o: ./src/main.cc
	$(CXX) -DPICOGENLIB -o $@ $<
 
	
$(OBJDIR)/constants.o: ./src/constants.cc
	$(CXX) -o $@ $<

	                

###############################################################################
# basictypes
###############################################################################

$(BASICTYPESOBJDIR)/material.o: ./src/basictypes/material.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/scene.o: ./src/basictypes/scene.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/differentialgeometry.o: \
                         ./src/basictypes/differentialgeometry.cc
	$(CXX) -o $@ $<
	
$(BASICTYPESOBJDIR)/intersection.o: ./src/basictypes/intersection.cc
	$(CXX) -o $@ $<



###############################################################################
# samplers
###############################################################################

$(SAMPLERSOBJDIR)/sampler.o: \
                     ./src/samplers/sampler.cc
	$(CXX) -o $@ $<



###############################################################################
# interaction
###############################################################################

$(INTERACTIONOBJDIR)/sdlcommandprocessor.o: \
                     ./src/interaction/sdlcommandprocessor.cc
	$(CXX) -o $@ $<



###############################################################################
# primitives
###############################################################################

$(PRIMITIVESOBJDIR)/closedsphere.o: \
                     ./src/primitives/closedsphere.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/heightmap.o: \
                     ./src/primitives/heightmap.cc
	$(CXX) -o $@ $<



###############################################################################
# cameras
###############################################################################

$(CAMERASOBJDIR)/pinhole.o: \
                     ./src/cameras/pinhole.cc
	$(CXX) -o $@ $<



###############################################################################
# render targets
###############################################################################

$(RENDERTARGETSOBJDIR)/sdlrendertarget.o: \
                     ./src/rendertargets/sdlrendertarget.cc
	$(CXX) -o $@ $<

$(RENDERTARGETSOBJDIR)/colorrendertarget.o: \
                     ./src/rendertargets/colorrendertarget.cc
	$(CXX) -o $@ $<



###############################################################################
# shapes
###############################################################################

$(SHAPEOBJDIR)/closedsphere.o: ./src/shapes/closedsphere.cc
	$(CXX) -o $@ $<



###############################################################################
# accelerators
###############################################################################

$(ACCELERATORSOBJDIR)/accelerators-bvh.o: ./src/accelerators/bvh.cc
	$(CXX) -o $@ $<



###############################################################################
# initialize
###############################################################################

.PHONY: init-dir-tree
init-dir-tree:
	mkdir -p $(LIBDIR)
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
	rm -r ./lib/*


.PHONY: clean
clean: init-dir-tree
	rm -f ./tmp/amalgam/amalgam.cc
	rm -f ./bin/redshift 
	rm -f ./bin/redshift-amalgam
	rm -f $(LIBDIR)/*.a
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


