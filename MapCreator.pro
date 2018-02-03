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
    skybox.cpp \
    test.cpp \
    model3d_2.cpp \
    mesh.cpp \
    vertexdata.cpp \
    Model3D/mesh.cpp \
    Model3D/model3d_2.cpp \
    Model3D/vertexdata.cpp \
    Model3D/material.cpp \
    Model3D/node.cpp \
    ModelLoader.cpp \
    Model3D/bone.cpp \
    Model3D/bone.cpp \
    Model3D/material.cpp \
    Model3D/mesh.cpp \
    Model3D/model3d_2.cpp \
    Model3D/node.cpp \
    Model3D/vertexdata.cpp \
    camera3d.cpp \
    group3d.cpp \
    main.cpp \
    mainwindow.cpp \
    Model3D.cpp \
    ModelLoader.cpp \
    simpleobject3d.cpp \
    skybox.cpp \
    test.cpp \
    Model3D/bone.cpp \
    Model3D/material.cpp \
    Model3D/mesh.cpp \
    Model3D/model3d_2.cpp \
    Model3D/node.cpp \
    Model3D/vertexdata.cpp \
    camera3d.cpp \
    group3d.cpp \
    main.cpp \
    mainwindow.cpp \
    Model3D.cpp \
    ModelLoader.cpp \
    simpleobject3d.cpp \
    skybox.cpp \
    test.cpp \
    Model3D/bone.cpp \
    Model3D/material.cpp \
    Model3D/mesh.cpp \
    Model3D/model3d_2.cpp \
    Model3D/node.cpp \
    Model3D/vertexdata.cpp \
    camera3d.cpp \
    group3d.cpp \
    main.cpp \
    mainwindow.cpp \
    Model3D.cpp \
    ModelLoader.cpp \
    simpleobject3d.cpp \
    skybox.cpp \
    test.cpp \
    Model3D_2/model3d_3.cpp \
    Model3D_4/model3d_4.cpp

HEADERS += \
        mainwindow.h \
    simpleobject3d.h \
    transformational.h \
    group3d.h \
    camera3d.h \
    skybox.h \
    mesh.h \
    vertexdata.h \
    Model3D/mesh.h \
    Model3D/model3d_2.h \
    Model3D/vertexdata.h \
    Model3D/material.h \
    Model3D/node.h \
    ModelLoader.h \
    Model3D/bone.h \
    External/assimp/include/assimp/Compiler/poppack1.h \
    External/assimp/include/assimp/Compiler/pstdint.h \
    External/assimp/include/assimp/Compiler/pushpack1.h \
    External/assimp/include/assimp/ai_assert.h \
    External/assimp/include/assimp/anim.h \
    External/assimp/include/assimp/camera.h \
    External/assimp/include/assimp/cexport.h \
    External/assimp/include/assimp/cfileio.h \
    External/assimp/include/assimp/cimport.h \
    External/assimp/include/assimp/color4.h \
    External/assimp/include/assimp/config.h \
    External/assimp/include/assimp/DefaultLogger.hpp \
    External/assimp/include/assimp/defs.h \
    External/assimp/include/assimp/Exporter.hpp \
    External/assimp/include/assimp/Importer.hpp \
    External/assimp/include/assimp/importerdesc.h \
    External/assimp/include/assimp/IOStream.hpp \
    External/assimp/include/assimp/IOSystem.hpp \
    External/assimp/include/assimp/light.h \
    External/assimp/include/assimp/Logger.hpp \
    External/assimp/include/assimp/LogStream.hpp \
    External/assimp/include/assimp/material.h \
    External/assimp/include/assimp/matrix3x3.h \
    External/assimp/include/assimp/matrix4x4.h \
    External/assimp/include/assimp/mesh.h \
    External/assimp/include/assimp/metadata.h \
    External/assimp/include/assimp/NullLogger.hpp \
    External/assimp/include/assimp/postprocess.h \
    External/assimp/include/assimp/ProgressHandler.hpp \
    External/assimp/include/assimp/quaternion.h \
    External/assimp/include/assimp/scene.h \
    External/assimp/include/assimp/texture.h \
    External/assimp/include/assimp/types.h \
    External/assimp/include/assimp/vector2.h \
    External/assimp/include/assimp/vector3.h \
    External/assimp/include/assimp/version.h \
    Model3D/bone.h \
    Model3D/material.h \
    Model3D/mesh.h \
    Model3D/model3d_2.h \
    Model3D/node.h \
    Model3D/vertexdata.h \
    camera3d.h \
    group3d.h \
    mainwindow.h \
    Model3D.h \
    ModelLoader.h \
    simpleobject3d.h \
    skybox.h \
    transformational.h \
    External/assimp/include/assimp/Compiler/poppack1.h \
    External/assimp/include/assimp/Compiler/pstdint.h \
    External/assimp/include/assimp/Compiler/pushpack1.h \
    External/assimp/include/assimp/ai_assert.h \
    External/assimp/include/assimp/anim.h \
    External/assimp/include/assimp/camera.h \
    External/assimp/include/assimp/cexport.h \
    External/assimp/include/assimp/cfileio.h \
    External/assimp/include/assimp/cimport.h \
    External/assimp/include/assimp/color4.h \
    External/assimp/include/assimp/config.h \
    External/assimp/include/assimp/DefaultLogger.hpp \
    External/assimp/include/assimp/defs.h \
    External/assimp/include/assimp/Exporter.hpp \
    External/assimp/include/assimp/Importer.hpp \
    External/assimp/include/assimp/importerdesc.h \
    External/assimp/include/assimp/IOStream.hpp \
    External/assimp/include/assimp/IOSystem.hpp \
    External/assimp/include/assimp/light.h \
    External/assimp/include/assimp/Logger.hpp \
    External/assimp/include/assimp/LogStream.hpp \
    External/assimp/include/assimp/material.h \
    External/assimp/include/assimp/matrix3x3.h \
    External/assimp/include/assimp/matrix4x4.h \
    External/assimp/include/assimp/mesh.h \
    External/assimp/include/assimp/metadata.h \
    External/assimp/include/assimp/NullLogger.hpp \
    External/assimp/include/assimp/postprocess.h \
    External/assimp/include/assimp/ProgressHandler.hpp \
    External/assimp/include/assimp/quaternion.h \
    External/assimp/include/assimp/scene.h \
    External/assimp/include/assimp/texture.h \
    External/assimp/include/assimp/types.h \
    External/assimp/include/assimp/vector2.h \
    External/assimp/include/assimp/vector3.h \
    External/assimp/include/assimp/version.h \
    Model3D/bone.h \
    Model3D/material.h \
    Model3D/mesh.h \
    Model3D/model3d_2.h \
    Model3D/node.h \
    Model3D/vertexdata.h \
    camera3d.h \
    group3d.h \
    mainwindow.h \
    Model3D.h \
    ModelLoader.h \
    simpleobject3d.h \
    skybox.h \
    transformational.h \
    Model3D/bone.h \
    Model3D/material.h \
    Model3D/mesh.h \
    Model3D/model3d_2.h \
    Model3D/node.h \
    Model3D/vertexdata.h \
    camera3d.h \
    group3d.h \
    mainwindow.h \
    Model3D.h \
    ModelLoader.h \
    simpleobject3d.h \
    skybox.h \
    transformational.h \
    Model3D_2/model3d_3.h \
    Model3D_4/model3d_4.h \
    assimp_adapter.h


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
