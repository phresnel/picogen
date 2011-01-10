INCLUDEPATH += $$PWD
SOURCES *= $$PWD/fragmentwidget.cc
HEADERS *= $$PWD/fragmentwidget.hh
FORMS   *= $$PWD/fragmentwidget.ui

HEADERS *= \
        $$PWD/../shared_ptr.hh \
        $$PWD/../optional.hh

include($$PWD/preset/preset.pri)
include($$PWD/../repository/repository.pri)
