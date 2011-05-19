INCLUDEPATH *= $$PWD
SOURCES *= $$PWD/colorpicker.cc \
    ../widgets/ColorPicker/colorpickerwidget.cc
HEADERS *= $$PWD/colorpicker.h \
    ../widgets/ColorPicker/colorpickercolor.h \
    ../widgets/ColorPicker/colorpickerwidget.h
FORMS *= $$PWD/colorpicker.ui \
    ../widgets/ColorPicker/colorpickerwidget.ui
include(../TristimulusColorPicker/tristimuluscolorpicker.pri)
include(../SpectralColorPicker/spectralcolorpicker.pri)
