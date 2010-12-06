INCLUDEPATH += $$PWD \
        $$PWD/../../../auxiliary/ \
        $$PWD/../../../renderer/
SOURCES += $$PWD/quatschsourceeditor.cc
HEADERS += $$PWD/quatschsourceeditor.hh
FORMS += $$PWD/quatschsourceeditor.ui
include($$PWD/../QuatschPreview/quatschpreview.pri)
