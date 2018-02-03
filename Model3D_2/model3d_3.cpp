#include "model3d_3.h"


bool Model3D_3::loadMesh(const QString &filename)
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
        Ret = initFromScene(pScene, filename);
    }
    else {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
    }

    return Ret;
}

void Model3D_3::initFromScene(const aiScene *pScene, const std::string &filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_materials.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (uint i = 0 ; i < m_Entries.size() ; ++i) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    // Initialize materials
    if (pScene->HasMaterials()) {
        for (uint i = 0; i < pScene->mNumMaterials; ++i)
        {
            QSharedPointer<Material> material = initMaterial(pScene->mMaterials[i]);
            m_materials.append(material);
        }
    }
}

void Model3D_3::initMesh(unsigned int Index, const aiMesh *paiMesh)
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

QSharedPointer<Material> Model3D_3::initMaterial(const aiMaterial* material)
{
    QSharedPointer<Material> newMaterial(new Material);

    aiString mname;
    material->Get(AI_MATKEY_NAME, mname);
    if (mname.length > 0)
        newMaterial->Name = mname.C_Str();


    aiColor3D dif(0.f, 0.f, 0.f);
    aiColor3D amb(0.f, 0.f, 0.f);
    aiColor3D spec(0.f, 0.f, 0.f);
    float shine = 0.0;

    material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
    material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
    material->Get(AI_MATKEY_SHININESS, shine);




    newMaterial->Ambient = QVector3D{ amb.r, amb.g, amb.b };
    newMaterial->Diffuse = QVector3D{ dif.r, dif.g, dif.b };
    newMaterial->Specular = QVector3D{ spec.r, spec.g, spec.b };
    newMaterial->Shininess = shine;

    //newMaterial->Ambient *= .2;
    if (newMaterial->Shininess == 0.0)
        newMaterial->Shininess = 30;

    int texIndex = 0;
    aiString path;	// filename

    aiReturn texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
    while (texFound == AI_SUCCESS) {
        newMaterial->addTexture(aiTextureType, QImage(path.data));

        // Has more textures?
        texIndex++;
        texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
    }
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
