CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(spectralcolorpickerplugin)
TEMPLATE    = lib

HEADERS     = spectralcolorpickerplugin.hh
SOURCES     = spectralcolorpickerplugin.cc
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(spectralcolorpicker.pri)
