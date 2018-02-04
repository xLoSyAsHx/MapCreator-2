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

void Model3D_4::rotate(const QQuaternion &rotation)
{

}

void Model3D_4::translate(const QVector3D &translation)
{

}

void Model3D_4::scale(const float scaleKoef)
{

}

void Model3D_4::setGlobalTransform(const QMatrix4x4 &matrix)
{

}

void Model3D_4::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    m_VAO.bind();

    // Enable attribute arrays
    int location = program->attributeLocation("a_position");
    program->enableAttributeArray(location);

    location = program->attributeLocation("a_textCoord");
    program->enableAttributeArray(location);

    location = program->attributeLocation("a_normal");
    program->enableAttributeArray(location);

    for (int i = 0; i < m_meshes.size(); ++i) {
        const Mesh& mesh = m_meshes[i];

        // Set uniform values
        program->setUniformValue("u_modelMatrix", modelMatrix * m_transformMatrixes[mesh.TransformMatrixIndex]);


        int offset = 0;

        // Set varying attribute "a_position"
        program->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));


        offset += sizeof(QVector3D);
        // Set varying attribute "a_textCoord"
        program->setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));


        offset += sizeof(QVector2D);
        // Set varying attribute "a_normal"
        program->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Draw
        functions->glDrawElements(GL_TRIANGLES, mesh.NumIndexes, GL_UNSIGNED_INT, 0);

    }

    m_VAO.release();
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
            mesh.NumIndexes = pMesh->mNumFaces * 3;
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

    QOpenGLBuffer vertexDatasBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer indexesBuffer(QOpenGLBuffer::IndexBuffer);

    // Bind VertexData array
    vertexDatasBuffer.create();
    vertexDatasBuffer.bind();
    vertexDatasBuffer.allocate(vectors.VertexDatas.constData(),
                               vectors.VertexDatas.size() * sizeof(VertexData));


    // Bind index array
    indexesBuffer.create();
    indexesBuffer.bind();
    indexesBuffer.allocate(vectors.VertexDatas.constData(),
                           vectors.VertexDatas.size() * sizeof(uint));

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
