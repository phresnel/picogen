INCLUDEPATH += $$PWD

SOURCES *= \
        $$PWD/presetdialog.cc \
        $$PWD/presetlistitemwidget.cc \
        $$PWD/presetlistwidget.cc \
        $$PWD/selectpresetdialog.cc

HEADERS *= \
        $$PWD/presetdialog.hh \
        $$PWD/presetlistitemwidget.hh \
        $$PWD/presetlistwidget.hh \
        $$PWD/selectpresetdialog.hh

FORMS *= \
        $$PWD/presetdialog.ui \
        $$PWD/presetlistitemwidget.ui \
        $$PWD/presetlistwidget.ui \
        $$PWD/selectpresetdialog.ui

HEADERS *= \
        $$PWD/../../shared_ptr.hh \
        $$PWD/../../optional.hh

include($$PWD/../../repository/repository.pri)
