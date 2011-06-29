#-------------------------------------------------
#
# Project created by QtCreator 2010-11-19T09:30:17
#
#-------------------------------------------------

QT       += core gui

TARGET = QuatschPresetEditor
TEMPLATE = app

SOURCES += main.cc\
        mainwindow.cc \
    ../../CosyGui/cosyscene/terrain.cc \
    ../../CosyGui/cosyscene/save_load.cc \
    ../../CosyGui/cosyscene/rendersettings.cc \
    ../../CosyGui/cosyscene/point3d.cc \
    ../../CosyGui/cosyscene/navigation.cc \
    ../../CosyGui/cosyscene/geometry.cc \
    ../../CosyGui/cosyscene/direction3d.cc \
    ../../CosyGui/cosyscene/color.cc \
    ../../CosyGui/cosyscene/vector3d.cc \
    ../../CosyGui/cosyscene/sunsky.cc \
    ../../CosyGui/cosyscene/scene.cc

HEADERS  += mainwindow.hh
FORMS    += mainwindow.ui

OTHER_FILES += \
    quatschpreseteditor.pri

include(quatschpreseteditor.pri)
include(../QuatschPreview/quatschpreview.pri)
include(../FilenameEdit/filenameedit.pri)