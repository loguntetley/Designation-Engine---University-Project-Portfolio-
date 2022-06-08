#pragma once
#define GLFW_INCLUDE_VULKAN
#include "PCH.h"

#include <GLFW/glfw3.h>

#include "Utilities.h"

struct Model 
{
	glm::mat4 m_Model;
};

class Mesh
{
private:
	int m_TextureId, m_VertexCount, m_IndexCount;
	Model m_Model;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_LogicalDevice;
	VkBuffer m_VertexBuffer, m_IndexBuffer;
	VkDeviceMemory m_VertexBufferMemory, m_IndexBufferMemory;

	//CREATE FUNCTIONS//
	void CreateVertexBuffer(VkQueue _transferQueue, VkCommandPool _transferCommandPool, std::vector<Vertex>* _vertices);
	void CreateIndexBuffer(VkQueue _transferQueue, VkCommandPool _transferCommandPool, std::vector<uint32_t>* _indices);

public:
	//CONSTRUCTORS//
	Mesh();
	Mesh(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, std::vector<Vertex>* _vertices, std::vector<uint32_t>* _indices, int _textureId);
	~Mesh();

	//GETTERS FUNCTIONS//
	int GetTextureId() const;
	int GetVertexCount() const;
	int GetIndexCount() const;
	Model GetModel() const;
	VkBuffer GetVertexBuffer() const;
	VkBuffer GetIndexBuffer() const;

	//SETTERS FUNCTIONS//
	void SetModel(glm::mat4 _model);

	//MANAGMENT FUNCTIONS//
	void DestroyBuffers();
	
};

