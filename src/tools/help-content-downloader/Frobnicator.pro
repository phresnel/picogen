# -------------------------------------------------
# Project created by QtCreator 2010-05-11T07:20:47
# -------------------------------------------------
QT += webkit \
    xml

# QT       -= gui
CONFIG += console
CONFIG -= app_bundle
TARGET = Frobnicator
LIBS += -lcurl
TEMPLATE = app
SOURCES += main.cc
HEADERS += download.hh
