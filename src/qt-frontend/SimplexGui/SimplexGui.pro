# -------------------------------------------------
# Project created by QtCreator 2010-03-18T07:25:16
# -------------------------------------------------
TARGET = picogen-simplex
RC_FILE = icon.rc
TEMPLATE = app
DEFINES += __STDC_LIMIT_MACROS
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
    coloreditmanager.cpp \
    coloreditfactory.cpp \
    coloredit.cpp
HEADERS += mainwindow.hh \
    ../widgets/RenderWindow/renderwindow.hh \
    ../widgets/RenderWindow/qimagerendertarget.hh \
    ../widgets/QuatschSourceEditor/quatschsourceeditor.hh \
    coloreditmanager.h \
    coloreditfactory.h \
    coloredit.h
FORMS += mainwindow.ui \
    ../widgets/RenderWindow/renderwindow.ui \
    ../widgets/QuatschSourceEditor/quatschsourceeditor.ui
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
