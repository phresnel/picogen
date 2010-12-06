# -------------------------------------------------
# Project created by QtCreator 2010-03-24T09:01:06
# -------------------------------------------------
TEMPLATE = app
DEFINES += __STDC_LIMIT_MACROS
SOURCES += main.cc \
    mainwindow.cc \
    quatschsourceeditor.cc
HEADERS += mainwindow.hh \
    quatschsourceeditor.hh
FORMS += mainwindow.ui \
    quatschsourceeditor.ui
INCLUDEPATH += \
        ../../../auxiliary/ \
        ../../../renderer/
LIBS += ../../../libredshift.a \
    -lgomp \
    -lnoise \
    -lSDL \
    -lSDL_image
unix:LIBS += /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a \
    /usr/lib/libboost_system.a
windows:LIBS += -lboost_program_options \
    -lboost_filesystem \
    -lboost_system
include(../Picohelp/picohelp.pri)
include(../QuatschPreview/quatschpreview.pri)
RESOURCES += ../../SimplexGui/resources.qrc
RC_FILE += icon.rc
OTHER_FILES += icon.rc \
    QuatschSourceEditor.pri
CONFIG+=debug_and_release
TARGET=picogen-quatsch-editor

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}
