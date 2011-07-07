#-------------------------------------------------
#
# Project created by QtCreator 2011-07-06T08:29:52
#
#-------------------------------------------------

QT       += core gui

TARGET = CrackerRenderer
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cc\
        mainwindow.cc \
    renderer.cc \
    rendertarget.cc \
    rendertargetrow.cc \
    color.cc \
    scene.cc \
    renderwidget.cc

HEADERS  += mainwindow.h \
    renderer.h \
    rendertarget.h \
    rendertargetrow.h \
    pixel.h \
    real.h \
    color.h \
    scene.h \
    renderwidget.h

FORMS    += mainwindow.ui \
    renderwidget.ui
