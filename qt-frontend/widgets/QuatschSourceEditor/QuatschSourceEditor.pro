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
LIBS += ../../redshift/lib/libpicogen.a \
    -lgomp \
    -lnoise \
    -lSDL \
    -lSDL_image
unix{
    LIBS += \
        /usr/lib/libboost_program_options.a \
        /usr/lib/libboost_filesystem.a  \
        /usr/lib/libboost_system.a
}
windows {
    LIBS += \
        -lboost_program_options \
        -lboost_filesystem  \
        -lboost_system
}

include(../Picohelp/picohelp.pri)
RESOURCES += ../../SimplexGui/resources.qrc
