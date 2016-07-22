#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T21:55:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softEMG-updater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    downloadmanager.cpp

HEADERS  += mainwindow.h \
    console.h \
    downloadmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

QMAKE_CXXFLAGS += -std=gnu++11
