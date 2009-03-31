#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (C) 2009  Sebastian Mach (*1983)
# * mail: phresnel/at/gmail/dot/com
# * http:#phresnel.org
# * http:#picogen.org
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


######################################################################
# DO NOT AUTOGENERATE THIS FILE
######################################################################


TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .



# Camera Settings Window
SOURCES += ../src/camerasettings.cc
HEADERS += ../include/camerasettings.hh
FORMS += ../ui/camerasettings.ui



# Heightmap Designer
SOURCES += ../src/heightmap-designer.cc
HEADERS += ../include/heightmap-designer.hh
FORMS += ../ui/heightmap-designer.ui



# Main Window
SOURCES += ../src/picogenqt.cc
HEADERS += ../include/picogenqt.hh
FORMS += ../ui/picogenqt.ui



# QImageRenderTarget
SOURCES += ../src/qimagerendertarget.cc
HEADERS += ../include/qimagerendertarget.hh



# main
SOURCES += ../src/main.cc



# sdl-config --cflags
INCLUDEPATHS += /usr/include/SDL
DEFINES += GNU_SOURCE=1 REENTRANT
LIBS += `sdl-config --static-libs` 

 

# ...
RESOURCES += ../resources/resources.qrc
LIBS += -L../../redshift/lib -lpicogen 
