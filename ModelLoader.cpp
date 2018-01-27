#include "ModelLoader.h"



QSharedPointer<MaterialInfo> ModelLoader::processMaterial(aiMaterial *material) {
	QSharedPointer<MaterialInfo> newMaterial(new MaterialInfo);

	aiString mname;
	material->Get(AI_MATKEY_NAME, mname);
	if (mname.length > 0)
		newMaterial->Name = mname.C_Str();

	int shadingModel;
	material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

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

	return newMaterial;
}

QSharedPointer<Mesh> ModelLoader::processMesh(Model3D* model, aiMesh *mesh) {
	QSharedPointer<Mesh> newMesh(new Mesh);

	newMesh->name = mesh->mName.length != 0 ? mesh->mName.C_Str() : "";
	newMesh->indexOffset = model->m_indices.size();
	unsigned int indexCountBefore = model->m_indices.size();
	int vertindexoffset = model->m_vertices.size() / 3;

	// Get Vertices
	if (mesh->mNumVertices > 0)
	{
		for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
		{
			aiVector3D &vec = mesh->mVertices[ii];

			model->m_vertices.push_back(vec.x);
			model->m_vertices.push_back(vec.y);
			model->m_vertices.push_back(vec.z);
		}
	}

	// Get Normals
	if (mesh->HasNormals())
	{
		for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
		{
			aiVector3D &vec = mesh->mNormals[ii];
			model->m_normals.push_back(vec.x);
			model->m_normals.push_back(vec.y);
			model->m_normals.push_back(vec.z);
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

		model->m_indices.push_back(face->mIndices[0] + vertindexoffset);
		model->m_indices.push_back(face->mIndices[1] + vertindexoffset);
		model->m_indices.push_back(face->mIndices[2] + vertindexoffset);
	}

	newMesh->indexCount = model->m_indices.size() - indexCountBefore;
	newMesh->material = model->m_materials.at(mesh->mMaterialIndex);

	return newMesh;
}

void ModelLoader::processNode(Model3D *model,
    const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode)
{
	newNode.name = node->mName.length != 0 ? node->mName.C_Str() : "";

	//newNode.transformation = QMatrix4x4(node->mTransformation[0]);
	newNode.transformation = QMatrix4x4(node->mTransformation[0]);

	newNode.meshes.resize(node->mNumMeshes);
	for (uint imesh = 0; imesh < node->mNumMeshes; ++imesh)
	{
		QSharedPointer<Mesh> mesh = model->m_meshes[node->mMeshes[imesh]];
		newNode.meshes[imesh] = mesh;
	}

	for (uint ich = 0; ich < node->mNumChildren; ++ich)
	{
		newNode.nodes.push_back(Node());
		processNode(model, scene, node->mChildren[ich], parentNode, newNode.nodes[ich]);
	}
}


Model3D* ModelLoader::loadFromFile(QString path)
{

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path.toStdString(),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (scene == nullptr) {
		qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return nullptr;
	}

	Model3D *model = new Model3D();

	// Process material
	if (scene->HasMaterials()) {
		for (unsigned int ii = 0; ii < scene->mNumMaterials; ++ii)
		{
            QSharedPointer<MaterialInfo> material = processMaterial(scene->mMaterials[ii]);
			model->m_materials.append(material);
		}
	}

	// Process meshes
	if (scene->HasMeshes()) {
		for (int i = 0; i < scene->mNumMeshes; ++i) {
            model->m_meshes.append(processMesh(model, scene->mMeshes[i]));
		}
	}
	else
	{
		qDebug() << "Error loading model";
        return nullptr;
	}

	// Process nodes
	if (scene->mRootNode != NULL) {
		Node *rootNode = new Node;

        processNode(model, scene, scene->mRootNode, 0, *rootNode);
		model->m_rootNode.reset(rootNode);
	}
	else
	{
		qDebug() << "Error loading model";
        return nullptr;
	}

    return model;
}
