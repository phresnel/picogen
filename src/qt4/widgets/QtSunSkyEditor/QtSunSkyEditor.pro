CONFIG+=debug_and_release
LIBS += -lgomp ../../../redshift/lib/libpicogen.a
QMAKE_CXXFLAGS += -fopenmp -O3
QMAKE_LFLAGS += -fopenmp

INCLUDEPATH += ../include/ \
    ../../ \
    ../../../redshift/include/
#../../../instant-preview/open-gl/include/ \
#../../skylab/include/
HEADERS += qtsunskyeditor.hh
SOURCES += qtsunskyeditor.cc \
    main.cc
#../../redshift/src/constants.cc
#../../skylab/src/preetham.cc
FORMS += qtsunskyeditor.ui
RESOURCES += ../QtQuatschEditor/resources/icons.qrc

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}
