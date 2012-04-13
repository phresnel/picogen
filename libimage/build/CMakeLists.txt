cmake_minimum_required (VERSION 2.6)
project (libimage)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (--std=c++0x -O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()

set (src_dir ../src)
include_directories (${src_dir})


# -- sources ------------------------------------------------------------------
set (sources ${sources}
        ${src_dir}/main.cc
        ${src_dir}/algorithm.h
        ${src_dir}/indexer.h
        ${src_dir}/surface.h
)


# -- packages ---------------------------------------------------------------
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

### QT ###
#find_package(Qt4 REQUIRED)
#include_directories (${QT_INCLUDE_DIR})
#set(QT_USE_QTWEBKIT TRUE)
#include(${QT_USE_FILE})

find_package(Boost COMPONENTS REQUIRED)
link_libraries(
)


### OpenEXR ###
#find_package (OpenEXR REQUIRED )
#link_libraries (${OPENEXR_LIBRARIES})
#include_directories (${OPENEXR_INCLUDE_PATHS})

#find_package (OpenMP REQUIRED)
#link_libraries(gomp)
#add_definitions (${OpenMP_CXX_FLAGS})

#find_package (Noise REQUIRED)
#link_libraries(${NOISE_LIBRARY})
#include_directories(${NOISE_INCLUDE_DIR})

#find_package (SDL REQUIRED )
#find_package (SDL_image REQUIRED )
#if (NOT SDL_FOUND )
#   message ( FATAL_ERROR "SDL not found!" )
#endif (NOT SDL_FOUND )
#link_libraries (${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} SDLmain)
#include_directories(${SDL_INCLUDE_DIR})


# -- subs ---------------------------------------------------------------------
MACRO(priinclude subdir)
        set (old_pwd ${pwd})
        set (pwd "${subdir}")
        include("${subdir}/include.cmake")
        set (pwd ${old_pwd})
ENDMACRO(priinclude)

#set (widgets_dir ../../widgets)
#priinclude("../../../../upstream/QtSolutions")
#priinclude("${widgets_dir}/AppNavigation")


# -- ui -----------------------------------------------------------------------
#qt4_add_resources(resources "${cosygui_src_dir}/resources.qrc")
#set (sources ${sources} ${resources})
#qt4_automoc (${sources})
#qt4_wrap_ui (forms_h ${forms})
#link_libraries (${QT_LIBRARIES})




# -- targets ------------------------------------------------------------------
add_executable (test ${sources} ${forms_h})
