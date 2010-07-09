# -------------------------------------------------
# Project created by QtCreator 2010-07-09T10:51:10
# -------------------------------------------------
TARGET = BatchRenderer
DEFINES += __STDC_LIMIT_MACROS
TEMPLATE = app
CONFIG+=debug_and_release
include(../../upstream/QtSolutions/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)
include(../../upstream/QtSolutions/qtlockedfile-2.4_1-opensource/src/qtlockedfile.pri)
SOURCES += main.cc \
    batchrenderer.cc \
    renderjobwidget.cc
HEADERS += batchrenderer.hh \
    renderjobwidget.hh
FORMS += batchrenderer.ui \
    renderjobwidget.ui
LIBS += ../../redshift/lib/libpicogen.a
unix:LIBS += /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a \
    /usr/lib/libboost_system.a
windows:LIBS += -lboost_program_options \
    -lboost_filesystem \
    -lboost_system
INCLUDEPATH += ../../redshift/include

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}
