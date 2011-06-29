#-------------------------------------------------
#
# Project created by QtCreator 2011-01-13T10:07:34
#
#-------------------------------------------------
QT       += core gui

TARGET = Preview
TEMPLATE = app

SOURCES += main.cc \
        $$PWD/previewmainwindow.cc
HEADERS  += $$PWD/previewmainwindow.hh
FORMS    += previewmainwindow.ui 

include(preview.pri)