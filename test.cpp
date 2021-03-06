#include "test.h"



Test::Test() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)

{

}

bool Test::loadFromFile(QString filename)
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

void Test::rotate(const QQuaternion &rotation)
{

}

void Test::translate(const QVector3D &translation)
{

}

void Test::scale(const float scaleKoef)
{

}

void Test::setGlobalTransform(const QMatrix4x4 &matrix)
{

}

void Test::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    //m_VAO.bind();
    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

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

    //m_VAO.release();
    // Release
    m_indexBuffer.release();
    m_vertexBuffer.release();
}

void Test::clear()
{
    //if (m_vertexBuffer.isCreated())
        //m_vertexBuffer.destroy();

    //if (m_indexBuffer.isCreated())
        //m_indexBuffer.destroy();
    //if (m_VAO.isCreated())
        //m_VAO.destroy();

    m_totalVertexes = 0;

    QVector<Mesh> v;
    int asd = v.size();
    int asd2 = m_v.size();

    m_meshes.resize(1);

    if (!m_meshes.isEmpty())
        m_meshes.clear();

    if (!m_materials.isEmpty())
        m_materials.clear();

    if (!m_transformMatrixes.isEmpty())
        m_transformMatrixes.clear();

}

void Test::recursiveInitMeshesMaterialsTransforms(
        aiMesh **pMeshes,
        const aiNode *pNode, QMatrix4x4 transformMatrix)
{
    m_transformMatrixes.push_back(transformMatrix);

    for (int i = 0; i < pNode->mNumChildren; ++i) {
        for (int j = 0; j < pNode->mNumMeshes; ++j) {
            const aiMesh* pMesh = pMeshes[pNode->mMeshes[i]];
            Mesh& mesh = m_meshes[i];

            mesh.BaseIndex = m_totalVertexes;
            mesh.NumVertexes = pMesh->mNumVertices;
            mesh.NumIndexes = pMesh->mNumFaces * 3;
            mesh.MaterialIndex = pMesh->mMaterialIndex;
            mesh.TransformMatrixIndex = m_transformMatrixes.size() - 1;

            m_totalVertexes += mesh.NumVertexes;
        }

        recursiveInitMeshesMaterialsTransforms(
                    pMeshes,
                    pNode->mChildren[i], transformMatrix * toQMatrix4x4(pNode->mTransformation));
    }
}

bool Test::initFromScene(const aiScene *pScene)
{
    clear();

    //m_VAO.create();
    //m_VAO.bind();


    m_meshes.resize(pScene->mNumMeshes);
    m_materials.resize(pScene->mNumMaterials);
    m_transformMatrixes.resize(0);

    recursiveInitMeshesMaterialsTransforms(
                pScene->mMeshes,
                pScene->mRootNode, toQMatrix4x4(pScene->mRootNode->mTransformation));

    // Vectors of vertex data
    QVector<VertexData> vertexesData;
    vertexesData.resize(m_totalVertexes);


    // Init meshes through node hierarchy
    VectorsForShader vectors(pScene->mMeshes, pScene->mNumMeshes);
    recursiveProcessAiNodes(pScene->mRootNode,
                            toQMatrix4x4(pScene->mRootNode->mTransformation),
                            vectors);

    //QOpenGLBuffer vertexDatasBuffer(QOpenGLBuffer::VertexBuffer);
    //QOpenGLBuffer indexesBuffer(QOpenGLBuffer::IndexBuffer);



    // Bind VertexData array
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vectors.VertexDatas.constData(),
                               vectors.VertexDatas.size() * sizeof(VertexData));
    m_vertexBuffer.release();


    // Bind index array
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(vectors.VertexDatas.constData(),
                           vectors.VertexDatas.size() * sizeof(uint));
    m_indexBuffer.release();

    //m_VAO.release();

    return true;
}

void Test::addVertexDatas(const aiMesh * const pMesh,
                                   QVector<VertexData> &vertexDatas, uint &vertexData_LastIndex)
{
    for (int i = 0; i < pMesh->mNumVertices; ++i) {
        QVector3D texCoords = pMesh->HasTextureCoords(i) ?
                    toQVector3D(*(pMesh->mTextureCoords[i])) : QVector3D(0, 0, 0);


        VertexData vd(
                    toQVector3D(pMesh->mVertices[i]),
                    texCoords.toVector2D(),
                    toQVector3D(pMesh->mNormals[i])
                    );

        vertexDatas[vertexData_LastIndex++] = vd;
    }
}

void Test::addIndexes(const aiMesh * const pMesh,
                           QVector<uint> &indexes, uint &indexes_LastIndex)
{
    for (int k = 0; k < pMesh->mNumFaces; ++k) {
        const aiFace& pFace = pMesh->mFaces[k];

        indexes[indexes_LastIndex++] = pFace.mIndices[0];
        indexes[indexes_LastIndex++] = pFace.mIndices[1];
        indexes[indexes_LastIndex++] = pFace.mIndices[2];
    }
}

void Test::recursiveProcessAiNodes(
        const aiNode *pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors)
{
    m_transformMatrixes.push_back(transformMatrix);

    for (int i = 0; i < pNode->mNumChildren; ++i) {
        for (int j = 0; j < pNode->mNumMeshes; ++j) {
            const aiMesh* const pMesh =  vectors.Meshes[pNode->mMeshes[i]];

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
                    transformMatrix * toQMatrix4x4(pNode->mTransformation),
                    vectors);
    }
}
