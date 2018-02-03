#ifndef MODEL3D_4_H
#define MODEL3D_4_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>


#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


#include "Model3D/mesh.h"
#include "Model3D/node.h"
#include "Model3D/material.h"

#include "Model3D/vertexdata.h"



#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class Model3D_4
{
public:
    Model3D_4();

    bool loadFromFile(QString filename);

private:

    void clear();

    void recursiveInitMeshesMaterialsTransforms(
            const aiNode *pNode, QMatrix4x4 transformMatrix) const;
    void initFromScene(aiScene *pScene);

    void recursiveProcessAiMeshes(const aiNode *pNode);

    void addVertexDatas(const aiMesh* const pMesh,
                            QVector<VertexData> &vertexDatas,
                            uint &vertexData_LastIndex);

    void addIndexes(const aiMesh* const pMesh,
                    QVector<uint> &indexes,
                    uint &indexes_LastIndex);

    struct VectorsForShader {
        VectorsForShader(const aiMesh** const meshes, uint size)
            : VertexData_LastIndex(0), Indexes_LastIndex(0)
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
    };

    void recursiveProcessAiNodes(
            const aiNode* pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors);


    struct Mesh {
        uint NumVertexes;
        uint BaseIndex;
        uint MaterialIndex;
        uint TransformMatrixIndex;
    };


    QOpenGLVertexArrayObject m_VAO;

    uint m_totalVertexes;
    QVector<Mesh> m_meshes;
    QVector<Material> m_materials;
    QVector<QMatrix4x4> m_transformMatrixes;
};

#endif // MODEL3D_4_H
