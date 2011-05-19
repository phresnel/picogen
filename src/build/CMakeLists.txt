
cmake_minimum_required (VERSION 2.6)
project (picogen)


###############################################################################
# Redshift files.
###############################################################################
set (redshiftBaseDir ../renderer/redshift)
set (redshiftLibFiles
        ${redshiftBaseDir}/src/constants.cc
        ${redshiftBaseDir}/src/jobfile.cc
        ${redshiftBaseDir}/src/quatsch-integration.cc
        ${redshiftBaseDir}/src/get_compilation_info.cc
        ${redshiftBaseDir}/src/save_openexr.cc

        ${redshiftBaseDir}/src/backgrounds/pss-adapter.cc
        ${redshiftBaseDir}/src/backgrounds/preetham-shirley-smits/atmconstants.cc
        ${redshiftBaseDir}/src/backgrounds/preetham-shirley-smits/sunconstants.cc
        ${redshiftBaseDir}/src/backgrounds/preetham-shirley-smits/sunsky.cc
        ${redshiftBaseDir}/src/backgrounds/preetham-shirley-smits/tospectrum.cc

        ${redshiftBaseDir}/src/basictypes/bsdf.cc
        ${redshiftBaseDir}/src/basictypes/differentialgeometry.cc
        ${redshiftBaseDir}/src/basictypes/intersection.cc
        ${redshiftBaseDir}/src/basictypes/material.cc
        ${redshiftBaseDir}/src/basictypes/scene.cc
        ${redshiftBaseDir}/src/basictypes/spectrum.cc
        ${redshiftBaseDir}/src/basictypes/volume.cc

        ${redshiftBaseDir}/src/bxdf/brdftobtdf.cc
        ${redshiftBaseDir}/src/bxdf/lambertian.cc
        ${redshiftBaseDir}/src/bxdf/mirror.cc

        ${redshiftBaseDir}/src/cameras/cubemapface.cc
        ${redshiftBaseDir}/src/cameras/cylindrical.cc
        ${redshiftBaseDir}/src/cameras/pinhole.cc

        ${redshiftBaseDir}/src/integrators/emission.cc
        ${redshiftBaseDir}/src/integrators/path.cc
        ${redshiftBaseDir}/src/integrators/redshift.cc
        ${redshiftBaseDir}/src/integrators/single-scattering.cc
        ${redshiftBaseDir}/src/integrators/whitted.cc

        ${redshiftBaseDir}/src/interaction/passivecommandprocessor.cc
        ${redshiftBaseDir}/src/interaction/sdlcommandprocessor.cc

        ${redshiftBaseDir}/src/material/leaf0.cc
        ${redshiftBaseDir}/src/material/matte.cc

        ${redshiftBaseDir}/src/primitives/boundinstance.cc
        ${redshiftBaseDir}/src/primitives/bvh.cc
        ${redshiftBaseDir}/src/primitives/closedsphere.cc
        ${redshiftBaseDir}/src/primitives/forest.cc
        ${redshiftBaseDir}/src/primitives/horizonplane.cc
        ${redshiftBaseDir}/src/primitives/instance.cc
        ${redshiftBaseDir}/src/primitives/lazyquadtree.cc
        ${redshiftBaseDir}/src/primitives/list.cc
        ${redshiftBaseDir}/src/primitives/lsystemtree.cc
        ${redshiftBaseDir}/src/primitives/triangle.cc
        ${redshiftBaseDir}/src/primitives/trianglebvh.cc
        ${redshiftBaseDir}/src/primitives/waterplane.cc

        ${redshiftBaseDir}/src/redshift_file/object.cc
        ${redshiftBaseDir}/src/redshift_file/save_load.cc
        ${redshiftBaseDir}/src/redshift_file/scene.cc
        ${redshiftBaseDir}/src/redshift_file/to_redshift/object_to_redshift.cc
        ${redshiftBaseDir}/src/redshift_file/to_redshift/redshift_file_to_redshift.cc

        ${redshiftBaseDir}/src/rendertargets/colorrendertarget.cc
        ${redshiftBaseDir}/src/rendertargets/sdlrendertarget.cc

        ${redshiftBaseDir}/src/shapes/closedsphere.cc

        ${redshiftBaseDir}/src/texture/image.cc

        ${redshiftBaseDir}/src/volume/exponential.cc
        ${redshiftBaseDir}/src/volume/homegeneous.cc
        ${redshiftBaseDir}/src/volume/list.cc
)
set (redshift
        ${redshiftLibFiles}
        ${redshiftBaseDir}/src/main.cc
)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})



###############################################################################
# External libraries.
###############################################################################

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



### BOOST ###
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
link_libraries(
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)


### OpenEXR ###
find_package (OpenEXR REQUIRED )
link_libraries (${OPENEXR_LIBRARIES})


### OpenMP ###
find_package (OpenMP REQUIRED)
link_libraries(gomp)   # TODO: is there a portable one?


### LibNoise ###
find_package (Noise REQUIRED)
link_libraries(${NOISE_LIBRARY})   # TODO: is there a portable one?




###############################################################################
# Build settings.
###############################################################################
#message (STATUS "foo: " ${NOISE_INCLUDE_DIR})
#message (STATUS "xyto files: " ${xytoFiles})

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (-O3
                         -ffast-math -msse -mfpmath=sse

                        #-Wpadded # when GCC puts pad-variables into structures. reordering fields might then be beneficial
                        -Wdisabled-optimization # when GCC was inable
                        #-Wsuggest-attribute=pure
                        #-Wsuggest-attribute=const
                        #-Wsuggest-attribute=noreturn

                         ${OpenMP_CXX_FLAGS}
                         -Wall -Wextra -Winit-self
                         -D__STDC_LIMIT_MACROS
                        )
else()
        message (STATUS "no special flags set for compiler")
endif()

include_directories (
        ${redshiftBaseDir}/include/  # TODO: get rid of this line
        ../renderer/                    # TODO: get rid of this line
        ../auxiliary/
        ${SDL_INCLUDE_DIR}
        ${OPENEXR_INCLUDE_PATHS}
        ${NOISE_INCLUDE_DIR}
)

add_subdirectory (../auxiliary/xyto/build "${CMAKE_CURRENT_BINARY_DIR}/xyto")
link_libraries(libxyto)

add_executable (redshift  ${redshift})
add_library (libredshift ${redshiftLibFiles})