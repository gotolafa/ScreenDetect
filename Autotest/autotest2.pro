#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07T18:02:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autotest2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        frame.cpp

HEADERS  += frame.h

FORMS    += frame.ui

INCLUDEPATH += C:\opencv-build\install\include
LIBS += -L"C:\opencv-build\install\x86\mingw\lib"
LIBS += -llibopencv_core320.dll
LIBS += -llibopencv_highgui320.dll
LIBS += -llibopencv_imgproc320.dll
LIBS += -llibopencv_video320.dll
LIBS += -llibopencv_videoio320.dll
LIBS += -llibopencv_imgcodecs320.dll
LIBS += -llibopencv_photo320.dll

DISTFILES += \
    pic.jpg
