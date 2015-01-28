#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T14:39:08
#
#-------------------------------------------------

QT       += core gui

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mogs_video_widget.cpp \
    imgconvert.cpp \
    mogs_qt_video_scene.cpp \
    mogs_qt_list_point.cpp \
    mogs_qt_list_video.cpp

HEADERS  += mogs_video_widget.h \
    imgconvert.h \
    mogs_qt_video_scene.h \
    mogs_qt_list_point.h \
    mogs_qt_list_video.h

FORMS    += mogs_video_widget.ui

unix|win32: LIBS += -lopencv_core

unix|win32: LIBS += -lopencv_highgui
