#ifndef MODEL3D_4_H
#define MODEL3D_4_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>


#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include "Model3D/node.h"
#include "Model3D/material.h"
#include "Model3D/vertexdata.h"

#include "transformational.h"



#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


#define TEST_MODE


class Model3D : public Transformational
{
public:

#ifdef TEST_MODE



#endif


    Model3D(QVector4D objId);
    ~Model3D();

    bool loadFromFile(QString filename);

    // Transformational interface
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    void drawSelectionCubes(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override;

    QVector3D getPosition() const override;

private:

    void clear();

    bool initFromScene(const aiScene *pScene);

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

    void determinateSelectionCube(VectorsForShader& vectors);

    void addVertexDatas(const aiMesh* const pMesh,
                            QVector<VertexData> &vertexDatas,
                            uint &vertexData_LastIndex);

    void addIndexes(const aiMesh* const pMesh,
                    QVector<uint> &indexes,
                    uint &indexes_LastIndex,
                    uint shift);

    void recursiveProcessAiNodes(
            const aiNode* pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors, uint lastMeshIndex = 0);

    void initMaterials(const aiScene* pScene);


    struct Mesh {
        Mesh() {}

        uint NumVertexes;
        uint NumIndexes;
        uint BaseIndex;
        uint BaseVertex;
        uint MaterialIndex;
        uint TransformMatrixIndex;
    };

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    QVector<Mesh> m_meshes;
    QVector<QSharedPointer<Material>> m_materials;
    QVector<QMatrix4x4> m_transformMatrixes;

    QVector3D m_translate;
    QQuaternion m_rotate;
    QVector3D m_scale;

    QMatrix4x4 m_globalTransform;

    QVector4D m_objId;
    QVector3D m_minCubeCorner;
    QVector3D m_maxCubeCorner;

    QOpenGLBuffer m_vertexBuffer_SelectionCube;
    QOpenGLBuffer m_indexBuffer_SelectionCube;
};

#endif // MODEL3D_4_H
