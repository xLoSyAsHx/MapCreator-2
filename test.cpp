#include "test.h"

Test::Test()
{

}

void Test::init(QString filename)
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





    int asd = m_v.size();
    m_v.resize(1);

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

void Test::draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions)
{
}

void Test::recursiveInitMeshesMaterialsTransforms(aiMesh **pMeshes, const aiNode *pNode, QMatrix4x4 transformMatrix)
{

}

bool Test::initFromScene(const aiScene *pScene)
{
    return true;
}

void Test::addVertexDatas(const aiMesh * const pMesh, QVector<VertexData> &vertexDatas, uint &vertexData_LastIndex)
{

}

void Test::addIndexes(const aiMesh * const pMesh, QVector<uint> &indexes, uint &indexes_LastIndex)
{

}

void Test::recursiveProcessAiNodes(const aiNode *pNode, QMatrix4x4 transformMatrix, Test::VectorsForShader &vectors)
{

}
