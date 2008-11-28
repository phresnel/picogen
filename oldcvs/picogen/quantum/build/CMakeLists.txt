
cmake_minimum_required (VERSION 2.6)
project (picogen) 

#set (CMAKE_VERBOSE_MAKEFILE true)


#########################################################################################
## #defines #############################################################################
add_definitions (-D__STDC_LIMIT_MACROS)
add_definitions (-DNO_LLVM)
add_definitions (-DPICOGEN_RELEASE -DNDEBUG)
#########################################################################################





#########################################################################################
#########################################################################################
set (OPTIMIZER_FLAGS )
macro (add_oflag flag)
    add_definitions (${flag})
endmacro (add_oflag)

if (MSVC)
    #message ("using msvc for compilation")
else (MSVC)
    #message ("using g++ for compilation")
    add_oflag   ( -Wall )
    add_oflag   ( -s )
    add_oflag   ( -O3 )
    add_oflag   ( -fexpensive-optimizations )
    add_oflag   ( -combine  )
    add_oflag   ( -mtune=generic )
    add_oflag   ( -ffast-math )
    add_oflag   ( -ftree-vectorize )
    add_oflag   ( -funroll-loops  )
    add_oflag   ( -fvariable-expansion-in-unroller )
    add_oflag   ( -funsafe-loop-optimizations )
    add_oflag   ( -ftree-loop-im )
    add_oflag   ( -funswitch-loops )
    add_oflag   ( -ftree-loop-ivcanon )
    add_oflag   ( -ftracer )
    add_oflag   ( -funsafe-math-optimizations )
    add_oflag   ( -ffinite-math-only )
    add_oflag   ( -fdata-sections )    
endif (MSVC)
#########################################################################################





#########################################################################################
## boost ################################################################################
#include (FindBoost.cmake)
#if (NOT BOOST_REGEX_FOUND)
    #message (FATAL_ERROR "boost-regex not found!")
#endif (NOT BOOST_REGEX_FOUND)
#message ("boost-regex-lib: ${BOOST_REGEX_LIBRARY}")
#link_libraries (${BOOST_REGEX_LIBRARY})

find_library (boost_regex_lib libboost_regex.a ../lib/)
link_libraries(${boost_regex_lib})
#########################################################################################





#########################################################################################
## SDL ##################################################################################
Find_Package(SDL REQUIRED)
if (NOT SDL_FOUND)
 message(FATAL_ERROR "SDL not found!")
endif (NOT SDL_FOUND)

link_libraries (
  ${SDL_LIBRARY}
  #SDLmain # Sadly not included in SDL_LIBRARY variable
)
#########################################################################################




#########################################################################################
## picogen source files #################################################################
set (CXX_SOURCEFILE_FOLDER ../src)
set (FILES )
macro (add_cxx_file filename)
    set (FILES ${FILES} ${CXX_SOURCEFILE_FOLDER}/${filename})
endmacro (add_cxx_file)

add_cxx_file (main.cc)
add_cxx_file (experimental/Rain.cc)
add_cxx_file (experimental/Snow.cc)
add_cxx_file (picosrc/generators/rng/MersenneTwister.cc)

set (FILES ${FILES} ../include/picogen/foreign/libcoyotl/mtwister.cpp)
set (FILES ${FILES} ../include/picogen/foreign/libcoyotl/prng.cpp)

add_cxx_file (picosrc/graphics/brdf/Lambertian.cc)
add_cxx_file (picosrc/graphics/brdf/Specular.cc)
add_cxx_file (picosrc/graphics/brdf/Specular_DistortedHeight.cc)

add_cxx_file (picosrc/graphics/film/DistanceVis.cc)
add_cxx_file (picosrc/graphics/film/NormalVis.cc)
add_cxx_file (picosrc/graphics/film/RayDirVis.cc)
add_cxx_file (picosrc/graphics/film/SimpleColorFilm.cc)

add_cxx_file (picosrc/graphics/integrators/image/XYIterator.cc)
add_cxx_file (picosrc/graphics/integrators/ray/Preview.cc)
add_cxx_file (picosrc/graphics/integrators/ray/Simple.cc)
add_cxx_file (picosrc/graphics/integrators/surface/Path.cc)
add_cxx_file (picosrc/graphics/integrators/surface/Whitted.cc)

add_cxx_file (picosrc/graphics/objects/AABox.cc)
add_cxx_file (picosrc/graphics/objects/CloudAdapter.cc)
add_cxx_file (picosrc/graphics/objects/ImplicitHeightField.cc)
add_cxx_file (picosrc/graphics/objects/LinearList.cc)
add_cxx_file (picosrc/graphics/objects/Preetham.cc)
add_cxx_file (picosrc/graphics/objects/QuadtreeHeightField.cc)
add_cxx_file (picosrc/graphics/objects/SimpleHeightField.cc)
add_cxx_file (picosrc/graphics/objects/Sphere.cc)
add_cxx_file (picosrc/graphics/objects/TriBIH.cc)
add_cxx_file (picosrc/graphics/objects/ZBNCubemap.cc)

add_cxx_file (picosrc/misc/functional.cc)
add_cxx_file (picosrc/misc/functional/LayeredNoise2d.cc)
add_cxx_file (picosrc/misc/functional/Layers.cc)
add_cxx_file (picosrc/misc/functional/Noise2d.cc)

add_cxx_file (picosrc/misc/scripting/AST2LLVM.cc)
add_cxx_file (picosrc/misc/scripting/nano-c.cc)
add_cxx_file (picosrc/misc/scripting/picogen-c.cc)

add_cxx_file (picossdf/picossdf.cc)

add_cxx_file (submain/mkheightmap.cc)
add_cxx_file (submain/mkskymap.cc)
#add_cxx_file (submain/seb.cc)
add_cxx_file (submain/ssdf.cc)
#add_cxx_file (submain/testscenes.cc)
#########################################################################################

add_executable (picogen ${FILES})
