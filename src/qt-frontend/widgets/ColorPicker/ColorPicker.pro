# -------------------------------------------------
# Project created by QtCreator 2010-04-13T21:20:24
# -------------------------------------------------
TARGET = ColorPicker
TEMPLATE = app
INCLUDEPATH += ../widgets/
SOURCES += main.cc \
    mainwindow.cc \
    colorpicker.cc
HEADERS += mainwindow.hh \
    colorpicker.hh
FORMS += mainwindow.ui \
    colorpicker.ui
OTHER_FILES += colorpicker.pri
include(../TristimulusColorPicker/tristimuluscolorpicker.pri)
include(../SpectralColorPicker/spectralcolorpicker.pri)
