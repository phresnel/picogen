AC_INIT(picogen, 0.4, phresnel@gmail.com)
AC_CONFIG_SRCDIR(renderer/redshift/src/main.cc)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([m4])

AM_INIT_AUTOMAKE

m4_include([m4/boost.m4])
m4_include([m4/autotroll.m4])


AC_SUBST(picogenpure_add_libs, `sdl-config --static-libs`)
#AC_CONFIG_SUBDIRS([gui/CosyGuiAutomakeProxy])

AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AT_WITH_QT([+webkit])
AT_REQUIRE_QT_VERSION([4.6])

BOOST_REQUIRE([1.40.0])
BOOST_FILESYSTEM
BOOST_SYSTEM
BOOST_PROGRAM_OPTIONS
BOOST_SMART_PTR
BOOST_OPTIONAL

AC_OUTPUT(Makefile)

