#pragma once
#include "PCH.h"

#include <assimp/scene.h>

#include "Mesh.h"

class MeshModel
{
private:
	std::vector<Mesh> m_MeshList;
	glm::mat4 m_Model;

public:
	//CONSTRUCTORS//
	MeshModel();
	MeshModel(std::vector<Mesh> _meshList);
	~MeshModel();

	//SETTER FUNCTIONS//
	void SetModel(glm::mat4 _model);

	//GETTER FUNCTIONS//
	size_t GetMeshCount() const;
	Mesh* GetMesh(size_t _index);
	glm::mat4 GetModel() const;

	//MANAGMENT FUNCTIONS
	void DestroyMeshModel();

	//LOAD FUNCTIONS//
	static std::vector<std::string> LoadMaterials(const aiScene* _scene);
	static std::vector<Mesh> LoadNode(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, aiNode* _node, const aiScene* _scene, std::vector<int> _materialToTexture);
	static Mesh LoadMesh(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, aiMesh* _mesh, const aiScene* _scene, std::vector<int> _materialToTexture);

};

