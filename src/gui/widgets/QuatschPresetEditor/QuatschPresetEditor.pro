#-------------------------------------------------
#
# Project created by QtCreator 2010-11-19T09:30:17
#
#-------------------------------------------------

QT       += core gui

TARGET = QuatschPresetEditor
TEMPLATE = app

SOURCES += main.cc\
        mainwindow.cc

HEADERS  += mainwindow.hh
FORMS    += mainwindow.ui

OTHER_FILES += \
    quatschpreseteditor.pri

include(quatschpreseteditor.pri)
