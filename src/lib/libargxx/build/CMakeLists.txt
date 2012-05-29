cmake_minimum_required (VERSION 2.6)
project (libargv)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (--std=c++0x -O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()

# -- ... ----------------------------------------------------------------------
set (src_dir ../src)
set (lib_dir ../../../lib)

# -- search-paths -------------------------------------------------------------
include_directories (${src_dir})

# -- sources ------------------------------------------------------------------
set (sources ${sources}
     ${src_dir}/argxx.h
     ${src_dir}/detail.h
)

set (sources_test
        ${sources}
        ${src_dir}/test.cc
)

# -- targets ------------------------------------------------------------------
add_executable (test ${sources_test})
