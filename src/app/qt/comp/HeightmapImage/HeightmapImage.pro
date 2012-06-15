#-------------------------------------------------
#
# Project created by QtCreator 2012-06-12T19:19:25
#
#-------------------------------------------------

QMAKE_CXXFLAGS += --std=c++0x
QT       += core gui

TARGET = HeightmapImage
TEMPLATE = app

INCLUDEPATH += ../../../../lib/libquatsch/src

unix:LIBS += -L../../../../lib/libquatsch/bin/ -lquatsch

SOURCES += main.cc \
        heightmapimage.cc

HEADERS  += heightmapimage.h

FORMS    += heightmapimage.ui
