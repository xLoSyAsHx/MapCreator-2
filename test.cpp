#ifndef TEST_H
#define TEST_H


#include <QString>
#include <QMatrix4x4>

#include <External/assimp/include/assimp/Importer.hpp>
#include <External/assimp/include/assimp/scene.h>
#include <External/assimp/include/assimp/postprocess.h>




struct MaterialInfo
{
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};




struct LightInfo
{
    QVector4D Position;
    QVector3D Intensity;
};


struct Mesh
{
    QString name;
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};


struct Node
{
    QString name;
    QMatrix4x4 transformation;
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};





class Test {


   QVector<float> m_vertices;
   QVector<float> m_normals;
   QVector<unsigned int> m_indices;

   QVector<QSharedPointer<MaterialInfo>> m_materials;
   QVector<QSharedPointer<Mesh>> m_meshes;
   QSharedPointer<Node> m_rootNode;
public:

    bool LoadFromFBX(QString path) {
        Assimp::Importer importer;

        std::string str = path.toStdString();
        const aiScene *scene = importer.ReadFile(str.c_str(),
                                                   aiProcess_GenSmoothNormals |
                                                   aiProcess_CalcTangentSpace |
                                                   aiProcess_Triangulate |
                                                   aiProcess_JoinIdenticalVertices |
                                                   aiProcess_SortByPType);

        if (scene == nullptr) {
            qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
            return false;
        }

        // Process material
        if (scene->HasMaterials()) {
            for (unsigned int ii = 0; ii < scene->mNumMaterials; ++ii)
            {
                QSharedPointer<MaterialInfo> material = processMaterial(scene->mMaterials[ii]);
                m_materials.push_back(material);
            }
        }

        // Process meshes
        if (scene->HasMeshes()) {
            for (uint i = 0; i < scene->mNumMeshes; ++i) {
                m_meshes.append(processMesh(scene->mMeshes[i]));
            }
        }
        else
        {
            qDebug() << "Error loading model";
            return false;
        }

        // Process nodes
        if (scene->mRootNode != NULL) {
            Node *rootNode = new Node;
            processNode(scene, scene->mRootNode, 0, *rootNode);
            m_rootNode.reset(rootNode);
        }
        else
        {
            qDebug() << "Error loading model";
            return false;
        }

        return true;
    }

    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *material) {
        QSharedPointer<MaterialInfo> newMaterial(new MaterialInfo);

        aiString mname;
        material->Get(AI_MATKEY_NAME, mname);
        if (mname.length > 0)
           newMaterial->Name = mname.C_Str();

        int shadingModel;
        aiReturn ret = material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
        if (ret != AI_SUCCESS)
            qDebug() << "can't get material";

        if (shadingModel != aiShadingMode_Phong)// && shadingModel != aiShadingMode_Gouraud)
        {
           qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
           newMaterial->Name = "DefaultMaterial";
        }
        else
        {
            aiColor3D  dif(0.0f,0.0f,0.0f);
            aiColor3D  amb(0.0f,0.0f,0.0f);
            aiColor3D spec(0.0f,0.0f,0.0f);
            float shine = 0.0;

            material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
            material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            material->Get(AI_MATKEY_SHININESS, shine);

            newMaterial->Ambient = QVector3D(amb.r, amb.g, amb.b);
            newMaterial->Diffuse = QVector3D(dif.r, dif.g, dif.b);
            newMaterial->Specular = QVector3D(spec.r, spec.g, spec.b);
            newMaterial->Shininess = shine;

            newMaterial->Ambient *= 0.2f;
            if (newMaterial->Shininess == 0.0)
                newMaterial->Shininess = 30;
        }

        return newMaterial;
    }

    QSharedPointer<Mesh> processMesh(aiMesh *mesh) {
        QSharedPointer<Mesh> newMesh(new Mesh);

        newMesh->name = mesh->mName.length != 0 ? mesh->mName.C_Str() : "";
        newMesh->indexOffset = m_indices.size();
        unsigned int indexCountBefore = m_indices.size();
        int vertindexoffset = m_vertices.size()/3;

        // Get Vertices
        if (mesh->mNumVertices > 0)
        {
            for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
            {
                aiVector3D &vec = mesh->mVertices[ii];

                m_vertices.push_back(vec.x);
                m_vertices.push_back(vec.y);
                m_vertices.push_back(vec.z);
            }
        }

        // Get Normals
        if (mesh->HasNormals())
        {
            for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
            {
                aiVector3D &vec = mesh->mNormals[ii];
                m_normals.push_back(vec.x);
                m_normals.push_back(vec.y);
                m_normals.push_back(vec.z);
            };
        }

        // Get mesh indexes
        for (uint t = 0; t < mesh->mNumFaces; ++t)
        {
            aiFace* face = &mesh->mFaces[t];
            if (face->mNumIndices != 3)
            {
                qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive.";
                continue;
            }

            m_indices.push_back(face->mIndices[0] + vertindexoffset);
            m_indices.push_back(face->mIndices[1] + vertindexoffset);
            m_indices.push_back(face->mIndices[2] + vertindexoffset);
        }

        newMesh->indexCount = m_indices.size() - indexCountBefore;
        newMesh->material = m_materials.at(mesh->mMaterialIndex);

        return newMesh;
    }

    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode)
    {
        newNode.name = node->mName.length != 0 ? node->mName.C_Str() : "";

        newNode.transformation = QMatrix4x4(node->mTransformation[0]);

        newNode.meshes.resize(node->mNumMeshes);
        for (uint imesh = 0; imesh < node->mNumMeshes; ++imesh)
        {
            QSharedPointer<Mesh> mesh = m_meshes[node->mMeshes[imesh]];
            newNode.meshes[imesh] = mesh;
        }

        for (uint ich = 0; ich < node->mNumChildren; ++ich)
        {
            newNode.nodes.push_back(Node());
            processNode(scene, node->mChildren[ich], parentNode, newNode.nodes[ich]);
        }
    }
};


#endif
