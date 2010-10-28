AUTOMAKE_OPTIONS = subdir-objects
##ACLOCAL_AMFLAGS = -I m4

include $(top_srcdir)/filelist.am
include $(top_srcdir)/cosygui-filelist.am

CLEANFILES = 


AM_CXXFLAGS = -fopenmp -D__STDC_LIMIT_MACROS
AM_CCFLAGS  = -fopenmp
AM_LDFLAGS  = -lgomp

noinst_LTLIBRARIES = libxyto.la libredshift.la
#bin_PROGRAMS     = picogen-pure
bin_PROGRAMS     = picogen-pure cosygui

libxyto_la_SOURCES        = $(xytofiles)

redshiftCPPFlags = $(BOOST_CPPFLAGS) -I./auxiliary/ -I./renderer/ -I./renderer/redshift/include $(AM_CPPFLAGS)
redshiftCXXFlags = `sdl-config --cflags` -I/usr/include/OpenEXR/ -I/mingw/include/OpenEXR/ $(AM_CXXFLAGS)

libredshift_la_CPPFLAGS     = $(redshiftCPPFlags)
libredshift_la_CXXFLAGS     = $(redshiftCXXFlags)
libredshift_la_SOURCES      = $(redshiftfiles)
libredshift_la_DEPENDENCIES = libxyto.la

picogen_pure_CPPFLAGS     = $(redshiftCPPFlags)
picogen_pure_CXXFLAGS     = $(redshiftCXXFlags) 
picogen_pure_LDFLAGS      = $(LDFLAGS) \
                            $(BOOST_FILESYSTEM_LDFLAGS) \
                            $(BOOST_SYSTEM_LDFLAGS) \
                            $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
picogen_pure_SOURCES      = renderer/redshift/src/main.cc
##picogen_pure_DEPENDENCIES = 
picogen_pure_libs = \
                            .libs/libredshift.a .libs/libxyto.a \
                            $(picogen_pure_add_libs) -lSDL_image \
                           -lnoise -lz -lgomp \
                           -lIlmImf -lImath -lHalf -lIex -lIlmThread \
                           $(BOOST_FILESYSTEM_LIBS) \
                           $(BOOST_SYSTEM_LIBS) \
                           $(BOOST_PROGRAM_OPTIONS_LIBS)
picogen_pure_LDADD        = $(picogen_pure_libs) $(LDADD)



###############################################################################
## Qt specific rules
###############################################################################
moc_%.cpp: %.hh
	$(MOC) $(QT_CPPFLAGS) $< -o $@

qrc_%.cpp: %.hh
	$(RCC) -name `echo "$<" | sed 's/\.qrc$$//'` $< -o $@

ui_%.h: %.ui
	$(UIC) $< -o $@


#%($EXEEXT): %.pro
#	$(QMAKE) $<

#gui/%/Makefile: gui/%/CosyGui.pro
#	$(QMAKE) $< -o $@
###############################################################################


###############################################################################
## Frontend
###############################################################################

BUILT_SOURCES  = $(COSY_BUILT_SOURCES)
CLEANFILES    += $(COSY_BUILT_SOURCES)

cosygui_SOURCES = $(COSY_SOURCES)
cosygui_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
cosygui_CPPFLAGS = $(QT_CPPFLAGS) $(COSY_CPPFLAGS) $(AM_CPPFLAGS)
cosygui_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
cosygui_LDADD    = $(QT_LIBS) $(picogen_pure_libs) $(LDADD)
