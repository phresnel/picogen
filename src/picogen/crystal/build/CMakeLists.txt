cmake_minimum_required (VERSION 2.6)
project (crystal)

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
        ../src               # project source folder
        ${QT_INCLUDE_DIR}    # self-explaining
        ../../../auxiliary
)

# Used Qt Modules.
#set(QT_USE_QTWEBKIT TRUE)

include(${QT_USE_FILE})



### BOOST ###
find_package(Boost COMPONENTS
                   #program_options
                   system
                   filesystem
                   REQUIRED)
link_libraries(
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  #${Boost_PROGRAM_OPTIONS_LIBRARY}
)


### OpenEXR ###
#find_package (OpenEXR REQUIRED )
#link_libraries (${OPENEXR_LIBRARIES})
#include_directories (${OPENEXR_INCLUDE_PATHS})

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

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY} # if using SDL_image, obviously
   SDLmain # Sadly not included in SDL_LIBRARY variable
)
include_directories(${SDL_INCLUDE_DIR})



# -- subs ---------------------------------------------------------------------

# -- sources ------------------------------------------------------------------
set (crystal_src_dir ../src)
include_directories (${crystal_src_dir})
set (sources ${sources}
        ${crystal_src_dir}/main.cc
        ${crystal_src_dir}/mainwindow.cc
        ${crystal_src_dir}/renderwidget.cc

        ${crystal_src_dir}/crystal_math.h

        ${crystal_src_dir}/scene.h
        ${crystal_src_dir}/scene.cc

        ${crystal_src_dir}/film.cc
        ${crystal_src_dir}/film.h

        ${crystal_src_dir}/cameras/pinhole.cc
        ${crystal_src_dir}/cameras/pinhole.h

        ${crystal_src_dir}/geoblocks/ray_tri_intersect.h

        ${crystal_src_dir}/geometry.h
        ${crystal_src_dir}/geometry/terrain2d.cc
        ${crystal_src_dir}/geometry/terrain2d.h
        ${crystal_src_dir}/geometry/terrain2d/patch.cc
        ${crystal_src_dir}/geometry/terrain2d/patch.h
        ${crystal_src_dir}/geometry/terrain2d/quadtree.cc
        ${crystal_src_dir}/geometry/terrain2d/quadtree.h

        ${crystal_src_dir}/surfaceintegrator.h
        ${crystal_src_dir}/surfaceintegrators/constant.h
        ${crystal_src_dir}/surfaceintegrators/constant.cc
        ${crystal_src_dir}/surfaceintegrators/whitted.h
        ${crystal_src_dir}/surfaceintegrators/whitted.cc

        ${crystal_src_dir}/background/mono-sky.cc
        ${crystal_src_dir}/background/mono-sky.h
        ${crystal_src_dir}/background/mono-sun.cc
        ${crystal_src_dir}/background/mono-sun.h
        ${crystal_src_dir}/background/sky.cc
        ${crystal_src_dir}/background/sky.h
        ${crystal_src_dir}/background/sun.cc
        ${crystal_src_dir}/background/sun.h
        ${crystal_src_dir}/background/atmosphere.cc
        ${crystal_src_dir}/background/atmosphere.h

        # --- redshift utah sky integration ------------------------------------
        ${crystal_src_dir}/background/utah-sky/algorithm.hh
        ${crystal_src_dir}/background/utah-sky/array.hh
        ${crystal_src_dir}/background/utah-sky/atmconstants.cc
        ${crystal_src_dir}/background/utah-sky/constants.cc
        ${crystal_src_dir}/background/utah-sky/constants.hh
        ${crystal_src_dir}/background/utah-sky/dirtydirty.cc
        ${crystal_src_dir}/background/utah-sky/geometry.hh
        #${crystal_src_dir}/background/utah-sky/K25.spectrum
        #${crystal_src_dir}/background/utah-sky/K.spectrum
        #Neta.Table
        ${crystal_src_dir}/background/utah-sky/real.hh
        ${crystal_src_dir}/background/utah-sky/rgb.hh
        ${crystal_src_dir}/background/utah-sky/setup.hh
        ${crystal_src_dir}/background/utah-sky/spectrum.cc
        ${crystal_src_dir}/background/utah-sky/spectrum.hh
        ${crystal_src_dir}/background/utah-sky/spectrum.inl.hh
        ${crystal_src_dir}/background/utah-sky/static_init.hh
        ${crystal_src_dir}/background/utah-sky/sunconstants.cc
        ${crystal_src_dir}/background/utah-sky/sunsky.cc
        ${crystal_src_dir}/background/utah-sky/sunsky.hh
        ${crystal_src_dir}/background/utah-sky/tospectrum.cc

        ${crystal_src_dir}/background/utah-sky.cc
        ${crystal_src_dir}/background/utah-sky.h
        ${crystal_src_dir}/background/utah-sun.cc
        ${crystal_src_dir}/background/utah-sun.h
        ${crystal_src_dir}/background/utah-atmosphere.cc
        ${crystal_src_dir}/background/utah-atmosphere.h
        ${crystal_src_dir}/background/noatmosphere.cc
        ${crystal_src_dir}/background/noatmosphere.h

        # --- redshift quatsch integration -------------------------------------
        ${crystal_src_dir}/quatsch-integration.cc
        ${crystal_src_dir}/quatsch-height-function.h
        ${crystal_src_dir}/heightmap.h

        # --- texturing --------------------------------------------------------
        ${crystal_src_dir}/texture/pointtoradiance.h
        ${crystal_src_dir}/texture/terrain/lerp.h
        ${crystal_src_dir}/texture/terrain/lerp.cc

        # --- builtins ---------------------------------------------------------
        ${crystal_src_dir}/builtin/terrain-texture/alpine.cc
        ${crystal_src_dir}/builtin/terrain-texture/alpine.h


        # --- pixel shaders ----------------------------------------------------
        ${crystal_src_dir}/pixel_shaders/colorbalance.h
        ${crystal_src_dir}/pixel_shaders/gamma.h
        ${crystal_src_dir}/pixel_shaders/gammaradiance.h
        ${crystal_src_dir}/pixel_shaders/scale.h
        ${crystal_src_dir}/pixel_shaders/multiply.h
        ${crystal_src_dir}/pixel_shaders/all.h


        # --- api --------------------------------------------------------------
        ${crystal_src_dir}/api/terrain.h
        ${crystal_src_dir}/api/terrain.cc
)



# -- ui -----------------------------------------------------------------------
set(forms ${forms}
        ${crystal_src_dir}/mainwindow.ui
        ${crystal_src_dir}/renderwidget.ui
)

#qt4_add_resources(resources "${crystal_src_dir}/resources.qrc")
set (sources ${sources} ${resources})

qt4_automoc (${sources})
qt4_wrap_ui (forms_h ${forms})
link_libraries (${QT_LIBRARIES})





# -- targets ------------------------------------------------------------------
add_executable (crystal  ${sources} ${forms_h})
