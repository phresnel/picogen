if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (-Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()


include_directories (. ..)
include_directories (../../../../renderer/)
include_directories (../../../../auxiliary/)


# Qt
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

include_directories (${QT_INCLUDE_DIR})


# OpenMP
find_package (OpenMP REQUIRED)
link_libraries(gomp)
add_definitions (${OpenMP_CXX_FLAGS})
