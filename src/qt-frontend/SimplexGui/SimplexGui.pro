# -------------------------------------------------
# Project created by QtCreator 2010-03-18T07:25:16
# -------------------------------------------------
TARGET = picogen-simplex
RC_FILE = icon.rc
TEMPLATE = app
DEFINES += __STDC_LIMIT_MACROS
INCLUDEPATH += ../../upstream/qt-widgets/pictureflow-0.1.0/pictureflow-qt/
INCLUDEPATH += ../widgets/
INCLUDEPATH += ../widgets/QuatschSourceEditor/
INCLUDEPATH += ../widgets/SimpleInputBox/
INCLUDEPATH += ../widgets/RenderWindow/
INCLUDEPATH += ../../
SOURCES += main.cc \
    mainwindow.cc \
    ../widgets/RenderWindow/renderwindow.cc \
    ../widgets/RenderWindow/qimagerendertarget.cc \
    ../widgets/QuatschSourceEditor/quatschsourceeditor.cc \
    ../widgets/OpenSceneFile/openscenefile.cc \
    coloreditmanager.cpp \
    coloreditfactory.cpp \
    coloredit.cpp \
    propertybrowser/propertybrowser-helpers.cc \
    propertybrowser/objectpropertybrowser.cc \
    propertybrowser/volumepropertybrowser.cc \
    propertybrowser/rendersettingspropertybrowser.cc \
    propertybrowser/cameraspropertybrowser.cc \
    propertybrowser/filmsettingspropertybrowser.cc \
    propertybrowser/backgroundspropertybrowser.cc \
    propertybrowser/scenepropertybrowser.cc \
    ../../upstream/qt-widgets/pictureflow-0.1.0/pictureflow-qt/pictureflow.cpp
HEADERS += mainwindow.hh \
    ../widgets/RenderWindow/renderwindow.hh \
    ../widgets/RenderWindow/qimagerendertarget.hh \
    ../widgets/QuatschSourceEditor/quatschsourceeditor.hh \
    ../widgets/OpenSceneFile/openscenefile.hh \
    coloreditmanager.h \
    coloreditfactory.h \
    coloredit.h \
    propertybrowser/propertybrowser-helpers.hh \
    propertybrowser/objectpropertybrowser.hh \
    propertybrowser/volumepropertybrowser.hh \
    propertybrowser/rendersettingspropertybrowser.hh \
    propertybrowser/cameraspropertybrowser.hh \
    propertybrowser/filmsettingspropertybrowser.hh \
    propertybrowser/backgroundspropertybrowser.hh \
    propertybrowser/scenepropertybrowser.hh \
    ../../upstream/QtApps-Widgets/QtCoverFlow/coverflow.h \
    ../../upstream/QtApps-Widgets/QtCoverFlow/coverflow_p.h \
    ../../upstream/qt-widgets/pictureflow-0.1.0/pictureflow-qt/pictureflow.h
FORMS += mainwindow.ui \
    ../widgets/RenderWindow/renderwindow.ui \
    ../widgets/QuatschSourceEditor/quatschsourceeditor.ui \
    ../widgets/OpenSceneFile/openscenefile.ui
LIBS += ../../redshift/lib/libpicogen.a \
    -lgomp \
    -lnoise \
    -lSDL \
    -lSDL_image
unix:LIBS += /usr/lib/libboost_program_options.a \
    /usr/lib/libboost_filesystem.a \
    /usr/lib/libboost_system.a
windows:LIBS += -lboost_program_options \
    -lboost_filesystem \
    -lboost_system
include(../../upstream/QtSolutions/qtwindowlistmenu-2.2_1-opensource/src/qtwindowlistmenu.pri)
include(../../upstream/QtSolutions/qtpropertybrowser-2.5_1-opensource/src/qtpropertybrowser.pri)
include(../widgets/ColorPicker/colorpicker.pri)
include(../widgets/Picohelp/picohelp.pri)
OTHER_FILES += HOWTO_New_Stuff.txt \
    icon.rc
RESOURCES += resources.qrc
