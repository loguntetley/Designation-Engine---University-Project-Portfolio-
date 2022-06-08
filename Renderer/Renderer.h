#pragma once
#define GLFW_INCLUDE_VULKAN
#include "PCH.h"

#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Utilities.h"
#include "Mesh.h"
#include "MeshModel.h"

class Renderer
{
private:
	struct 
	{
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
	} m_MainDevice;

	int m_CurrentFrame = 0;
	GLFWwindow* m_Window;
	VkInstance m_Instance;
	VkQueue m_GraphicsQueue, m_PresentationQueue;
	VkSurfaceKHR m_Surface;
	VkSwapchainKHR m_SwapChain;
	VkPipeline m_GraphicsPipeline;
	VkPipelineLayout m_PipelineLayout;
	VkRenderPass m_RenderPass;
	VkCommandPool m_GraphicsCommandPool;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	VkImage m_DepthBufferImage;
	VkImageView m_DepthBufferImageView;
	VkDeviceMemory m_DepthBufferImageMemory;
	VkDescriptorSetLayout m_DescriptorSetLayout, m_SamplerSetLayout;
	VkDescriptorPool m_DescriptorPool, m_SamplerDescriptorPool;
	VkPushConstantRange m_PushConstantRange;
	VkSampler m_TextureSampler;
	std::vector<VkDescriptorSet> m_DescriptorSets, m_SamplerDescriptorSets;
	std::vector<MeshModel> m_ModelList;
	std::vector<SwapChainImage> m_SwapChainImages;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	std::vector<VkCommandBuffer> m_CommandBuffers;
	std::vector<VkBuffer> m_ViewProjectionUniformBuffer;
	std::vector<VkDeviceMemory> m_ViewProjectionUniformBufferMemory, m_TextureImageMemory;
	std::vector<VkImage> m_TextureImages;
	std::vector<VkImageView> m_TextureImageViews;
	std::vector<VkSemaphore> m_ImageAvailable, m_RenderFinished;
	std::vector<VkFence> m_DrawFences;

	//CREATE FUNCTIONS//
	void CreateInstance();
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateSwapChain();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateDescriptorSetLayout();
	void CreatePushConstantRange();
	void CreateDepthBufferImage();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSynchronisation();
	void CreateTextureSampler();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	VkImage CreateImage(uint32_t _width, uint32_t _height, VkFormat _format, VkImageTiling _tiling, VkImageUsageFlags _useFlags, VkMemoryPropertyFlags _propFlags, VkDeviceMemory* _imageMemory);
	VkImageView CreateImageView(VkImage _image, VkFormat _format, VkImageAspectFlags _aspectFlags);
	VkShaderModule CreateShaderModule(const std::vector<char>& _code);
	int CreateTextureImage(std::string _fileName);
	int CreateTexture(std::string _fileName);
	int CreateTextureDescriptor(VkImageView _textureImage);

	//GETTER FUNCTIONS//
	void GetPhysicalDevice();
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice _physicalDevice);
	SwapChainDetails GetSwapChainDetails(VkPhysicalDevice _physicalDevice);
	
	//CHECK FUNCTIONS//
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice _physicalDevice);
	bool CheckDeviceSuitable(VkPhysicalDevice _physicalDevice);

	//SELECT FUNCTIONS//
	VkSurfaceFormatKHR SelectBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _formats);
	VkPresentModeKHR SelectBestPresentationMode(const std::vector<VkPresentModeKHR> _presentationModes);
	VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities);
	VkFormat SelectSupportedFormat(const std::vector<VkFormat>& _formats, VkImageTiling _tiling, VkFormatFeatureFlags _featureFlags);

	//RECORD FUNCTIONS//
	void RecordCommands(uint32_t _currentImage);

	//UPDATE FUNCTIONS//
	void UpdateUniformBuffers(uint32_t _imageIndex);

	//LOAD FUNCTIONS//
	stbi_uc* LoadTextureFile(std::string _fileName, int* _width, int* _height, VkDeviceSize* _imageSize);

public:
	struct UniformBufferObjectViewProjection
	{
		glm::mat4 m_Projection, m_View;
	} m_UniformBufferObjectViewProjection;

	//MANAGEMENT FUNCTIONS//
	int Initialise(GLFWwindow* _window);
	void Cleanup();
	void Draw();

	//USAGE FUNCTIONS//
	void UpdateModel(int _modelId, glm::mat4 _model);
	int CreateMeshModel(std::string _modelFile);
	
};

