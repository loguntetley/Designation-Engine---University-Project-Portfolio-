#include "MeshModel.h"

#pragma region Constructors

MeshModel::MeshModel()
{
}

MeshModel::MeshModel(std::vector<Mesh> _meshList)
	: m_MeshList(_meshList), m_Model(glm::mat4(1.0f))
{
	DN_CORE_INFO_OUTPUT("Constructing mesh model");
}

MeshModel::~MeshModel()
{
}

#pragma endregion

#pragma region Setter Functions

void MeshModel::SetModel(glm::mat4 _model)
{
	m_Model = _model;
}

#pragma endregion

#pragma region Getter Functions

size_t MeshModel::GetMeshCount() const
{
	return m_MeshList.size();
}

Mesh* MeshModel::GetMesh(size_t _index)
{
	if (_index >= m_MeshList.size())
	{
		DN_CORE_ERROR_OUTPUT("Attempted to access invalid mesh index");
		throw std::runtime_error("");
	}
		
	return &m_MeshList[_index];
}

glm::mat4 MeshModel::GetModel() const
{
	return m_Model;
}

#pragma endregion

#pragma region Load Functions

std::vector<std::string> MeshModel::LoadMaterials(const aiScene* _scene)
{
	DN_CORE_INFO_OUTPUT("Loading materials");
	//SIZED TEXTURE//	
	std::vector<std::string> textureList(_scene->mNumMaterials);

	for (size_t i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial* material = _scene->mMaterials[i];
		textureList[i] = "";

		//DIFFUSE TEXTURE CHECK//
		DN_CORE_INFO_OUTPUT("Checking diffuse texture");
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			//TEXTURE FILE PATH//
			DN_CORE_INFO_OUTPUT("Checking texture file path");
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);
				textureList[i] = fileName;
			}
		}
	}
	DN_CORE_SUCCESS_OUTPUT("Texture list loaded");

	return textureList;
}

std::vector<Mesh> MeshModel::LoadNode(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, aiNode* _node, const aiScene* _scene, std::vector<int> _materialToTexture)
{
	//NODE TREE//
	std::vector<Mesh> meshList;

	for (size_t i = 0; i < _node->mNumMeshes; i++)
		meshList.push_back(LoadMesh(_physicalDevice, _logicalDevice, _transferQueue, _transferCommandPool, _scene->mMeshes[_node->mMeshes[i]], _scene, _materialToTexture));

	for (size_t i = 0; i < _node->mNumChildren; i++)
	{
		std::vector<Mesh> newList = LoadNode(_physicalDevice, _logicalDevice, _transferQueue, _transferCommandPool, _node->mChildren[i], _scene, _materialToTexture);
		meshList.insert(meshList.end(), newList.begin(), newList.end());
	}

	return meshList;
}

Mesh MeshModel::LoadMesh(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, aiMesh* _mesh, const aiScene* _scene, std::vector<int> _materialToTexture)
{
	DN_CORE_INFO_OUTPUT("Loading mesh");
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	vertices.resize(_mesh->mNumVertices);

	//COPY VETEX DATA//
	DN_CORE_INFO_OUTPUT("Copying vertex data");
	for (size_t i = 0; i < _mesh->mNumVertices; i++)
	{
		vertices[i].m_Position = { _mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z };

		if (_mesh->mTextureCoords[0])
			vertices[i].m_Texture = { _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y };
		else
			vertices[i].m_Texture = { 0.0f, 0.0f };

		vertices[i].m_Colour = { 1.0f, 1.0f, 1.0f };
	}
	DN_CORE_SUCCESS_OUTPUT("Vertex data copied");

	//COPY FACES DATA//
	DN_CORE_INFO_OUTPUT("Copying faces data");
	for (size_t i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);	
	}
	DN_CORE_SUCCESS_OUTPUT("Faces data copied");

	Mesh mesh = Mesh(_physicalDevice, _logicalDevice, _transferQueue, _transferCommandPool, &vertices, &indices, _materialToTexture[_mesh->mMaterialIndex]);
	DN_CORE_SUCCESS_OUTPUT("Mesh loaded");

	return mesh;
}

#pragma endregion

#pragma region Managment Functions

void MeshModel::DestroyMeshModel()
{
	for (auto& _mesh : m_MeshList)
		_mesh.DestroyBuffers();
}

#pragma endregion










