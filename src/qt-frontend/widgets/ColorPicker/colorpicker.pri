INCLUDEPATH += $$PWD
SOURCES += $$PWD/colorpicker.cc \
    ../widgets/ColorPicker/colorpickerwidget.cc
HEADERS += $$PWD/colorpicker.hh \
    ../widgets/ColorPicker/colorpickerwidget.hh \
    ../widgets/ColorPicker/colorpickercolor.hh
FORMS += $$PWD/colorpicker.ui \
    ../widgets/ColorPicker/colorpickerwidget.ui
include(../TristimulusColorPicker/tristimuluscolorpicker.pri)
include(../SpectralColorPicker/spectralcolorpicker.pri)
