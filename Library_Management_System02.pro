#-------------------------------------------------
#
# Project created by QtCreator 2025-08-11T22:44:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Library_Management_System02
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    libraryitem.cpp \
    book.cpp \
    magazine.cpp \
    libraryutils.cpp

HEADERS  += mainwindow.h \
    libraryitem.h \
    book.h \
    magazine.h \
    libraryutils.h

FORMS    += mainwindow.ui
