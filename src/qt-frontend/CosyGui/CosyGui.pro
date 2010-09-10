#-------------------------------------------------
#
# Project created by QtCreator 2010-08-27T13:22:12
#
#-------------------------------------------------

QT += core gui
DEFINES += __STDC_LIMIT_MACROS
CONFIG += debug_and_release
QMAKE_CXXFLAGS += -fopenmp
INCLUDEPATH += \
        ../../upstream/qt-widgets/pictureflow-0.1.0/pictureflow-qt/ \
        ../widgets/ \
        ../widgets/QuatschSourceEditor/ \
        ../widgets/SimpleInputBox/ \
        ../widgets/RenderWindow/ \
        ../../

TARGET = CosyGui
TEMPLATE = app


SOURCES += main.cc\
        mainwindow.cc \
    filmsettings.cc \
    cosyscene/terrain.cc \
    terrainwindow.cc \
    cosyscene/scene.cc \
    cosyscene/save_load.cc \
    stashview.cc \
    stashframe.cc \
    sunskywindow.cc \
    cosyscene/sunsky.cc \
    cosyscene/geometry.cc \
    cosyscene/vector3d.cc \
    cosyscene/direction3d.cc

HEADERS  += mainwindow.hh \
    filmsettings.hh \
    cosyscene/stash.hh \
    cosyscene/terrain.hh \
    terrainwindow.hh \
    cosyscene/terrain.serialization.hh \
    cosyscene/scene.hh \
    cosyscene/serialization/stash.ser.hh \
    cosyscene/serialization/scene.ser.hh \
    cosyscene/serialization/terrain.ser.hh \
    cosyscene/save_load.hh \
    stashview.hh \
    stashframe.hh \
    sunskywindow.hh \
    cosyscene/sunsky.hh \
    cosyscene/serialization/sunsky.ser.hh \
    cosyscene/serialization/vector3d.ser.hh \
    cosyscene/geometry.hh \
    cosyscene/serialization/geometry.ser.hh \
    cosyscene/serialization/vector3d.ser.hh \
    cosyscene/serialization/direction3d.ser.hh \
    cosyscene/vector3d.hh \
    cosyscene/direction3d.hh

FORMS    += mainwindow.ui \
    filmsettings.ui \
    terrainwindow.ui \
    stashview.ui \
    stashframe.ui \
    sunskywindow.ui

LIBS += \
    ../../redshift/lib/libpicogen.a \
    -lgomp \
    -lnoise \
    -lSDL -lSDL_image

LIBS += -lpthread -lIlmImf -lz -lImath -lHalf -lIex -lIlmThread
unix:LIBS += \
    /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a \
    /usr/lib/libboost_system.a
windows:LIBS += \
    -lboost_program_options \
    -lboost_filesystem \
    -lboost_system

RESOURCES += \
    resources.qrc

include(../widgets/QuatschSourceEditor/QuatschSourceEditor.pri)
include(../widgets/Picohelp/picohelp.pri)
include(../widgets/QtSunSkyEditor/qtsunskyeditor.pri)

CONFIG(debug, debug|release):DESTDIR = debug
else:DESTDIR = release
