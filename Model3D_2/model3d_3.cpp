#include "model3d_3.h"


bool Model3D_3::LoadMesh(const QString &filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename.toStdString(),
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs);

    if (pScene) {
        Ret = InitFromScene(pScene, filename);
    }
    else {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
    }

    return Ret;
}

bool Model3D_3::InitFromScene(const aiScene *pScene, const std::string &Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

void Model3D_3::InitMesh(unsigned int Index, const aiMesh *paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

        QVector<VertexData> Vertices;
        QVector<unsigned int> Indices;

        const QVector3D Zero3D(0.0f, 0.0f, 0.0f);
        for (unsigned int i = 0; i< paiMesh->mNumVertices; i++){
            const aiVector3D& pos      = paiMesh->mVertices[i];
            const aiVector3D& normal   = paiMesh->mNormals[i];
            const aiVector3D& texCoord = paiMesh->HasTextureCoords(0) ?
                                paiMesh->mTextureCoords[0][i] : Zero3D;

            VertexData vd{pos, texCoord, normal};
            Vertices.push_back(vd);
        }

        for (unsigned int i = 0; i < paiMesh->mNumFaces; i++){
            const aiFace& Face = paiMesh->mFaces[i];

            assert(Face.mNumIndices == 3);
            Indices.push_back(Face.mIndices[0]);
            Indices.push_back(Face.mIndices[1]);
            Indices.push_back(Face.mIndices[2]);
        }

        m_Entries[Index].Init(Vertices, Indices);
}

bool Model3D_3::MeshEntry::Init(const QVector<VertexData> &vertices, QVector<uint> &indices)
{
    NumIndices = indices.size();

    m_VB.create();
    m_VB.bind();
    m_VB.allocate(vertices.data(), sizeof(VertexData) * vertices.size());
    m_VB.release();

    m_IB.create();
    m_IB.bind();
    m_IB.allocate(indices.data(), sizeof(uint) * indices.size());
    m_IB.release();

    return true;
}
