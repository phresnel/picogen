cmake_minimum_required (VERSION 2.6)
project (mkheightmap)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (--std=c++0x -O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()

# -- ... ----------------------------------------------------------------------
set (src_dir ../src)
set (lib_dir ../../../lib)

# -- search paths -------------------------------------------------------------
include_directories (${src_dir})
include_directories (${lib_dir}/libquatsch/src)
link_directories (${lib_dir}/libquatsch/bin)

# -- sources ------------------------------------------------------------------
set (sources ${sources}
        ${src_dir}/main.cc
)

# -- targets ------------------------------------------------------------------
add_executable (mkheightmap ${sources})

# -- libraries ----------------------------------------------------------------
target_link_libraries (mkheightmap quatsch)
