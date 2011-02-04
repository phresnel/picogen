#-------------------------------------------------
#
# Project created by QtCreator 2011-01-27T10:56:22
#
#-------------------------------------------------

QT       += core gui

TARGET = GraphicalNavigation
TEMPLATE = app

SOURCES += main.cc\
        mainwindow.cc
HEADERS  += mainwindow.hh
FORMS    += mainwindow.ui

OTHER_FILES += \
    graphicalnavigationwidget.pri

include(graphicalnavigationwidget.pri)
