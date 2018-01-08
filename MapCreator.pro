#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T16:01:11
#
#-------------------------------------------------

QT       += core gui widgets


TARGET = MapCreator
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
    simpleobject3d.cpp \
    group3d.cpp \
    camera3d.cpp \
    skybox.cpp

HEADERS += \
        mainwindow.h \
    simpleobject3d.h \
    transformational.h \
    group3d.h \
    camera3d.h \
    skybox.h

win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += -lopengl32.lib
}

RESOURCES += \
    shaders.qrc \
    textures.qrc
