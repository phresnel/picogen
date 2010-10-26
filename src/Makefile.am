AUTOMAKE_OPTIONS = subdir-objects
##ACLOCAL_AMFLAGS = -I m4

include $(top_srcdir)/filelist.am

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
ui_%.h: %.ui
	$(UIC) $< -o $@
#%($EXEEXT): %.pro
#	$(QMAKE) $<

#gui/%/Makefile: gui/%/CosyGui.pro
#	$(QMAKE) $< -o $@
###############################################################################

#SUBDIRS = gui/CosyGuiAutomakeProxy/
#cosygui_DEPENDENCIES = gui/CosyGui/Makefile
#cosygui_SOURCES = 

###############################################################################
## Frontend
###############################################################################
QTFRONTEND_SOURCEFILES = \
	gui/CosyGui/main.cc \
	gui/CosyGui/mainwindow.cc \
	gui/CosyGui/cosyscene/terrain.cc \
	gui/CosyGui/terrainwindow.cc \
	gui/CosyGui/cosyscene/scene.cc \
	gui/CosyGui/cosyscene/save_load.cc \
	gui/CosyGui/stashview.cc \
	gui/CosyGui/stashframe.cc \
	gui/CosyGui/sunskywindow.cc \
	gui/CosyGui/cosyscene/sunsky.cc \
	gui/CosyGui/cosyscene/geometry.cc \
	gui/CosyGui/cosyscene/vector3d.cc \
	gui/CosyGui/cosyscene/direction3d.cc \
	gui/CosyGui/navigationwindow.cc \
	gui/CosyGui/cosyscene/navigation.cc \
	gui/CosyGui/cosyscene/point3d.cc \
	gui/CosyGui/terrainformation.cc \
	gui/CosyGui/terraintexturing.cc \
	gui/CosyGui/cosyscene/color.cc \
	gui/CosyGui/cosyscene/rendersettings.cc \
	gui/CosyGui/filmsettingswindow.cc \
	gui/CosyGui/twinrendersettingswindow.cc \
	gui/CosyGui/rendersettingswindow.cc

QTFRONTEND_MOC_FILES = \
	gui/CosyGui/moc_mainwindow.cpp \
	gui/CosyGui/moc_terrainwindow.cpp \
	gui/CosyGui/moc_stashview.cpp \
	gui/CosyGui/moc_stashframe.cpp \
	gui/CosyGui/moc_sunskywindow.cpp \
	gui/CosyGui/moc_navigationwindow.cpp \
	gui/CosyGui/moc_terrainformation.cpp \
	gui/CosyGui/moc_terraintexturing.cpp \
	gui/CosyGui/moc_filmsettingswindow.cpp \
	gui/CosyGui/moc_twinrendersettingswindow.cpp \
	gui/CosyGui/moc_rendersettingswindow.cpp

QTFRONTEND_UI_FILES = \
	gui/CosyGui/ui_mainwindow.h \
	gui/CosyGui/ui_terrainwindow.h \
	gui/CosyGui/ui_stashview.h \
	gui/CosyGui/ui_stashframe.h \
	gui/CosyGui/ui_sunskywindow.h \
	gui/CosyGui/ui_navigationwindow.h \
	gui/CosyGui/ui_terrainformation.h \
	gui/CosyGui/ui_terraintexturing.h \
	gui/CosyGui/ui_filmsettingswindow.h \
	gui/CosyGui/ui_twinrendersettingswindow.h \
	gui/CosyGui/ui_rendersettingswindow.h

BUILT_SOURCES  = $(QTFRONTEND_UI_FILES) $(QTFRONTEND_MOC_FILES) 
CLEANFILES     += $(BUILT_SOURCES)

cosygui_SOURCES = $(QTFRONTEND_SOURCEFILES) \
		 $(QTFRONTEND_MOC_FILES)
cosygui_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
cosygui_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
cosygui_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
cosygui_LDADD    = $(QT_LIBS) $(picogen_pure_libs) $(LDADD)
