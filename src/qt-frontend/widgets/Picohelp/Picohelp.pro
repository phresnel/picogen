# -------------------------------------------------
# Project created by QtCreator 2010-05-13T09:20:23
# -------------------------------------------------
QT += webkit
TARGET = picohelp
TEMPLATE = app
CONFIG+=debug_and_release
SOURCES += main.cc \
    mainwindow.cc \
    picohelpbrowser.cc
HEADERS += mainwindow.hh \
    picohelpbrowser.hh
FORMS += mainwindow.ui \
    picohelpbrowser.ui
RESOURCES += ../../SimplexGui/resources.qrc
OTHER_FILES += picohelp.pri
RC_FILE += icon.rc
OTHER_FILES += icon.rc
