AC_INIT(picogen, 0.4, phresnel@gmail.com)
AC_CONFIG_SRCDIR(renderer/redshift/src/main.cc)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([m4])

AM_INIT_AUTOMAKE

m4_include([m4/boost.m4])
m4_include([m4/autotroll.m4])


#AC_SUBST(picogenpure_add_libs, `sdl-config --libs`)
#AC_CONFIG_SUBDIRS([gui/CosyGuiAutomakeProxy])

AM_SILENT_RULES([yes])

AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AT_WITH_QT([+webkit])
AT_REQUIRE_QT_VERSION([4.6])


AC_CHECK_LIB([SDL], [SDL_Init], , AC_MSG_ERROR([
*** Unable to find SDL libary
(http://www.libsdl.org/)
]))

AC_CHECK_LIB(SDL_image, IMG_LoadPNG_RW, , AC_MSG_ERROR([
*** Unable to find SDL_image libary with PNG support
(http://www.libsdl.org/projects/SDL_image/)
]))

#TODO: below, and generally completize library requirements
#AC_CHECK_LIB(IlmImf
#AC_CHECK_LIB(Imath
#AC_CHECK_LIB(Half
#AC_CHECK_LIB(Iex
#AC_CHECK_LIB(IlmThread

BOOST_REQUIRE([1.40.0])
BOOST_FILESYSTEM
BOOST_SYSTEM
BOOST_PROGRAM_OPTIONS
BOOST_SMART_PTR
BOOST_OPTIONAL

AC_OUTPUT(Makefile)

