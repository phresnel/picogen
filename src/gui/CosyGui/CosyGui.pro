# -------------------------------------------------
# Project created by QtCreator 2010-08-27T13:22:12
# -------------------------------------------------
QT += core gui
DEFINES += __STDC_LIMIT_MACROS
CONFIG += debug_and_release

CONFIG += console
#CONFIG += qtestlib
## gives you a console if necessary
#debug:CONFIG += console

QMAKE_CXXFLAGS += -fopenmp
INCLUDEPATH += ../../upstream/qt-widgets/pictureflow-0.1.0/pictureflow-qt/ \
    ../widgets/ \
    ../widgets/QuatschSourceEditor/ \
    ../widgets/SimpleInputBox/ \
    ../widgets/RenderWindow/ \
    ../../renderer/ \
    ../../auxiliary/
TARGET = CosyGui
TEMPLATE = app
SOURCES += main.cc \
    mainwindow.cc \
    terrainwindow.cc \
    stashview.cc \
    stashframe.cc \
    sunskywindow.cc \
    navigationwindow.cc \
    terrainformation.cc \
    material-ui.cc \
    filmsettingswindow.cc \
    twinrendersettingswindow.cc \
    rendersettingswindow.cc \
    stylesheetliveeditor.cc \
    terrainfitting.cc \
    waterwindow.cc \
    waterfitting.cc \
    camerawidget.cc \
    filmsettingsandcamera.cc \
    startpopup.cc
HEADERS += mainwindow.hh \
    terrainwindow.hh \
    stashview.hh \
    stashframe.hh \
    sunskywindow.hh \
    navigationwindow.hh \
    terrainformation.hh \
    material-ui.hh \
    filmsettingswindow.hh \
    twinrendersettingswindow.hh \
    rendersettingswindow.hh \
    createredshiftsceneclosure.hh \
    stylesheetliveeditor.hh \
    terrainfitting.hh \
    scopedblocksignals.hh \
    waterwindow.hh \
    waterfitting.hh \
    camerawidget.hh \
    filmsettingsandcamera.hh \
    startpopup.hh
FORMS += mainwindow.ui \
    terrainwindow.ui \
    stashview.ui \
    stashframe.ui \
    sunskywindow.ui \
    navigationwindow.ui \
    terrainformation.ui \
    material-ui.ui \
    filmsettingswindow.ui \
    twinrendersettingswindow.ui \
    rendersettingswindow.ui \
    stylesheetliveeditor.ui \
    terrainfitting.ui \
    waterwindow.ui \
    waterfitting.ui \
    camerawidget.ui \
    filmsettingsandcamera.ui \
    startpopup.ui
LIBS += ../../.libs/libredshift.a ../../.libs/libxyto.a \
    -lgomp \
    -lnoise \
    -lSDL \
    -lSDL_image
LIBS += -lpthread \
    -lIlmImf \
    -lz \
    -lImath \
    -lHalf \
    -lIex \
    -lIlmThread
unix:LIBS += /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a \
    /usr/lib/libboost_system.a
windows:LIBS += -lboost_program_options \
    -lboost_filesystem \
    -lboost_system
RESOURCES += resources.qrc
include(../widgets/QuatschSourceEditor/QuatschSourceEditor.pri)
include(../widgets/Picohelp/picohelp.pri)
include(../widgets/QtSunSkyEditor/qtsunskyeditor.pri)
include(../widgets/RenderWindow/renderwindow.pri)
include(../widgets/RenderWidget/renderwidget.pri)
include(../widgets/ColorPicker/colorpicker.pri)
include(../widgets/QuatschPreview/quatschpreview.pri)
include(../widgets/QuatschPresetEditor/quatschpreseteditor.pri)
include(../widgets/FilenameEdit/filenameedit.pri)
include(../widgets/TextDialog/textdialog.pri)
include(../widgets/GraphicalNavigation/graphicalnavigationwidget.pri)
include(../Repository/repository/repository.pri)
include(../Repository/fragment/fragment.pri)
include(cosyscene.pri)
CONFIG(debug, debug|release):DESTDIR = debug
else:DESTDIR = release
