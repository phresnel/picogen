QT      += core gui
INCLUDEPATH += $$PWD $$PWD/../../../auxiliary
CXXFLAGS += -Wall
SOURCES += $$PWD/quatschpreseteditor.cc \
        $$PWD/../../../auxiliary/quatsch-preprocessor/ios.cc \
        $$PWD/../../../auxiliary/quatsch-preprocessor/meta.cc \
        $$PWD/../../../auxiliary/quatsch-preprocessor/parsing.cc
HEADERS += $$PWD/quatschpreseteditor.hh \
        $$PWD/../../../auxiliary/quatsch-preprocessor/ios.hh \
        $$PWD/../../../auxiliary/quatsch-preprocessor/meta.hh \
        $$PWD/../../../auxiliary/quatsch-preprocessor/parsing.hh \
        $$PWD/../../../auxiliary/quatsch-preprocessor/optional.hh \
        $$PWD/../../../auxiliary/quatsch-preprocessor/tuple.hh
FORMS   += $$PWD/quatschpreseteditor.ui
#include($$PWD/../QuatschPreview/quatschpreview.pri)
