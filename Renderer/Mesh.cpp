#include "Mesh.h"

#pragma region Constructors

Mesh::Mesh()
{
}

Mesh::Mesh(VkPhysicalDevice _physicalDevice, VkDevice _logicalevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, std::vector<Vertex>* _vertices, std::vector<uint32_t>* _indices, int _textureId)
	: m_VertexCount(_vertices->size()), m_IndexCount(_indices->size()), m_PhysicalDevice(_physicalDevice), m_LogicalDevice(_logicalevice), m_TextureId(_textureId)
{
	DN_CORE_INFO_OUTPUT("Constructing mesh");
	CreateVertexBuffer(_transferQueue, _transferCommandPool, _vertices);
	CreateIndexBuffer(_transferQueue, _transferCommandPool, _indices);
	m_Model.m_Model = glm::mat4(1.0f);
}

Mesh::~Mesh()
{
}

#pragma endregion

#pragma region Create Functions

void Mesh::CreateVertexBuffer(VkQueue _transferQueue, VkCommandPool _transferCommandPool, std::vector<Vertex>* _vertices)
{
	DN_CORE_INFO_OUTPUT("Creating vertex buffer");
	//STAGING BUFFER//
	VkDeviceSize bufferSize = sizeof(Vertex) * _vertices->size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

	//MAP MEMORY//
	DN_CORE_INFO_OUTPUT("Mapping memory");
	void* data;																
	vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);			
	memcpy(data, _vertices->data(), (size_t)bufferSize);							
	vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);
	DN_CORE_SUCCESS_OUTPUT("Memory mapped");

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_VertexBuffer, &m_VertexBufferMemory);
	DN_CORE_SUCCESS_OUTPUT("Vertex buffer created");
	CopyBuffer(m_LogicalDevice, _transferQueue, _transferCommandPool, stagingBuffer, m_VertexBuffer, bufferSize);

	vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
}

void Mesh::CreateIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices)
{
	DN_CORE_INFO_OUTPUT("Creating index buffer");
	//STAGING BUFFER//
	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

	//MAP MEMORY//
	DN_CORE_INFO_OUTPUT("Mapping memory");
	void* data;
	vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices->data(), (size_t)bufferSize);
	vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);
	DN_CORE_SUCCESS_OUTPUT("Memory mapped");

	CreateBuffer(m_PhysicalDevice, m_LogicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_IndexBuffer, &m_IndexBufferMemory);
	DN_CORE_SUCCESS_OUTPUT("Vertex index created");
	CopyBuffer(m_LogicalDevice, transferQueue, transferCommandPool, stagingBuffer, m_IndexBuffer, bufferSize);

	vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
}

#pragma endregion

#pragma region Setter Functions

void Mesh::SetModel(glm::mat4 _model)
{
	m_Model.m_Model = _model;
}

#pragma endregion

#pragma region Getter Functions

int Mesh::GetTextureId() const
{
	return m_TextureId;
}

int Mesh::GetVertexCount() const
{
	return m_VertexCount;
}

int Mesh::GetIndexCount() const
{
	return m_IndexCount;
}

Model Mesh::GetModel() const
{
	return m_Model;
}

VkBuffer Mesh::GetVertexBuffer() const
{
	return m_VertexBuffer;
}

VkBuffer Mesh::GetIndexBuffer() const
{
	return m_IndexBuffer;
}

#pragma endregion

#pragma region Managment Functions

void Mesh::DestroyBuffers()
{
	DN_CORE_INFO_OUTPUT("Cleaning up mesh buffers");
	vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_VertexBufferMemory, nullptr);
	vkDestroyBuffer(m_LogicalDevice, m_IndexBuffer, nullptr);
	vkFreeMemory(m_LogicalDevice, m_IndexBufferMemory, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Mesh buffers cleaned up");
}

#pragma endregion

