# -------------------------------------------------
# Project created by QtCreator 2010-03-24T09:01:06
# -------------------------------------------------
TARGET = picogen-quatsch-editor
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
    -lSDL \
    -lSDL_image \
    -lnoise \
    /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a  \
    /usr/lib/libboost_system.a
include(../Picohelp/picohelp.pri)
RESOURCES += ../../SimplexGui/resources.qrc
