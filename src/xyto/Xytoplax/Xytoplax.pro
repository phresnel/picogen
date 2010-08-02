#-------------------------------------------------
#
# Project created by QtCreator 2010-08-02T13:48:51
#
#-------------------------------------------------

QT       += core gui

TARGET = Xytoplax
TEMPLATE = app


SOURCES += main.cc\
        mainwindow.cc \
    simple.cc \
    ../xyto_ios.cc \
    ../tokenize.cc \
    ../token.cc \
    ../segment.cc \
    ../production_header.cc \
    ../production_body.cc \
    ../production.cc \
    ../pattern.cc \
    ../parse_expr.cc \
    ../parameterlist.cc \
    ../parameter.cc \
    ../lsystem.cc \
    ../interpreter.cc \
    ../compiler.cc

HEADERS  += mainwindow.hh \
    simple.hh \
    ../xyto_ios.hh \
    ../tokenize.hh \
    ../token.hh \
    ../soft_value.hh \
    ../segment.hh \
    ../production_header.hh \
    ../production_body.hh \
    ../production.hh \
    ../pattern.hh \
    ../parse_helpers.hh \
    ../parse_expr.hh \
    ../parameterlist.hh \
    ../parameter.hh \
    ../lsystem.hh \
    ../kiss.hh \
    ../codeiterator.hh

FORMS    += mainwindow.ui \
    simple.ui
