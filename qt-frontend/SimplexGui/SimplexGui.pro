# -------------------------------------------------
# Project created by QtCreator 2010-03-18T07:25:16
# -------------------------------------------------
TARGET = simplex-gui
TEMPLATE = app
DEFINES += __STDC_LIMIT_MACROS
SOURCES += main.cc \
    mainwindow.cc \
    ../RenderWindow/renderwindow.cc \
    ../RenderWindow/qimagerendertarget.cc \
    ../SimpleInputBox/simpleinputbox.cc
HEADERS += mainwindow.hh \
    ../RenderWindow/renderwindow.hh \
    ../RenderWindow/qimagerendertarget.hh \
    ../SimpleInputBox/simpleinputbox.hh
FORMS += mainwindow.ui \
    ../RenderWindow/renderwindow.ui \
    ../SimpleInputBox/simpleinputbox.ui
LIBS += ../../redshift/lib/libpicogen.a \
    -lgomp \
    -lboost_program_options \
    -lSDL
include(../../upstream/QtSolutions/qtwindowlistmenu-2.2_1-opensource/src/qtwindowlistmenu.pri)
include(../../upstream/QtSolutions/qtpropertybrowser-2.5_1-opensource/src/qtpropertybrowser.pri)
