
#------------------------------------------------------------------------------
# obj directories
#------------------------------------------------------------------------------
LIBDIR := ./lib
OBJDIR := ./obj

ACCELERATORSOBJDIR  := $(OBJDIR)/accelerators
APPOBJDIR           := $(OBJDIR)/app
BACKGROUNDSOBJDIR   := $(OBJDIR)/backgrounds
BASICTYPESOBJDIR    := $(OBJDIR)/basictypes
CAMERASOBJDIR       := $(OBJDIR)/cameras
INTEGRATORSOBJDIR   := $(OBJDIR)/integrators
INTERACTIONOBJDIR   := $(OBJDIR)/interaction
BXDFOBJDIR          := $(OBJDIR)/bxdf
MATERIALOBJDIR      := $(OBJDIR)/material
PRIMITIVESOBJDIR    := $(OBJDIR)/primitives
RENDERTARGETSOBJDIR := $(OBJDIR)/rendertargets
SHAPEOBJDIR         := $(OBJDIR)/shapes
SAMPLERSOBJDIR      := $(OBJDIR)/samplers
VOLUMESOBJDIR       := $(OBJDIR)/volume
TEXTURESOBJDIR      := $(OBJDIR)/texture


#------------------------------------------------------------------------------
# bin directories
#------------------------------------------------------------------------------
BINDIR := ./bin

#------------------------------------------------------------------------------
# include directories
#------------------------------------------------------------------------------
KALLISTOBASE := 
#SFMLINCLUDE := -I../externs/SFML/include
INCLUDEDIRS  := -I../../auxiliary/ \
                -I./include/ \
                -I../ \
                -I/usr/include/OpenEXR/ -I/mingw/include/OpenEXR/

#------------------------------------------------------------------------------
# libs
#------------------------------------------------------------------------------
SDLLIBS := `sdl-config --static-libs` -lSDL_image
BOOSTLIBS := `./boost-libs.sh`

LIBS   := $(SDLLIBS) -lnoise $(BOOSTLIBS) -lIlmImf \
	  -lz -lImath -lHalf -lIex -lIlmThread

#------------------------------------------------------------------------------
# compiler flags
#------------------------------------------------------------------------------
COMPILE := ""
CXXFLAGS += -D__STDC_LIMIT_MACROS
ifeq ($(COMPILE), "FAST_COMPILE")
else ifeq ($(COMPILE), "DEBUG")
	CXXFLAGS += -O0 -g
else
	CXXFLAGS += -O3
	CXXFLAGS += -ffast-math
	CXXFLAGS += -msse -mfpmath=sse
	CXXFLAGS += -fopenmp
endif

CXXFLAGS += -Wextra -Wall -Wno-unused -pedantic -Wstrict-aliasing=0
CXXFLAGS += $(INCLUDEDIRS)
CXXFLAGS += `sdl-config --cflags`

LINK := g++ -mconsole -s -fopenmp \
        `./machine-linker-settings.sh`

# -Wl,--enable-auto-import
AR   := ar -rs
CXX  := g++ -c $(CXXFLAGS)
#  -fipa-pta -fipa-struct-reorg # gcc dies on them
AMALGAM := g++ $(CXXFLAGS) \
	-combine -fwhole-program -funit-at-a-time -DAMALGAM \
	 -fipa-cp -fipa-matrix-reorg  \
	$(LIBS)


OBJFILES :=
# main.o is intentionally excluded, and must be included by main targets
OBJFILES += $(OBJDIR)/redshift_file_to_redshift.o
OBJFILES += $(OBJDIR)/redshift_file_save_load.o
OBJFILES += $(OBJDIR)/redshift_file_scene.o
OBJFILES += $(OBJDIR)/redshift_file_object.o
OBJFILES += $(OBJDIR)/redshift_file_object_to_redshift.o

OBJFILES += $(OBJDIR)/constants.o
OBJFILES += $(OBJDIR)/jobfile.o
OBJFILES += $(OBJDIR)/save_openexr.o
OBJFILES += $(OBJDIR)/quatsch-integration.o
OBJFILES += $(OBJDIR)/xyto/*.o

OBJFILES += $(BACKGROUNDSOBJDIR)/pss-adapter.o

OBJFILES += $(BACKGROUNDSOBJDIR)/preetham-shirley-smits_atmconstants.o
OBJFILES += $(BACKGROUNDSOBJDIR)/preetham-shirley-smits_sunconstants.o
OBJFILES += $(BACKGROUNDSOBJDIR)/preetham-shirley-smits_sunsky.o
OBJFILES += $(BACKGROUNDSOBJDIR)/preetham-shirley-smits_tospectrum.o

OBJFILES += $(BASICTYPESOBJDIR)/material.o
OBJFILES += $(BASICTYPESOBJDIR)/scene.o
#OBJFILES += $(BASICTYPESOBJDIR)/heightmap.o
OBJFILES += $(BASICTYPESOBJDIR)/differentialgeometry.o
OBJFILES += $(BASICTYPESOBJDIR)/intersection.o
OBJFILES += $(BASICTYPESOBJDIR)/bsdf.o
OBJFILES += $(BASICTYPESOBJDIR)/volume.o
OBJFILES += $(BASICTYPESOBJDIR)/spectrum.o

#OBJFILES += $(SAMPLERSOBJDIR)/sampler.o

OBJFILES += $(SHAPEOBJDIR)/closedsphere.o

#OBJFILES += $(INTEGRATORSOBJDIR)/direct-lighting.o
OBJFILES += $(INTEGRATORSOBJDIR)/whitted.o
OBJFILES += $(INTEGRATORSOBJDIR)/redshift.o
OBJFILES += $(INTEGRATORSOBJDIR)/path.o
OBJFILES += $(INTEGRATORSOBJDIR)/emission.o
OBJFILES += $(INTEGRATORSOBJDIR)/single-scattering.o
OBJFILES += $(INTERACTIONOBJDIR)/sdlcommandprocessor.o
OBJFILES += $(INTERACTIONOBJDIR)/passivecommandprocessor.o

OBJFILES += $(BXDFOBJDIR)/lambertian.o
OBJFILES += $(BXDFOBJDIR)/mirror.o
OBJFILES += $(BXDFOBJDIR)/brdftobtdf.o

OBJFILES += $(MATERIALOBJDIR)/matte.o
OBJFILES += $(MATERIALOBJDIR)/leaf0.o

#OBJFILES += $(PRIMITIVESOBJDIR)/heightmap.o
#OBJFILES += $(PRIMITIVESOBJDIR)/booleanfield.o
OBJFILES += $(PRIMITIVESOBJDIR)/closedsphere.o
OBJFILES += $(PRIMITIVESOBJDIR)/lazyquadtree.o
OBJFILES += $(PRIMITIVESOBJDIR)/horizonplane.o
OBJFILES += $(PRIMITIVESOBJDIR)/waterplane.o
OBJFILES += $(PRIMITIVESOBJDIR)/list.o
OBJFILES += $(PRIMITIVESOBJDIR)/bvh.o
OBJFILES += $(PRIMITIVESOBJDIR)/trianglebvh.o
OBJFILES += $(PRIMITIVESOBJDIR)/triangle.o
OBJFILES += $(PRIMITIVESOBJDIR)/lsystemtree.o
OBJFILES += $(PRIMITIVESOBJDIR)/instance.o
OBJFILES += $(PRIMITIVESOBJDIR)/boundinstance.o
OBJFILES += $(PRIMITIVESOBJDIR)/forest.o

OBJFILES += $(CAMERASOBJDIR)/cylindrical.o
OBJFILES += $(CAMERASOBJDIR)/pinhole.o
OBJFILES += $(CAMERASOBJDIR)/cubemapface.o

#OBJFILES += $(ACCELERATORSOBJDIR)/accelerators-bvh.o

OBJFILES += $(RENDERTARGETSOBJDIR)/colorrendertarget.o
OBJFILES += $(RENDERTARGETSOBJDIR)/sdlrendertarget.o

OBJFILES += $(VOLUMESOBJDIR)/homogeneous.o
OBJFILES += $(VOLUMESOBJDIR)/exponential.o
OBJFILES += $(VOLUMESOBJDIR)/list.o

OBJFILES += $(TEXTURESOBJDIR)/image.o


###############################################################################
## MAIN TARGETS ###############################################################
###############################################################################

.PHONY: all
all: redshift amalgam lib


.PHONY: redshift
redshift: init-dir-tree redshift_

redshift_: $(OBJDIR)/main.o $(OBJFILES)
	$(LINK) -o $(BINDIR)/redshift $^ $(LIBS)



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


$(OBJDIR)/redshift_file_to_redshift.o: \
                ./src/redshift_file/to_redshift/redshift_file_to_redshift.cc
	$(CXX) -o $@ $<

$(OBJDIR)/redshift_file_save_load.o: ./src/redshift_file/save_load.cc
	$(CXX) -o $@ $<

$(OBJDIR)/redshift_file_scene.o: ./src/redshift_file/scene.cc
	$(CXX) -o $@ $<

$(OBJDIR)/redshift_file_object.o: ./src/redshift_file/object.cc
	$(CXX) -o $@ $<

$(OBJDIR)/redshift_file_object_to_redshift.o: \
                ./src/redshift_file/to_redshift/object_to_redshift.cc
	$(CXX) -o $@ $<


        
$(OBJDIR)/main.o: ./src/main.cc
	$(CXX) -o $@ $<

$(OBJDIR)/lib-main.o: ./src/main.cc
	$(CXX) -DPICOGENLIB -o $@ $<

$(OBJDIR)/constants.o: ./src/constants.cc
	$(CXX) -o $@ $<

$(OBJDIR)/jobfile.o: ./src/jobfile.cc
	$(CXX) -o $@ $<

$(OBJDIR)/save_openexr.o: ./src/save_openexr.cc
	$(CXX) -o $@ $<

$(OBJDIR)/quatsch-integration.o: ./src/quatsch-integration.cc
	$(CXX) -o $@ $<


$(OBJDIR)/xyto/*.o: ../xyto/*.cc
	mkdir -p obj/xyto && \
	cd obj/xyto && \
	$(CXX) ../../../xyto/*.cc


$(OBJDIR)/libxyto.a: $(OBJDIR)/xyto/*.o
	cd obj/xyto && \
	$(AR) -o $@ *




###############################################################################
# texture
###############################################################################

$(TEXTURESOBJDIR)/image.o: \
                     ./src/texture/image.cc
	$(CXX) -o $@ $<



###############################################################################
# backgrounds
###############################################################################

$(BACKGROUNDSOBJDIR)/pss-adapter.o: \
                     ./src/backgrounds/pss-adapter.cc
	$(CXX) -o $@ $<

$(BACKGROUNDSOBJDIR)/preetham-shirley-smits_atmconstants.o: \
                     ./src/backgrounds/preetham-shirley-smits/atmconstants.cc
	$(CXX) -o $@ $<

$(BACKGROUNDSOBJDIR)/preetham-shirley-smits_sunconstants.o: \
                     ./src/backgrounds/preetham-shirley-smits/sunconstants.cc
	$(CXX) -o $@ $<

$(BACKGROUNDSOBJDIR)/preetham-shirley-smits_sunsky.o: \
                     ./src/backgrounds/preetham-shirley-smits/sunsky.cc
	$(CXX) -o $@ $<

$(BACKGROUNDSOBJDIR)/preetham-shirley-smits_tospectrum.o: \
                     ./src/backgrounds/preetham-shirley-smits/tospectrum.cc
	$(CXX) -o $@ $<



###############################################################################
# basictypes
###############################################################################

$(BASICTYPESOBJDIR)/material.o: ./src/basictypes/material.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/scene.o: ./src/basictypes/scene.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/heightmap.o: ./src/basictypes/heightmap.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/differentialgeometry.o: \
                         ./src/basictypes/differentialgeometry.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/intersection.o: ./src/basictypes/intersection.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/bsdf.o: ./src/basictypes/bsdf.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/volume.o: ./src/basictypes/volume.cc
	$(CXX) -o $@ $<

$(BASICTYPESOBJDIR)/spectrum.o: ./src/basictypes/spectrum.cc
	$(CXX) -o $@ $<



###############################################################################
# samplers
###############################################################################

$(SAMPLERSOBJDIR)/sampler.o: \
                     ./src/samplers/sampler.cc
	$(CXX) -o $@ $<


###############################################################################
# integrators
###############################################################################

$(INTEGRATORSOBJDIR)/whitted.o: \
                     ./src/integrators/whitted.cc
	$(CXX) -o $@ $<

#$(INTEGRATORSOBJDIR)/direct-lighting.o: \
#                     ./src/integrators/direct-lighting.cc
#	$(CXX) -o $@ $<

$(INTEGRATORSOBJDIR)/redshift.o: \
                     ./src/integrators/redshift.cc
	$(CXX) -o $@ $<

$(INTEGRATORSOBJDIR)/path.o: \
                     ./src/integrators/path.cc
	$(CXX) -o $@ $<

$(INTEGRATORSOBJDIR)/emission.o: \
                     ./src/integrators/emission.cc
	$(CXX) -o $@ $<

$(INTEGRATORSOBJDIR)/single-scattering.o: \
                     ./src/integrators/single-scattering.cc
	$(CXX) -o $@ $<


###############################################################################
# interaction
###############################################################################

$(INTERACTIONOBJDIR)/sdlcommandprocessor.o: \
                     ./src/interaction/sdlcommandprocessor.cc
	$(CXX) -o $@ $<


$(INTERACTIONOBJDIR)/passivecommandprocessor.o: \
                     ./src/interaction/passivecommandprocessor.cc
	$(CXX) -o $@ $<


###############################################################################
# primitives
###############################################################################

$(PRIMITIVESOBJDIR)/forest.o: \
                     ./src/primitives/forest.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/boundinstance.o: \
                     ./src/primitives/boundinstance.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/instance.o: \
                     ./src/primitives/instance.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/lsystemtree.o: \
                     ./src/primitives/lsystemtree.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/bvh.o: \
                     ./src/primitives/bvh.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/trianglebvh.o: \
                     ./src/primitives/trianglebvh.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/triangle.o: \
                     ./src/primitives/triangle.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/closedsphere.o: \
                     ./src/primitives/closedsphere.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/heightmap.o: \
                     ./src/primitives/heightmap.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/lazyquadtree.o: \
                     ./src/primitives/lazyquadtree.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/horizonplane.o: \
                     ./src/primitives/horizonplane.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/waterplane.o: \
                     ./src/primitives/waterplane.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/list.o: \
                     ./src/primitives/list.cc
	$(CXX) -o $@ $<

$(PRIMITIVESOBJDIR)/booleanfield.o: \
                     ./src/primitives/booleanfield.cc \
                     ./src/primitives/booleanfield-impl.cc
	$(CXX) -o $@ $<



###############################################################################
# cameras
###############################################################################

$(CAMERASOBJDIR)/cylindrical.o: \
                     ./src/cameras/cylindrical.cc
	$(CXX) -o $@ $<

$(CAMERASOBJDIR)/pinhole.o: \
                     ./src/cameras/pinhole.cc
	$(CXX) -o $@ $<

$(CAMERASOBJDIR)/cubemapface.o: \
                     ./src/cameras/cubemapface.cc
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
# materials
###############################################################################

$(MATERIALOBJDIR)/matte.o: ./src/material/matte.cc
	$(CXX) -o $@ $<

$(MATERIALOBJDIR)/leaf0.o: ./src/material/leaf0.cc
	$(CXX) -o $@ $<



###############################################################################
# bxdf
###############################################################################

$(BXDFOBJDIR)/brdftobtdf.o: ./src/bxdf/brdftobtdf.cc
	$(CXX) -o $@ $<

$(BXDFOBJDIR)/lambertian.o: ./src/bxdf/lambertian.cc
	$(CXX) -o $@ $<

$(BXDFOBJDIR)/mirror.o: ./src/bxdf/mirror.cc
	$(CXX) -o $@ $<



###############################################################################
# accelerators
###############################################################################

$(ACCELERATORSOBJDIR)/accelerators-bvh.o: ./src/accelerators/bvh.cc
	$(CXX) -o $@ $<



###############################################################################
# volumes
###############################################################################

$(VOLUMESOBJDIR)/homogeneous.o: ./src/volume/homegeneous.cc
	$(CXX) -o $@ $<

$(VOLUMESOBJDIR)/exponential.o: ./src/volume/exponential.cc
	$(CXX) -o $@ $<

$(VOLUMESOBJDIR)/list.o: ./src/volume/list.cc
	$(CXX) -o $@ $<



###############################################################################
# initialize
###############################################################################

.PHONY: init-dir-tree
init-dir-tree:
	mkdir -p ./bin
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(ACCELERATORSOBJDIR)
	mkdir -p $(APPOBJDIR)
	mkdir -p $(BACKGROUNDSOBJDIR)
	mkdir -p $(BASICTYPESOBJDIR)
	mkdir -p $(CAMERASOBJDIR)
	mkdir -p $(INTEGRATORSOBJDIR)
	mkdir -p $(INTERACTIONOBJDIR)
	mkdir -p $(BXDFOBJDIR)
	mkdir -p $(PRIMITIVESOBJDIR)
	mkdir -p $(RENDERTARGETSOBJDIR)
	mkdir -p $(SAMPLERSOBJDIR)
	mkdir -p $(SHAPEOBJDIR)
	mkdir -p $(VOLUMESOBJDIR)
	mkdir -p $(TEXTURESOBJDIR)
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
	rm -f $(BACKGROUNDSOBJDIR)/*.o
	rm -f $(BASICTYPESOBJDIR)/*.o
	rm -f $(CAMERASOBJDIR)/*.o
	rm -f $(BXDFOBJDIR)/*.o
	rm -f $(MATERIALOBJDIR)/*.o
	rm -f $(INTEGRATORSOBJDIR)/*.o
	rm -f $(INTERACTIONOBJDIR)/*.o
	rm -f $(OBJDIR)/*.o
	rm -f $(PRIMITIVESOBJDIR)/*.o
	rm -f $(RENDERTARGETSOBJDIR)/*.o
	rm -f $(SAMPLERSOBJDIR)/*.o
	rm -f $(SHAPEOBJDIR)/*.o
	rm -f $(VOLUMESOBJDIR)/*.o
	rm -f $(TEXTURESOBJDIR)/*.o


