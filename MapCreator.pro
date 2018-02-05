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
    group3d.cpp \
    camera3d.cpp \
    main.cpp \
    mainwindow.cpp \
    simpleobject3d.cpp \
    Model3D/bone.cpp \
    Model3D/material.cpp \
    Model3D/node.cpp \
    Model3D/vertexdata.cpp \
    skybox.cpp \
    Model3D_4/model3d_4.cpp \
    test.cpp \
    assimp_adapter.cpp

HEADERS += \
    Model3D/material.h \
    Model3D/bone.h \
    Model3D/node.h \
    Model3D/vertexdata.h \
    camera3d.h \
    group3d.h \
    mainwindow.h \
    simpleobject3d.h \
    skybox.h \
    transformational.h \
    Model3D_4/model3d_4.h \
    assimp_adapter.h \
    test.h



win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += opengl32.lib
}


RESOURCES += \
    shaders.qrc \
    textures.qrc \
    models.qrc


win32: LIBS += -L$$PWD/External/assimp/ -lassimp

INCLUDEPATH += $$PWD/External/assimp/include
DEPENDPATH += $$PWD/External/assimp/include

DISTFILES += \
    Model3D_2/cube.jpg
