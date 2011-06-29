#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T14:49:44
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = qtirr
TEMPLATE = app


LIBS += -lIrrlicht

SOURCES += main.cc\
        mainwindow.cc \
    irrlichtrenderer.cc \
    #QIrrWidget.cpp

HEADERS  += mainwindow.hh \
    irrlichtrenderer.hh \
    #QIrrWidget.h

FORMS    += mainwindow.ui \
    irrlichtrenderer.ui
