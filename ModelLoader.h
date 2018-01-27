#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <QSharedPointer>

#include "Model3D.h"




class ModelLoader final
{

public:

    static Model3D* loadFromFile(QString path);


private:




	/** Methods */

    static QSharedPointer<MaterialInfo> processMaterial(aiMaterial *material);

    static QSharedPointer<Mesh> processMesh(Model3D* model, aiMesh *mesh);

    static void processNode(Model3D* model,
            const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);


};

#endif MODEL_LOADER_H
