#include "model3d_4.h"

Model3D_4::Model3D_4()
{

}

bool Model3D_4::loadFromFile(QString filename)
{
    bool result = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename.toStdString(),
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs);

    if (pScene) {
        result = initFromScene(pScene);
    }
    else {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
    }

    return result;
}

void Model3D_4::clear()
{
    if (m_VAO.isCreated())
        m_VAO.destroy();

    m_totalVertexes = 0;

    m_meshes.resize(0);
    m_materials.resize(0);
    m_transformMatrixes.resize(0);

}

void Model3D_4::recursiveInitMeshesMaterialsTransforms(
        const aiNode *pNode, QMatrix4x4 transformMatrix) const
{
    m_transformMatrixes.push_back(transformMatrix);

    for (int i = 0; i < pNode->mNumChildren; ++i) {
        for (int j = 0; j < pNode->mNumMeshes; ++j) {
            const aiMesh* pMesh = pNode->mMeshes[i];
            Mesh& mesh = m_meshes[i];

            mesh.BaseIndex = m_totalVertexes;
            mesh.NumVertexes = pMesh->mNumVertices;
            mesh.MaterialIndex = pMesh->mMaterialIndex;
            mesh.TransformMatrixIndex = m_transformMatrixes.size() - 1;

            m_totalVertexes += mesh.NumVertexes;
        }

        recursiveInitMeshesMaterialsTransforms(
                    pNode->mChildren[i], transformMatrix * pNode->mTransformation);
    }
}

void Model3D_4::initFromScene(aiScene *pScene)
{
    clear();

    m_VAO.create();
    m_VAO.bind();

    m_meshes.resize(pScene->mNumMeshes);
    m_materials.resize(pScene->mNumMaterials);
    m_transformMatrixes.resize(0);

    recursiveInitMeshesMaterialsTransforms(
                pScene->mRootNode, pScene->mRootNode->mTransformation);

    // Vectors of vertex data
    QVector<VertexData> vertexesData;
    vertexesData.resize(m_totalVertexes);


    // Init meshes through node hierarchy
    VectorsForShader vectors(pScene->mMeshes, pScene->mNumMeshes);
    recursiveProcessAiNodes(pScene->mRootNode,
                            pScene->mRootNode->mTransformation,
                            vectors);

    m_VAO.release();
}

void Model3D_4::recursiveProcessAiMeshes(const aiNode *pNode)
{
    for (int i = 0; i < pNode->mNumChildren; ++i) {
        for (int j = 0; j < pNode->mNumMeshes; ++j) {
            const aiMesh* pMesh = pNode->mMeshes[i];
            Mesh& mesh = m_meshes[i];

            mesh.BaseIndex = m_totalVertexes;
            mesh.NumVertexes = pMesh->mNumVertices;
            mesh.MaterialIndex = pMesh->mMaterialIndex;
            mesh.TransformMatrixIndex = m_transformMatrixes.size() - 1;

            m_totalVertexes += mesh.NumVertexes;
        }

        recursiveProcessAiMeshes(pNode->mChildren[i]);
    }
}

void Model3D_4::addVertexDatas(const aiMesh * const pMesh,
                                   QVector<VertexData> &vertexDatas, uint &vertexData_LastIndex)
{
    for (int i = 0; i < pMesh->mNumVertices; ++i) {
        QVector3D texCoords = pMesh->HasTextureCoords(i) ?
                    *(pMesh->mTextureCoords[i]) : QVector3D(0, 0, 0);

        VertexData vd(
                    *(pMesh->mVertices[i]),
                    texCoords.toVector2D(),
                    *(pMesh->mNormals[i])
                    );

        vertexDatas[vertexData_LastIndex++] = vd;
    }
}

void Model3D_4::addIndexes(const aiMesh * const pMesh,
                           QVector<uint> &indexes, uint &indexes_LastIndex)
{
    for (int k = 0; k < pMesh->mNumFaces; ++k) {
        const aiFace* const pFace = pMesh->mFaces[k];

        indexes[indexes_LastIndex++] = pFace[0];
        indexes[indexes_LastIndex++] = pFace[1];
        indexes[indexes_LastIndex++] = pFace[2];
    }
}

void Model3D_4::recursiveProcessAiNodes(
        const aiNode *pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors)
{
    m_transformMatrixes.push_back(transformMatrix);

    for (int i = 0; i < pNode->mNumChildren; ++i) {
        for (int j = 0; j < pNode->mNumMeshes; ++j) {
            const aiMesh* const pMesh = pNode->mMeshes[i];

            // Init Mesh
            Mesh& mesh = m_meshes[j];
            mesh.BaseIndex = m_totalVertexes;
            mesh.NumVertexes = pMesh->mNumVertices;
            mesh.MaterialIndex = pMesh->mMaterialIndex;
            mesh.TransformMatrixIndex = m_transformMatrixes.size() - 1;

            // Start process aiMesh

                addVertexDatas(pMesh, vectors.VertexDatas, vectors.VertexData_LastIndex);
                addIndexes(pMesh, vectors.Indexes, vectors.Indexes_LastIndex);

            // End process aiMesh

            m_totalVertexes += mesh.NumVertexes;
        }

        recursiveProcessAiNodes(
                    pNode->mChildren[i],
                    transformMatrix * pNode->mTransformation,
                    vectors);
    }
}
