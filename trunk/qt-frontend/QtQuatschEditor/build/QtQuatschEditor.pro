# -------------------------------------------------
# Project created by QtCreator 2009-06-17T07:49:33
# -------------------------------------------------
QT += opengl
TARGET = QtQuatschEditor
TEMPLATE = app
LIBS += ../../../instant-preview/open-gl/build/libinstantpreview.a
INCLUDEPATH += ../include/ \
    ../../../ \
    ../../../instant-preview/open-gl/include/
SOURCES += ../src/main.cpp \
    ../src/qtquatscheditor.cc \
    ../src/userconstant.cc \
    ../src/predefinedconstant.cc \
    ../src/parameter.cc \
    ../src/quatsch-integration.cc \
    ../src/nodeitem.cc \
    ../src/edgeitem.cc \
    ../glwidget.cpp
HEADERS += ../include/qtquatscheditor.hh \
    ../include/userconstant.hh \
    ../include/predefinedconstant.hh \
    ../include/parameter.hh \
    ../include/nodeitem.hh \
    ../include/edgeitem.hh \
    ../include/jux_gen.hh \
    ../glwidget.h
FORMS += ../forms/qtquatscheditor.ui \
    ../forms/userconstant.ui \
    ../forms/predefinedconstant.ui \
    ../forms/parameter.ui
RESOURCES += ../resources/icons.qrc
