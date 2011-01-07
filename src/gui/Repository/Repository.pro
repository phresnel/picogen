#-------------------------------------------------
#
# Project created by QtCreator 2011-01-05T15:39:05
#
#-------------------------------------------------

QT       += core gui
TARGET = Repository
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cc \
    package.cc \
    parametricpreset.cc \
    parametricpresetsui.cc \
    database.cc \
    parametricpresetui.cc \
    parametricpresetpopup.cc \
    entityui.cc

HEADERS  += mainwindow.hh \
    package.hh \
    parametricpreset.hh \
    parametricpresetsui.hh \
    database.hh \
    shared_ptr.hh \
    parametricpresetui.hh \
    parametricpresetpopup.hh \
    entityui.hh

FORMS    += mainwindow.ui \
    parametricpresetsui.ui \
    parametricpresetui.ui \
    parametricpresetpopup.ui \
    entityui.ui

include(../widgets/QuatschPresetEditor/quatschpreseteditor.pri)
include(../widgets/QuatschPreview/quatschpreview.pri)
include(../widgets/FilenameEdit/filenameedit.pri)
include(../CosyGui/cosyscene.pri)
