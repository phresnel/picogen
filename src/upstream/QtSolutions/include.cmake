include_directories (
        "${pwd}/qtcolortriangle-2.6_1-opensource/src/")
qt4_wrap_cpp (colorTriangleMocs
        ${pwd}/qtcolortriangle-2.6_1-opensource/src/qtcolortriangle.h)
set (sources ${sources}
        ${pwd}/qtcolortriangle-2.6_1-opensource/src/qtcolortriangle.cpp
        ${colorTriangleMocs})