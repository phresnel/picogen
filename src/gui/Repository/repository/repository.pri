INCLUDEPATH *= $$PWD
SOURCES *= \
        $$PWD/repositorysettings.cc \
        $$PWD/fragment.cc \
        $$PWD/collection.cc \
        $$PWD/database.cc \
        $$PWD/parametricpreset.cc \
    ../Repository/repository/metadata.cc

HEADERS *= \
        $$PWD/repositorysettings.hh \
        $$PWD/collection.hh \
        $$PWD/fragment.hh \
        $$PWD/database.hh \
        $$PWD/parametricpreset.hh \
    ../Repository/repository/metadata.hh

include($$PWD/../../widgets/QuatschPreview/quatschpreview.pri)
include($$PWD/../../widgets/Preview/preview.pri)
