#-------------------------------------------------
#
# Project created by QtCreator 2011-01-05T15:39:05
#
#-------------------------------------------------

QT       += core gui
TARGET = Repository
TEMPLATE = app


SOURCES += \
        main.cpp\
        repositorymainwindow.cc \
        collection.cc \
        parametricpreset.cc \
        database.cc

HEADERS  += \
        repositorymainwindow.hh \
        collection.hh \
        parametricpreset.hh \
        database.hh \
        shared_ptr.hh \
        optional.hh

FORMS    += repositorymainwindow.ui

include(../widgets/QuatschPresetEditor/quatschpreseteditor.pri)
include(../widgets/QuatschPreview/quatschpreview.pri)
include(../widgets/FilenameEdit/filenameedit.pri)
include(../CosyGui/cosyscene.pri)
include(fragment/fragment.pri)
