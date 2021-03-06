#include "Model.h"

void Model::LoadModel(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | 
		aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		printf("Model (%s) failed to load: %s", filename.c_str(), importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		else
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size()); 
	//Vectors have the same memory structures as arrays. Passing the address of the first element of the vector
	//is same as passing the name of the array.
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		textureList[i] = nullptr;
		aiMaterial* material = scene->mMaterials[i];
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				if (idx == std::string::npos)
					printf("%s file not found. Check the delimiter.\n", std::string(path.data).c_str());
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string texPath = std::string("textures/") + filename;
				textureList[i] = new Texture(texPath.c_str());
				if (!textureList[i]->LoadTexture())
				{
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		if (!textureList[i])
		{
			textureList[i] = new Texture("./textures/default.png");
			textureList[i]->LoadTexture();
		}
	}
}

void Model::RenderModel()
{
	for (unsigned int i = 0; i < meshList.size(); ++i)
	{
		if (meshToTex[i] < textureList.size() && textureList[meshToTex[i]])
			textureList[meshToTex[i]]->UseTexture();
		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (unsigned int i = 0; i < meshList.size(); ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	for (unsigned int i = 0; i < textureList.size(); ++i)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
	ClearModel();
}
