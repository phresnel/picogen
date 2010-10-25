AUTOMAKE_OPTIONS = subdir-objects
##ACLOCAL_AMFLAGS = -I m4

include $(top_srcdir)/filelist.am


AM_CXXFLAGS = -fopenmp -D__STDC_LIMIT_MACROS
AM_CCFLAGS  = -fopenmp
AM_LDFLAGS  = -lgomp

noinst_LTLIBRARIES = libxyto.la libredshift.la
bin_PROGRAMS     = picogen-pure qttest
##bin_PROGRAMS     = qttest

libxyto_la_SOURCES        = $(xytofiles)

redshiftCPPFlags = $(AM_CPPFLAGS) -I./auxiliary/ -I./renderer/ -I./renderer/redshift/include
libredshift_la_CPPFLAGS     = -DPICOGENLIB $(redshiftCPPFlags) $(AM_CPPFLAGS)
libredshift_la_CXXFLAGS     = `sdl-config --cflags` -I/usr/include/OpenEXR/ -I/mingw/include/OpenEXR/ $(AM_CXXFLAGS)
libredshift_la_SOURCES      = $(redshiftfiles)
libredshift_la_DEPENDENCIES = libxyto.la

picogen_pure_CPPFLAGS     = $(redshiftCPPFlags) $(BOOST_CPPFLAGS) $(AM_CPPFLAGS)
picogen_pure_CXXFLAGS     = $(libredshift_la_CXXFLAGS) 
picogen_pure_LDFLAGS      = $(LDFLAGS) \
                            $(BOOST_FILESYSTEM_LDFLAGS) \
                            $(BOOST_SYSTEM_LDFLAGS) \
                            $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
picogen_pure_SOURCES      = $(redshiftfiles) $(xytofiles)
picogen_pure_DEPENDENCIES = libxyto.la
picogen_pure_LDADD        = $(LDADD) \
                            $(picogen_pure_libs) \
                           -lSDL_image -lnoise -lz \
                           -lIlmImf -lImath -lHalf -lIex -lIlmThread \
                           $(BOOST_FILESYSTEM_LIBS) \
                           $(BOOST_SYSTEM_LIBS) \
                           $(BOOST_PROGRAM_OPTIONS_LIBS) \
                           libxyto.la

                           

##include $(top_srcdir)/m4/autotroll.mk
moc_%.cpp: %.hh
	$(MOC) $(QT_CPPFLAGS) $< -o $@        
ui_%.h: %.ui
	$(UIC) $< -o $@
ui_%.cpp: %.ui
	$(UIC) -impl $*.hh $< -o $@

qttest_SOURCES = aqttest/main.cc \
		 aqttest/mainwindow.cc \
		 aqttest/mainwindow.hh \
		 aqttest/moc_mainwindow.cpp
qttest_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
qttest_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
qttest_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
qttest_LDADD    = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = aqttest/moc_mainwindow.cpp aqttest/ui_mainwindow.h 
DISTCLEANFILES = BUILT_SOURCES $(DISTCLEANFILES)

##--------