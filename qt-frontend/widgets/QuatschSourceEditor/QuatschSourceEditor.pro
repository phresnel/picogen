# -------------------------------------------------
# Project created by QtCreator 2010-03-24T09:01:06
# -------------------------------------------------
TARGET = QuatschSourceEditor
TEMPLATE = app
DEFINES += __STDC_LIMIT_MACROS
SOURCES += main.cc \
    mainwindow.cc \
    quatschsourceeditor.cc
HEADERS += mainwindow.hh \
    quatschsourceeditor.hh
FORMS += mainwindow.ui \
    quatschsourceeditor.ui
INCLUDEPATH += ../../../ \
    ../../../redshift/include
LIBS += ../../../redshift/lib/libpicogen.a \
    -lgomp \
    -lboost_program_options \
    -lSDL \
    -lSDL_image \
    -lboost_filesystem \
    -lboost_system \
    -lnoise
include(../Picohelp/picohelp.pri)
RESOURCES += ../../SimplexGui/resources.qrc
