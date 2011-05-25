
cmake_minimum_required (VERSION 2.6)
project (picogen)

###############################################################################
# Build settings.
###############################################################################
include_directories (
        ../renderer/redshift/include/  # TODO: get rid of this line
        ../renderer/                    # TODO: get rid of this line
        ../auxiliary/
)


###############################################################################
# Redshift files.
###############################################################################

MACRO(priinclude subdir)
        set (old_pwd ${pwd})
        set (pwd "${subdir}")
        include("${subdir}/include.cmake")
        set (pwd ${old_pwd})
ENDMACRO(priinclude)

priinclude ("../renderer/redshift")

set (redshift
        "${sources}"
        ../renderer/redshift/src/main.cc
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
include_directories(${SDL_INCLUDE_DIR})



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
include_directories(${OPENEXR_INCLUDE_PATHS})


### OpenMP ###
find_package (OpenMP REQUIRED)
link_libraries(gomp)   # TODO: is there a portable one?


### LibNoise ###
find_package (Noise REQUIRED)
link_libraries(${NOISE_LIBRARY})
include_directories(${NOISE_INCLUDE_DIR})


### xyto ###
add_subdirectory (../auxiliary/xyto/build "${CMAKE_CURRENT_BINARY_DIR}/xyto")
link_libraries(libxyto)

###############################################################################
# Targets.
###############################################################################


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

add_executable (redshift  ${redshift})
add_library (libredshift ${sources})