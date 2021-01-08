#-------------------------------------------------
#
# Project created by QtCreator 2020-09-22T09:42:10
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cyg2Comm
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

CONFIG += c++17

SOURCES += \
        main.cpp \
        cyg2comm.cpp \
    cyg_online.cpp \
    data_compute.cpp \
    cyg_worker.cpp \
    errmsg.cpp

HEADERS += \
        cyg2comm.h \
    cyg_online.h \
    cyg_worker.h \
    data_compute.h \
    errmsg.h

FORMS += \
        cyg2comm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = "cyg2.ico"

RESOURCES += \
    rsc/qrc.qrc
