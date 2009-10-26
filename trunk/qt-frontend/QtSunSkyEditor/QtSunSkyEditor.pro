INCLUDEPATH += ../include/ \
    ../../ \
    ../../instant-preview/open-gl/include/ \
    ../../skylab/include/ \
    ../../redshift/include/
HEADERS += include/qtsunskyeditor.hh \
    ../../skylab/include/preetham.hh
SOURCES += src/qtsunskyeditor.cc \
    src/main.cc \
    ../../skylab/src/preetham.cc \
    ../../redshift/src/constants.cc
FORMS += forms/qtsunskyeditor.ui
