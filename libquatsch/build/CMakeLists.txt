cmake_minimum_required (VERSION 2.6)
project (quatsch-compiler)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (--std=c++0x -O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()

set (src_dir ../src)
include_directories (${src_dir})


# -- sources ------------------------------------------------------------------
set (sources ${sources}
        ${src_dir}/main.cc
        ${src_dir}/code_iterator.h   ${src_dir}/code_iterator.cc
        ${src_dir}/quatsch_except.h

        ${src_dir}/detail/parse_primitive.h

        ${src_dir}/Typename.h

        ${src_dir}/Template.h
        ${src_dir}/Template.cc
        ${src_dir}/template/required.h
        ${src_dir}/template/StaticType.h
        ${src_dir}/template/StaticArgument.h
        ${src_dir}/template/StaticArgument.cc
        ${src_dir}/template/StaticArgumentMeta.h
        ${src_dir}/template/StaticArgumentMeta.cc
        ${src_dir}/template/Instantiation.h
        ${src_dir}/template/Instantiation.cc

        ${src_dir}/phase1/Token.h    ${src_dir}/phase1/Token.cc
        ${src_dir}/phase1/Toque.h    ${src_dir}/phase1/Toque.cc
        ${src_dir}/phase1/tokenize.h ${src_dir}/phase1/tokenize.cc

        ${src_dir}/phase2/Program.h  ${src_dir}/phase2/Program.cc
        ${src_dir}/phase2/Tree.h     ${src_dir}/phase2/Tree.cc
        ${src_dir}/phase2/Builtin.h  ${src_dir}/phase2/Builtin.cc
        ${src_dir}/phase2/Defun.h    ${src_dir}/phase2/Defun.cc
        ${src_dir}/phase2/Constant.h ${src_dir}/phase2/Constant.cc
        ${src_dir}/phase2/Argument.h ${src_dir}/phase2/Argument.cc

        ${src_dir}/phase2/parse.h    ${src_dir}/phase2/parse.cc

        ${src_dir}/phase3/Program.cc ${src_dir}/phase3/Program.h
        ${src_dir}/phase3/Tree.cc    ${src_dir}/phase3/Tree.h
        ${src_dir}/phase3/Defun.h    ${src_dir}/phase3/Defun.cc
        ${src_dir}/phase3/Constant.h ${src_dir}/phase3/Constant.cc
        ${src_dir}/phase3/resolve_and_verify.h
        ${src_dir}/phase3/resolve_and_verify.cc

        ${src_dir}/phase5/C99/to_C99.h ${src_dir}/phase5/C99/to_C99.cc

        ${src_dir}/phase5/callable/to_callable.h
        ${src_dir}/phase5/callable/to_callable.cc

)


# -- packages ---------------------------------------------------------------
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

### QT ###
#find_package(Qt4 REQUIRED)
#include_directories (${QT_INCLUDE_DIR})
#set(QT_USE_QTWEBKIT TRUE)
#include(${QT_USE_FILE})

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
link_libraries(
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
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
add_executable (quac ${sources} ${forms_h})
