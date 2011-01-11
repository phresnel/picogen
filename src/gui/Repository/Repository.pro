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
    ../RepositorySettings/repositorysettings.cc

HEADERS  += \
        repositorymainwindow.hh \        
        shared_ptr.hh \
        optional.hh \
    ../RepositorySettings/repositorysettings.hh

FORMS    += repositorymainwindow.ui

include(../widgets/QuatschPresetEditor/quatschpreseteditor.pri)
include(../widgets/QuatschPreview/quatschpreview.pri)
include(../widgets/FilenameEdit/filenameedit.pri)
include(../CosyGui/cosyscene.pri)
include(fragment/fragment.pri)
include(repository/repository.pri)
