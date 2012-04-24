cmake_minimum_required (VERSION 2.6)
project (libquatsch)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (--std=c++0x -O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()

# ---------- output directories -----------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)

set (src_dir ../src)
include_directories (${src_dir})


# -- sources ------------------------------------------------------------------
set (sources ${sources}
        ${src_dir}/quatsch.h
        ${src_dir}/quatsch_function.h
        ${src_dir}/DynamicVariant.h  ${src_dir}/DynamicVariant.cc

        ${src_dir}/code_iterator.h   ${src_dir}/code_iterator.cc
        ${src_dir}/quatsch_except.h

        ${src_dir}/detail/parse_primitive.h

        ${src_dir}/Typename.h
        ${src_dir}/ErrorState.cc ${src_dir}/ErrorState.h

        ${src_dir}/Template.h
        ${src_dir}/Template.cc
        ${src_dir}/template/required.h
        ${src_dir}/template/StaticType.h
        ${src_dir}/template/StaticArgument.h
        ${src_dir}/template/StaticArgument.cc
        ${src_dir}/template/StaticArgumentMeta.h
        ${src_dir}/template/StaticArgumentMeta.cc
        ${src_dir}/template/DynamicArgumentsMeta.h
        ${src_dir}/template/DynamicArgumentsMeta.cc
        ${src_dir}/template/Instantiation.h

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

        ${src_dir}/compile_phase3_program.h
        ${src_dir}/compile_phase3_program.cc
        ${src_dir}/compile.h ${src_dir}/compile.cc
)


# -- targets ------------------------------------------------------------------
add_executable (quac ${sources} ${src_dir}/main.cc)
add_library    (quatsch ${sources})
