INCLUDEPATH += $$PWD

INCLUDEPATH += $$PWD/../../../
INCLUDEPATH += $$PWD/../../../redshift/include/

DEFINES += __STDC_LIMIT_MACROS

SOURCES += \
    $$PWD/spectralcolorpicker.cc \
    $$PWD/spectralslider.cc \
    $$PWD/spectrumdisplay.cc \
    $$PWD/spectralcurve.cc \
    $$PWD/importrawdatawizard.cc
HEADERS += \
    $$PWD/spectralcolorpicker.hh \
    $$PWD/spectralslider.hh \
    $$PWD/spectrumdisplay.hh \
    $$PWD/spectralcurve.hh \
    $$PWD/createspectralimage.hh \
    $$PWD/importrawdatawizard.hh
FORMS += \
    $$PWD/spectralcolorpicker.ui \
    $$PWD/spectralslider.ui \
    $$PWD/spectrumdisplay.ui \
    $$PWD/spectralcurve.ui \
    $$PWD/importrawdatawizard.ui
LIBS += $$PWD/../../../redshift/lib/libpicogen.a


