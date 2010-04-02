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
    spectralslider.cc \
    spectrumdisplay.cc \
    spectralcurve.cc \
    importrawdatawizard.cc
HEADERS += mainwindow.hh \
    spectralcolorpicker.hh \
    spectralslider.hh \
    spectrumdisplay.hh \
    spectralcurve.hh \
    createspectralimage.hh \
    importrawdatawizard.hh
FORMS += mainwindow.ui \
    spectralcolorpicker.ui \
    spectralslider.ui \
    spectrumdisplay.ui \
    spectralcurve.ui \
    importrawdatawizard.ui
LIBS += ../../../redshift/lib/libpicogen.a

OTHER_FILES += spectralcolorpicker.pri
