#-------------------------------------------------
#
# Project created by QtCreator 2015-01-31T17:49:27
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TWEANN_Platform
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    neuron.cpp \
    connections.cpp \
    neuralnetwork.cpp \
    controller.cpp \
    scape.cpp

HEADERS += \
    neuron.h \
    connections.h \
    neuralnetwork.h \
    controller.h \
    scape.h \
    iscapeinterface.h
