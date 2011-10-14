cmake_minimum_required (VERSION 2.6)
project (cracker)

set(CMAKE_BUILD_TYPE Release)

if (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (# language
                         -std=c++0x

                         # optimization
                         -O3
                         -ffast-math
                         -msse -mfpmath=sse
                         -Wdisabled-optimization
                         -march=native

                         # diagnosis
                         -g

                         # warnings
                         -Wall -Wextra -D__STDC_LIMIT_MACROS)

        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p -fprofile-generate")
        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use")

endif()


set(aux_dir ../../../auxiliary)


#include_directories (../../../)
#include_directories (../../../redshift/include)
include_directories (${aux_dir})




# -- openmp package -----------------------------------------------------------
find_package (OpenMP REQUIRED)
link_libraries(gomp)
add_definitions (${OpenMP_CXX_FLAGS})




# -- packages ---------------------------------------------------------------
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

### QT ###
find_package(Qt4 REQUIRED)

# Include dirs.
include_directories (
        ./                   # for generated stuff
        ../                  # project source folder
        ${QT_INCLUDE_DIR}    # self-explaining
)

# Used Qt Modules.
#set(QT_USE_QTWEBKIT TRUE)

include(${QT_USE_FILE})



### BOOST ###
find_package(Boost COMPONENTS
                   #program_options
                   #system
                   #filesystem
                   REQUIRED)
link_libraries(
  #${Boost_FILESYSTEM_LIBRARY}
  #${Boost_SYSTEM_LIBRARY}
  #${Boost_PROGRAM_OPTIONS_LIBRARY}
)


### OpenEXR ###
find_package (OpenEXR REQUIRED )
link_libraries (${OPENEXR_LIBRARIES})
include_directories (${OPENEXR_INCLUDE_PATHS})

### OpenMP ###
find_package (OpenMP REQUIRED)
link_libraries(gomp)   # TODO: is there a portable one?


### LibNoise ###
find_package (Noise REQUIRED)
link_libraries(${NOISE_LIBRARY})
include_directories(${NOISE_INCLUDE_DIR})

### SDL ###
find_package (SDL REQUIRED )
find_package (SDL_image REQUIRED )
if (NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif (NOT SDL_FOUND )

#link_libraries (
#   ${SDL_LIBRARY}
#   ${SDLIMAGE_LIBRARY} # if using SDL_image, obviously
#   SDLmain # Sadly not included in SDL_LIBRARY variable
#)
#include_directories(${SDL_INCLUDE_DIR})



# -- subs ---------------------------------------------------------------------

# -- sources ------------------------------------------------------------------
set (cracker_src_dir ../src)
include_directories (${cracker_src_dir})
set (sources ${sources}
        ${cracker_src_dir}/color.cc
        ${cracker_src_dir}/main.cc
        ${cracker_src_dir}/mainwindow.cc
        ${cracker_src_dir}/render.cc
        ${cracker_src_dir}/rendertarget.cc
        ${cracker_src_dir}/rendertargetrow.cc
        ${cracker_src_dir}/renderwidget.cc
        ${cracker_src_dir}/scene.cc

        ${cracker_src_dir}/real.cc
        ${cracker_src_dir}/constants.cc
        ${cracker_src_dir}/interval.cc
        ${cracker_src_dir}/math3d.cc
        ${cracker_src_dir}/algorithm.cc
        ${cracker_src_dir}/random.cc

        ${cracker_src_dir}/ray.cc
        ${cracker_src_dir}/material.cc

        ${cracker_src_dir}/background/sun.cc
        ${cracker_src_dir}/background/sky.cc
        ${cracker_src_dir}/background/utah-sky.cc
        ${cracker_src_dir}/background/mono-sky.cc
        ${cracker_src_dir}/background/utah-sun.cc
        ${cracker_src_dir}/background/mono-sun.cc
        ${cracker_src_dir}/background/utah-sky/dirtydirty.cc

        ${cracker_src_dir}/math3d/vector.cc
        ${cracker_src_dir}/math3d/point.cc
        ${cracker_src_dir}/math3d/direction.cc
        ${cracker_src_dir}/math3d/vector.inl.cc
        ${cracker_src_dir}/math3d/point.inl.cc
        ${cracker_src_dir}/math3d/direction.inl.cc
        ${cracker_src_dir}/math3d/normal.inl.cc
        ${cracker_src_dir}/aabb.cc
        ${cracker_src_dir}/intersection.cc
        ${cracker_src_dir}/differentialgeometry.cc

        ${cracker_src_dir}/cameras/pinhole.cc

        ${cracker_src_dir}/volumeintegrators/volumeintegrator.cc

        ${cracker_src_dir}/surfaceintegrators/path.cc
        ${cracker_src_dir}/surfaceintegrators/whitted.cc
        ${cracker_src_dir}/surfaceintegrators/raydirection.cc
        ${cracker_src_dir}/surfaceintegrators/primarydistance.cc
        ${cracker_src_dir}/surfaceintegrators/surfacenormal.cc
        ${cracker_src_dir}/surfaceintegrators/cpucore.cc
        ${cracker_src_dir}/surfaceintegrators/combiner.cc
        ${cracker_src_dir}/surfaceintegrators/interlacer.cc
        ${cracker_src_dir}/surfaceintegrators/nameof.cc

        ${cracker_src_dir}/primitives/primitive.cc
        ${cracker_src_dir}/primitives/sphere.cc

        ${cracker_src_dir}/materials/lambertmaterial.cc
        ${cracker_src_dir}/materials/specularmirror.cc

        ${cracker_src_dir}/terrain/gridterrain.cc
        ${cracker_src_dir}/terrain/quadtree.cc
        ${cracker_src_dir}/terrain/quadtree/patch.cc

        ${cracker_src_dir}/presentationfilter.cc
)



# -- ui -----------------------------------------------------------------------
set(forms ${forms}
        ${cracker_src_dir}/mainwindow.ui
        ${cracker_src_dir}/renderwidget.ui
)

#qt4_add_resources(resources "${cracker_src_dir}/resources.qrc")
set (sources ${sources} ${resources})

qt4_automoc (${sources})
qt4_wrap_ui (forms_h ${forms})
link_libraries (${QT_LIBRARIES})





# -- targets ------------------------------------------------------------------
add_executable (cracker  ${sources} ${forms_h})
