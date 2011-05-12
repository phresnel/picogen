
cmake_minimum_required (VERSION 2.6)
project (xyto)

set (xytoFiles
        compiler.cc
        interpreter.cc
        lsystem.cc
        parameter.cc
        parameterlist.cc
        parse_expr.cc
        pattern.cc
        production.cc
        production_body.cc
        production_header.cc
        segment.cc
        token.cc
        tokenize.cc
        xyto_ios.cc
)


add_definitions (-Wall -Wextra -D__STDC_LIMIT_MACROS)
include_directories (
        ../                                # auxiliary
        ../../renderer/redshift/include/   # redshift
)

add_executable (xyto  ${xytoFiles} main.cpp)
