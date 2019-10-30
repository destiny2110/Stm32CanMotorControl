#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T16:43:06
#
#-------------------------------------------------
TARGET = MyChartV2

TEMPLATE = app

QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


SOURCES +=\
    realtimedemo.cpp \
    qcharviewer.cpp \
    data_transfer.cpp

HEADERS  += \
    realtimedemo.h \
    qchartviewer.h \
    data_transfer.h

RESOURCES += \
    realtimedemo.qrc

INCLUDEPATH += ../include

DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

LIBS += ../lib/chartdir60.lib

QMAKE_POST_LINK = copy /Y ..\\lib\\chartdir60.dll $(DESTDIR)

FORMS +=
