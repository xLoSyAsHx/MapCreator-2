
#include "model3d.h"

#include "assimp_adapter.h"

#include "camera3d.h"

Model3D::Model3D(QVector4D objId) :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_vertexBuffer_SelectionCube(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer_SelectionCube(QOpenGLBuffer::IndexBuffer),
    m_scale(QVector3D(1, 1, 1)),
    m_objId(objId)
{
    Camera3D& camera = Camera3D::Instance();
    m_translate = camera.getPosition() + camera.getDirection() * 50;
    QVector3D dir = camera.getDirection();
    m_translate = QVector3D(qRound(m_translate.x()), qRound(m_translate.y()), qRound(m_translate.z()));
}

Model3D::~Model3D()
{
    clear();
}

bool Model3D::loadFromFile(QString filename)
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

void Model3D::rotate(const QQuaternion &rotation)
{
    m_rotate = rotation * m_rotate;
}

void Model3D::translate(const QVector3D &translation)
{
    m_translate += translation;
}

void Model3D::scale(const float scaleKoef)
{
    m_scale *= scaleKoef;
}

void Model3D::setGlobalTransform(const QMatrix4x4 &matrix)
{
    m_globalTransform = matrix;
}

void Model3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);


    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    // Enable attribute arrays
    int loc_position = program->attributeLocation("a_position");
    program->enableAttributeArray(loc_position);

    int loc_textCoord = program->attributeLocation("a_textCoord");
    program->enableAttributeArray(loc_textCoord);

    int loc_normal = program->attributeLocation("a_normal");
    program->enableAttributeArray(loc_normal);

    for (int i = 0; i < m_meshes.size(); ++i) {
        const Mesh& mesh = m_meshes[i];
        const QSharedPointer<Material>& material = m_materials[mesh.MaterialIndex];

        // Set uniform values
        program->setUniformValue("u_modelMatrix", modelMatrix * m_transformMatrixes[mesh.TransformMatrixIndex]);
        program->setUniformValue("u_isLandscape", 0);


        if (material->bind(aiTextureType_DIFFUSE, 0))
        {
            program->setUniformValue("u_texture0", 0);
            program->setUniformValue("u_hasTexture", 1);
        }
        else
        {
            program->setUniformValue("u_hasTexture", 0);

            program->setUniformValue("u_diffuse", material->Diffuse);
            program->setUniformValue("u_ambient", material->Ambient);
            program->setUniformValue("u_specular", material->Specular);
            program->setUniformValue("u_shininess", material->Shininess);
        }


        int offset = 0;

        // Set varying attribute "a_position"
        program->setAttributeBuffer(loc_position, GL_FLOAT, offset, 3, sizeof(VertexData));


        offset += sizeof(QVector3D);
        // Set varying attribute "a_textCoord"
        program->setAttributeBuffer(loc_textCoord, GL_FLOAT, offset, 2, sizeof(VertexData));


        offset += sizeof(QVector2D);
        // Set varying attribute "a_normal"
        program->setAttributeBuffer(loc_normal, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Draw
        functions->glDrawElements(GL_TRIANGLES, mesh.NumIndexes, GL_UNSIGNED_INT, (GLvoid*)(sizeof(uint) * mesh.BaseIndex));

        if (material->isBound(aiTextureType_DIFFUSE))
            material->release(aiTextureType_DIFFUSE);
    }

    // Release
    m_indexBuffer.release();
    m_vertexBuffer.release();
}

void Model3D::drawSelectionCubes(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (isSelected() == false)
        return;

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);


    // Bind buffers
    m_vertexBuffer_SelectionCube.bind();
    m_indexBuffer_SelectionCube.bind();

    program->setUniformValue("u_modelMatrix", modelMatrix);

    // Enable attribute arrays
    int loc_position = program->attributeLocation("a_position");
    program->enableAttributeArray(loc_position);



    // Set varying attribute "a_position"
    program->setAttributeBuffer(loc_position, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Draw
    functions->glDrawElements(GL_LINES, m_indexBuffer_SelectionCube.size(), GL_UNSIGNED_INT, 0);


    // Release buffers
    m_vertexBuffer_SelectionCube.release();
    m_indexBuffer_SelectionCube.release();
}

void Model3D::objectPicking(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);


    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    // Enable attribute arrays
    int loc_position = program->attributeLocation("a_position");
    program->enableAttributeArray(loc_position);

    int loc_textCoord = program->attributeLocation("a_textCoord");
    program->enableAttributeArray(loc_textCoord);

    int loc_normal = program->attributeLocation("a_normal");
    program->enableAttributeArray(loc_normal);

    int loc_id = program->attributeLocation("a_id");
    program->enableAttributeArray(loc_id);

    for (int i = 0; i < m_meshes.size(); ++i) {
        const Mesh& mesh = m_meshes[i];

        // Set uniform values
        program->setUniformValue("u_modelMatrix", modelMatrix * m_transformMatrixes[mesh.TransformMatrixIndex]);

        int offset = 0;

        // Set varying attribute "a_position"
        program->setAttributeBuffer(loc_position, GL_FLOAT, offset, 3, sizeof(VertexData));


        offset += sizeof(QVector3D);
        // Set varying attribute "a_textCoord"
        program->setAttributeBuffer(loc_textCoord, GL_FLOAT, offset, 2, sizeof(VertexData));


        offset += sizeof(QVector2D);
        // Set varying attribute "a_normal"
        program->setAttributeBuffer(loc_normal, GL_FLOAT, offset, 3, sizeof(VertexData));

        offset += sizeof(QVector3D);
        // Set varying attribute "a_id"
        program->setAttributeBuffer(loc_id, GL_FLOAT, offset, 4, sizeof(VertexData));


        // Draw
        functions->glDrawElements(GL_TRIANGLES, mesh.NumIndexes, GL_UNSIGNED_INT, (GLvoid*)(sizeof(uint) * mesh.BaseIndex));
    }

    // Release
    m_indexBuffer.release();
    m_vertexBuffer.release();
}

QVector3D Model3D::getPosition() const
{
    return m_translate;
}

void Model3D::clear()
{
    m_globalTransform.setToIdentity();

    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (!m_meshes.isEmpty())
        m_meshes.clear();

    if (!m_materials.isEmpty())
        m_materials.clear();

    if (!m_transformMatrixes.isEmpty())
        m_transformMatrixes.clear();


}

bool Model3D::initFromScene(const aiScene *pScene)
{
    clear();

    m_meshes.resize(pScene->mNumMeshes);
    m_materials.resize(pScene->mNumMaterials);
    m_transformMatrixes.resize(0);

    initMaterials(pScene);

    // Init meshes through node hierarchy
    VectorsForShader vectors(pScene->mMeshes, pScene->mNumMeshes);
    recursiveProcessAiNodes(pScene->mRootNode,
                            toQMatrix4x4(pScene->mRootNode->mTransformation),
                            vectors);

    /* Determinate cube min and max corners minCorner(minX, minY, minZ) maxCorner(maxX, maxY, maxZ)
     * for futher use when creating cube for visualise selection
    */
    determinateSelectionCube(vectors);

    // Bind VertexData array
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vectors.VertexDatas.constData(),
                               vectors.VertexDatas.size() * sizeof(VertexData));
    m_vertexBuffer.release();


    // Bind index array
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(vectors.Indexes.constData(),
                           vectors.Indexes.size() * sizeof(uint));
    m_indexBuffer.release();

    return true;
}

void Model3D::determinateSelectionCube(VectorsForShader& vectors)
{
    QVector3D minCorner, maxCorner;
    bool isFirst = true;


    for (int i = 0; i < m_meshes.size(); ++i) {
        const Mesh& mesh = m_meshes[i];
        const QMatrix4x4& transformMatrix = m_transformMatrixes[mesh.TransformMatrixIndex];

        for (int j = mesh.BaseVertex; j < mesh.BaseVertex + mesh.NumVertexes; ++j)
        {
            QVector4D realPos = transformMatrix * QVector4D(vectors.VertexDatas[j].position, 1);

            if (isFirst)
            {
                minCorner = maxCorner = QVector3D(realPos.x(), realPos.y(), realPos.z());
                isFirst = false;
            }

            if (realPos.x() > maxCorner.x())
                maxCorner.setX(realPos.x());
            else if (realPos.x() < minCorner.x())
                minCorner.setX(realPos.x());

            if (realPos.y() > maxCorner.y())
                maxCorner.setY(realPos.y());
            else if (realPos.y() < minCorner.y())
                minCorner.setY(realPos.y());

            if (realPos.z() > maxCorner.z())
                maxCorner.setZ(realPos.z());
            else if (realPos.z() < minCorner.z())
                minCorner.setZ(realPos.z());
        }
    }


    minCorner.setX(minCorner.x() - 0.5f);
    minCorner.setZ(minCorner.z() - 0.5f);

    maxCorner.setX(maxCorner.x() + 0.5f);
    maxCorner.setZ(maxCorner.z() + 0.5f);

    QVector<QVector3D> vertexes;
    QVector<uint> indexes;

    vertexes.resize(8);
    indexes.resize(24);

    vertexes[0] = minCorner;
    vertexes[1] = QVector3D(minCorner.x(), minCorner.y(), maxCorner.z());
    vertexes[2] = QVector3D(maxCorner.x(), minCorner.y(), maxCorner.z());
    vertexes[3] = QVector3D(maxCorner.x(), minCorner.y(), minCorner.z());
    vertexes[4] = QVector3D(minCorner.x(), maxCorner.y(), minCorner.z());
    vertexes[5] = QVector3D(minCorner.x(), maxCorner.y(), maxCorner.z());
    vertexes[6] = QVector3D(maxCorner.x(), maxCorner.y(), maxCorner.z());
    vertexes[7] = QVector3D(maxCorner.x(), maxCorner.y(), minCorner.z());

    // Bottom
    indexes[0] = 0;
    indexes[1] = 1;
    indexes[2] = 1;
    indexes[3] = 2;
    indexes[4] = 2;
    indexes[5] = 3;
    indexes[6] = 3;
    indexes[7] = 0;

    // Top
    indexes[8]  = 4;
    indexes[9]  = 5;
    indexes[10] = 5;
    indexes[11] = 6;
    indexes[12] = 6;
    indexes[13] = 7;
    indexes[14] = 7;
    indexes[15] = 4;

    // Corners

    indexes[16] = 0;
    indexes[17] = 4;
    indexes[18] = 1;
    indexes[19] = 5;
    indexes[20] = 2;
    indexes[21] = 6;
    indexes[22] = 3;
    indexes[23] = 7;

    // Bind VertexData array
    m_vertexBuffer_SelectionCube.create();
    m_vertexBuffer_SelectionCube.bind();
    m_vertexBuffer_SelectionCube.allocate(vertexes.constData(), vertexes.size() * sizeof(QVector3D));
    m_vertexBuffer_SelectionCube.release();


    // Bind index array
    m_indexBuffer_SelectionCube.create();
    m_indexBuffer_SelectionCube.bind();
    m_indexBuffer_SelectionCube.allocate(indexes.constData(), indexes.size() * sizeof(uint));
    m_indexBuffer_SelectionCube.release();
}

void Model3D::addVertexDatas(const aiMesh * const pMesh,
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
        vd.id = m_objId;

        vertexDatas[vertexData_LastIndex++] = vd;
    }
}

void Model3D::addIndexes(const aiMesh * const pMesh,
                           QVector<uint> &indexes, uint &indexes_LastIndex, uint shift)
{
    for (int k = 0; k < pMesh->mNumFaces; ++k) {
        const aiFace& pFace = pMesh->mFaces[k];

        indexes[indexes_LastIndex++] = pFace.mIndices[0] + shift;
        indexes[indexes_LastIndex++] = pFace.mIndices[1] + shift;
        indexes[indexes_LastIndex++] = pFace.mIndices[2] + shift;
    }
}

void Model3D::recursiveProcessAiNodes(
        const aiNode *pNode, QMatrix4x4 transformMatrix, VectorsForShader& vectors, uint lastMeshIndex)
{
    static uint totalIndexes = 0;
    static uint totalVertexes = 0;

    m_transformMatrixes.push_back(transformMatrix);


    for (int i = 0; i < pNode->mNumMeshes; ++i) {
        const aiMesh* const pMesh =  vectors.Meshes[pNode->mMeshes[i]];

        // Init Mesh
        Mesh& mesh = m_meshes[lastMeshIndex++];
        mesh.BaseIndex = totalIndexes;
        mesh.BaseVertex = totalVertexes;
        mesh.NumIndexes = pMesh->mNumFaces * 3;
        mesh.NumVertexes = pMesh->mNumVertices;
        mesh.MaterialIndex = pMesh->mMaterialIndex;
        mesh.TransformMatrixIndex = m_transformMatrixes.size() - 1;

        // Start process aiMesh

            addVertexDatas(pMesh, vectors.VertexDatas, vectors.VertexData_LastIndex);
            addIndexes(pMesh, vectors.Indexes, vectors.Indexes_LastIndex, mesh.BaseVertex);

        // End process aiMesh

        totalIndexes += mesh.NumIndexes;
        totalVertexes += mesh.NumVertexes;
    }

    for (int i = 0; i < pNode->mNumChildren; ++i) {
        recursiveProcessAiNodes(
                    pNode->mChildren[i],
                    transformMatrix * toQMatrix4x4(pNode->mTransformation),
                    vectors,
                    lastMeshIndex);
    }

    totalIndexes = 0;
    totalVertexes = 0;
}

void Model3D::initMaterials(const aiScene *pScene)
{


    for (unsigned int i = 0; i< pScene->mNumMaterials; i++) {
        QSharedPointer<Material> material(new Material());

        const aiMaterial* pMaterial = pScene->mMaterials[i];


        QVector<aiTextureType> tTypes = {
            aiTextureType_NONE,
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_AMBIENT,
            aiTextureType_EMISSIVE,
            aiTextureType_HEIGHT,
            aiTextureType_NORMALS,
            aiTextureType_SHININESS,
            aiTextureType_OPACITY,
            aiTextureType_DISPLACEMENT,
            aiTextureType_LIGHTMAP,
            aiTextureType_REFLECTION,
            aiTextureType_UNKNOWN
        };
        QVector<int> t;
        for (auto it = tTypes.begin(); it != tTypes.end(); ++it)
            t.push_back(pMaterial->GetTextureCount(*it));


        // Get material properties
        aiColor3D color;
        // Diffuse factor
        if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
            material->Diffuse = toQVector3D(color);
        else
            material->Diffuse = QVector3D(1.0f, 1.0f, 1.0f);

        // Ambient factor
        if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
            material->Ambient = toQVector3D(color);
        else
            material->Ambient = QVector3D(1.0f, 1.0f, 1.0f);

        // Specular factor
        if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
            material->Specular = toQVector3D(color);
        else
            material->Specular = QVector3D(1.0f, 1.0f, 1.0f);

        // Specular factor
        float shininess;
        if (pMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
            material->Shininess = shininess;
        else
            material->Shininess = 0.0f;
        // End getting material properties


        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path,
                                NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
//                    std::string FullPath = Dir + "/" + Path.data;
//                    m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

//                    if (!m_Textures[i]->Load()){
//                        printf("Error loading texture '%s'\n", FullPath.c_str());
//                        delete m_Textures[i];
//                        m_Textures[i] = NULL;
//                        Ret = false;
//                    }

                qDebug() << "Loaded object has texture. BUT THEY DONT HANDELED!!!";
            }
        }

        m_materials[i] = material;
    }
}
