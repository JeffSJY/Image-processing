#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T21:22:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = work_1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mypixmapitem.cpp \
    imageprocessor.cpp \
    dialog.cpp \
    dialog2.cpp \
    dialog3.cpp \
    dialog4.cpp \
    dialog5.cpp \
    dialog6.cpp \
    dialog7.cpp \
    matrix.cpp \
    twofft.cpp \
    dialogmat.cpp \
    dialogidf.cpp \
    dialoghdf.cpp

HEADERS += \
        mainwindow.h \
    mypixmapitem.h \
    imageprocessor.h \
    dialog.h \
    dialog2.h \
    dialog3.h \
    dialog4.h \
    dialog5.h \
    dialog6.h \
    dialog7.h \
    matrix.h \
    twofft.h \
    dialogmat.h \
    dialogidf.h \
    dialoghdf.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    dialog2.ui \
    dialog3.ui \
    dialog4.ui \
    dialog5.ui \
    dialog6.ui \
    dialog7.ui \
    dialogmat.ui \
    dialogidf.ui \
    dialoghdf.ui

DISTFILES +=
