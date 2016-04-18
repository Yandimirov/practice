#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T19:25:32
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Triangle33
TEMPLATE = app


SOURCES +=\
    Triangle.cpp \
    MainWindow.cpp \
    Main.cpp

HEADERS  += mainwindow.h \
    triangle.h

FORMS    += mainwindow.ui

LIBS += -lGLEW
