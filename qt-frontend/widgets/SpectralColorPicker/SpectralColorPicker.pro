# -------------------------------------------------
# Project created by QtCreator 2010-03-25T10:39:36
# -------------------------------------------------
TARGET = SpectralColorPicker
TEMPLATE = app

INCLUDEPATH += ../../../
INCLUDEPATH += ../../../redshift/include/

DEFINES += __STDC_LIMIT_MACROS

SOURCES += main.cc \
    mainwindow.cc \
    spectralcolorpicker.cc \
    spectralslider.cc
HEADERS += mainwindow.hh \
    spectralcolorpicker.hh \
    spectralslider.hh
FORMS += mainwindow.ui \
    spectralcolorpicker.ui \
    spectralslider.ui
LIBS += ../../../redshift/lib/libpicogen.a

