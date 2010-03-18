#-------------------------------------------------
#
# Project created by QtCreator 2010-03-18T07:25:16
#
#-------------------------------------------------

TARGET = test-import
TEMPLATE = app

DEFINES += __STDC_LIMIT_MACROS

SOURCES += main.cc\
        mainwindow.cc

HEADERS  += mainwindow.hh

FORMS    += mainwindow.ui

include(../../qtwindowlistmenu-2.2_1-opensource/src/qtwindowlistmenu.pri)
include(../../qtpropertybrowser-2.5_1-opensource/src/qtpropertybrowser.pri)
