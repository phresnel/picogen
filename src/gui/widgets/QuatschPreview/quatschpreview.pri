INCLUDEPATH += \
        $$PWD \
        $$PWD/../../../auxiliary/ \
        $$PWD/../../../renderer/
SOURCES += $$PWD/quatschpreview.cc
HEADERS += $$PWD/quatschpreview.hh
FORMS   += $$PWD/quatschpreview.ui
DEFINES += __STDC_LIMIT_MACROS
QMAKE_CXXFLAGS += -fopenmp
LIBS += \
        $$PWD/../../../libredshift.a  \
        -lgomp \
        -lnoise \
        -lSDL_image -lSDL

unix:LIBS += \
        /usr/lib/libboost_program_options.a \
        /usr/lib/libboost_filesystem.a \
        /usr/lib/libboost_system.a
windows:LIBS += \
        -lboost_program_options \
        -lboost_filesystem \
        -lboost_system
