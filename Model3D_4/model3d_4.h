#ifndef MODEL3D_4_H
#define MODEL3D_4_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>


#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "Model3D/node.h"
#include "Model3D/material.h"
#include "Model3D/vertexdata.h"

#include "transformational.h"



#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>





class Model3D_4 : public Transformational
{
public:
    Model3D_4();

    bool loadFromFile(QString filename);

    // Transformational interface
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;

private:

    void clear();
/*
    void recursiveInitMeshesMaterialsTransforms(
            aiMesh **pMeshes,
            const aiNode *pNode, QMatrix4x4 transformMatrix);
            */
    bool initFromScene(const aiScene *pScene);

    void addVertexDatas(const aiMesh* const pMesh,
                            QVector<VertexData> &vertexDatas,
                            uint &vertexData_LastIndex);

    void addIndexes(const aiMesh* const pMesh,
                    QVector<uint> &indexes,
                    uint &indexes_LastIndex);

    struct VectorsForShader {
        VectorsForShader(aiMesh** meshes, uint size)
            : Meshes(meshes), VertexData_LastIndex(0), Indexes_LastIndex(0)
        {
            uint totalVertexes = 0;
            uint totalFaces = 0;

            for (int i = 0; i < size; ++i) {
                const aiMesh * const pMesh = meshes[i];

                totalVertexes += pMesh->mNumVertices;
                totalFaces += pMesh->mNumFaces;
            }

            VertexDatas.resize(totalVertexes);
            Indexes.resize(totalFaces * 3);
        }

        QVector<VertexData> VertexDatas;
        uint VertexData_LastIndex;

        QVector<uint> Indexes;
        uint Indexes_LastIndex;

        aiMesh** Meshes;
    };

    void recursiveProcessAiNodes(
            const aiNode* pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors, uint lastMeshIndex = 0);


    struct Mesh {
        Mesh() {}

        uint NumVertexes;
        uint NumIndexes;
        uint BaseIndex;
        uint MaterialIndex;
        uint TransformMatrixIndex;
    };


    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    uint m_totalIndexes;
    QVector<Mesh> m_meshes;
    QVector<uint> m_v;
    QVector<Material> m_materials;
    QVector<QMatrix4x4> m_transformMatrixes;


};

#endif // MODEL3D_4_H
