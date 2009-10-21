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
    ../src/noise2d.cc \
    ../src/layerednoise2d.cc \
    ../../common/src/quatschnode.cc
HEADERS += ../include/qtquatscheditor.hh \
    ../include/userconstant.hh \
    ../include/predefinedconstant.hh \
    ../include/parameter.hh \
    ../include/nodeitem.hh \
    ../include/edgeitem.hh \
    ../include/noise2d.hh \
    ../include/layerednoise2d.hh \
    ../../common/include/quatschnode.hh
FORMS += ../forms/qtquatscheditor.ui \
    ../forms/userconstant.ui \
    ../forms/predefinedconstant.ui \
    ../forms/parameter.ui \
    ../forms/noise2d.ui \
    ../forms/layerednoise2d.ui
RESOURCES += ../resources/icons.qrc
