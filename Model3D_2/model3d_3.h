#ifndef MODEL3D_3_H
#define MODEL3D_3_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>


#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


#include "Model3D/mesh.h"
#include "Model3D/node.h"
#include "Model3D/material.h"

#include "Model3D/vertexdata.h"



#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class Model3D_3
{
public:
    Model3D_3();

    bool loadMesh(const QString& filename);

private:
    void initFromScene(const aiScene* pScene, const std::string& filename);
    void initMesh(unsigned int Index, const aiMesh* paiMesh);
    QSharedPointer<Material> initMaterial(const aiMaterial* material);
    void Clear();


    struct MeshEntry {
            MeshEntry() :
                m_VB(QOpenGLBuffer::VertexBuffer),
                m_IB(QOpenGLBuffer::IndexBuffer)
            {
                NumIndices    = 0;
                MaterialIndex = -1;
            }

            bool Init(const QVector<VertexData>& vertices,
                                  QVector<uint>& indices);

            QOpenGLBuffer m_VB;
            QOpenGLBuffer m_IB;

            unsigned int NumIndices;
            unsigned int MaterialIndex;
        };

        QVector<MeshEntry> m_Entries;
        QVector<QSharedPointer<Material>> m_materials;

};

#endif // MODEL3D_3_H
