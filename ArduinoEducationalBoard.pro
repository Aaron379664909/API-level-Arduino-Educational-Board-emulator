#-------------------------------------------------
#
# Project created by QtCreator 2018-08-21T19:19:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArduinoEducationalBoard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usercode.cpp

HEADERS  += mainwindow.h \
    isadefinitions.h \
    isaliquidcrystal.h \
    isabuttons.h \
    isaledcontrol.h \
    rightjoy.h \
    isa7segmentdisplay.h \
    isaoled.h \
    leftjoy.h \
    usercode.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
