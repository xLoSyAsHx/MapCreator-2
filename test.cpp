#ifndef TEST_H
#define TEST_H


#include <QString>
#include <QMatrix4x4>

#include  "External/assimp-3.3.1/include/assimp/scene.h"
#include "External/assimp-3.3.1/include/assimp/Importer.hpp"
#include "External/assimp-3.3.1/include/assimp/postprocess.h"




struct MaterialInfo
{
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};




struct LightInfo
{
    QVector4D Position;
    QVector3D Intensity;
};


struct Mesh
{
    QString name;
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};


struct Node
{
    QString name;
    QMatrix4x4 transformation;
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};





class Test {


public:

    void LoadFromFBX(QString path) {
        Assimp::Importer importer;

        aiScene scene = Assimp::Importer::ReadFile(path.toStdString(),
                                                   aiProcess_GenSmoothNormals |
                                                   aiProcess_CalcTangentSpace |
                                                   aiProcess_Triangulate |
                                                   aiProcess_JoinIdenticalVertices |
                                                   aiProcess_SortByPType
                                                   );

        if (!scene)
            {
                qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
                return false;
            }

        for (int i = 0; i < scene.mNumMeshes; ++i) {

        }
    }


    void processMesh(aiMesh mesh) {

    }
}


#endif
