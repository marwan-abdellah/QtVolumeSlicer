#-------------------------------------------------
#
# Project created by QtCreator 2014-06-20T10:32:32
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VolumeSlicer
INSTALLS += target
TEMPLATE = app

SOURCES +=      RunVolumeSlicer.cpp \
                OpenGLWindow.cpp \
                VolumeSlicer.cpp

HEADERS +=      OpenGLWindow.h \
                VolumeSlicer.h
