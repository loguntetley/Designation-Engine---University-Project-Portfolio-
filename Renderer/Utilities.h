#pragma once
#include "PCH.h"

const int MAX_OBJECTS = 20;
const int MAX_FRAME_DRAWS = 2;

const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

struct Vertex
{
	glm::vec3 m_Position;		//X, Y, Z
	glm::vec3 m_Colour;			//R, G, B
	glm::vec2 m_Texture;		//U, V
};

struct QueueFamilyIndices 
{
	int m_GraphicsFamily = -1, m_PresentationFamily = -1;									//Location of queue families
	bool IsValid() const { return m_GraphicsFamily >= 0 && m_PresentationFamily >= 0; }		//Check families are valid
};

struct SwapChainDetails 
{
	VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;		
	std::vector<VkSurfaceFormatKHR> m_Formats;			
	std::vector<VkPresentModeKHR> m_PresentationModes;	
};

struct SwapChainImage 
{
	VkImage m_Image;
	VkImageView m_ImageView;
};

static std::vector<char> ReadFile(const std::string& _filename)
{
	std::ifstream file(_filename, std::ios::binary | std::ios::ate);

	if (!file.is_open())												//Check if opened
	{
		DN_CORE_ERROR_OUTPUT("Failed to open a file");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("File opened");

	DN_CORE_INFO_OUTPUT("Reading file");
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);

	file.seekg(0);														//Move read positionto file start
	file.read(fileBuffer.data(), fileSize);								//Read the file data to buffer
	file.close();

	DN_CORE_SUCCESS_OUTPUT("File read");
	return fileBuffer;
}

static uint32_t FindMemoryTypeIndex(VkPhysicalDevice _physicalDevice, uint32_t _allowedTypes, VkMemoryPropertyFlags _properties)
{
	DN_CORE_INFO_OUTPUT("Finding memory type index");
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((_allowedTypes & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & _properties) == _properties)
		{
			DN_CORE_SUCCESS_OUTPUT("Memory type index found");
			return i;
		}	
	}
}

static void CreateBuffer(VkPhysicalDevice _physicalDevice, VkDevice _logicalDevice, VkDeviceSize _bufferSize, VkBufferUsageFlags _bufferUsage, VkMemoryPropertyFlags _bufferProperties, VkBuffer* _buffer, VkDeviceMemory* _bufferMemory)
{
	//VERTEX BUFFER//
	DN_CORE_INFO_OUTPUT("Creating vertex buffer");
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = _bufferSize;								
	bufferInfo.usage = _bufferUsage;								
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;			

	VkResult result = vkCreateBuffer(_logicalDevice, &bufferInfo, nullptr, _buffer);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create vertex buffer");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Vertex buffer created");

	//BUFFER MEMORY REQUIREMENTS//
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(_logicalDevice, *_buffer, &memoryRequirements);

	//ALLOCATE MEMORY BUFFER//
	DN_CORE_INFO_OUTPUT("Allocating vertex buffer memory");
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryTypeIndex(_physicalDevice, memoryRequirements.memoryTypeBits, _bufferProperties);
																																																			
	result = vkAllocateMemory(_logicalDevice, &memoryAllocateInfo, nullptr, _bufferMemory);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to allocate vertex buffer memory");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Vertex buffer memory allocated");
		
	vkBindBufferMemory(_logicalDevice, (*_buffer), (*_bufferMemory), 0);
}

static VkCommandBuffer BeginCommandBuffer(VkDevice _logicalDevice, VkCommandPool _commandPool)
{
	DN_CORE_INFO_OUTPUT("Begining command buffer");
	VkCommandBuffer commandBuffer;

	DN_CORE_INFO_OUTPUT("Allocating command buffer");
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(_logicalDevice, &allocInfo, &commandBuffer);
	DN_CORE_SUCCESS_OUTPUT("Command buffer allocated");

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

static void EndAndSubmitCommandBuffer(VkDevice _logicalDevice, VkCommandPool _commandPool, VkQueue _queue, VkCommandBuffer _commandBuffer)
{
	// End commands
	DN_CORE_INFO_OUTPUT("Submitting command buffer");
	vkEndCommandBuffer(_commandBuffer);

	// Queue submission information
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffer;

	// Submit transfer command to transfer queue and wait until it finishes
	vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_queue);
	DN_CORE_SUCCESS_OUTPUT("Command buffer submitted");
	// Free temporary command buffer back to pool
	vkFreeCommandBuffers(_logicalDevice, _commandPool, 1, &_commandBuffer);
}

static void CopyBuffer(VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, VkBuffer _srcBuffer, VkBuffer _dstBuffer, VkDeviceSize _bufferSize)
{
	DN_CORE_INFO_OUTPUT("Copying buffer");
	// Create buffer
	VkCommandBuffer transferCommandBuffer = BeginCommandBuffer(_logicalDevice, _transferCommandPool);

	// Region of data to copy from and to
	VkBufferCopy bufferCopyRegion = {};
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.size = _bufferSize;

	// Command to copy src buffer to dst buffer
	vkCmdCopyBuffer(transferCommandBuffer, _srcBuffer, _dstBuffer, 1, &bufferCopyRegion);
	DN_CORE_SUCCESS_OUTPUT("Buffer copied");

	EndAndSubmitCommandBuffer(_logicalDevice, _transferCommandPool, _transferQueue, transferCommandBuffer);
}

static void CopyImageBuffer(VkDevice _logicalDevice, VkQueue _transferQueue, VkCommandPool _transferCommandPool, VkBuffer _srcBuffer, VkImage _image, uint32_t _width, uint32_t _height)
{
	DN_CORE_INFO_OUTPUT("Copying image buffer");
	VkCommandBuffer transferCommandBuffer = BeginCommandBuffer(_logicalDevice, _transferCommandPool);

	VkBufferImageCopy imageRegion = {};
	imageRegion.bufferOffset = 0;											
	imageRegion.bufferRowLength = 0;										
	imageRegion.bufferImageHeight = 0;										
	imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;	
	imageRegion.imageSubresource.mipLevel = 0;								
	imageRegion.imageSubresource.baseArrayLayer = 0;						
	imageRegion.imageSubresource.layerCount = 1;							
	imageRegion.imageOffset = { 0, 0, 0 };									
	imageRegion.imageExtent = { _width, _height, 1 };							

	vkCmdCopyBufferToImage(transferCommandBuffer, _srcBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);
	DN_CORE_SUCCESS_OUTPUT("Image buffer copied");

	EndAndSubmitCommandBuffer(_logicalDevice, _transferCommandPool, _transferQueue, transferCommandBuffer);
}

static void TransitionImageLayout(VkDevice _logicalDevice, VkQueue _queue, VkCommandPool _commandPool, VkImage _image, VkImageLayout _oldLayout, VkImageLayout _newLayout)
{
	VkCommandBuffer commandBuffer = BeginCommandBuffer(_logicalDevice, _commandPool);

	DN_CORE_INFO_OUTPUT("Transitioning image layout");
	VkImageMemoryBarrier imageMemoryBarrier = {};
	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemoryBarrier.oldLayout = _oldLayout;									
	imageMemoryBarrier.newLayout = _newLayout;									
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;			
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;			
	imageMemoryBarrier.image = _image;											
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;	
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;						
	imageMemoryBarrier.subresourceRange.levelCount = 1;							
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;						
	imageMemoryBarrier.subresourceRange.layerCount = 1;				

	VkPipelineStageFlags srcStage;
	VkPipelineStageFlags dstStage;

	if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		imageMemoryBarrier.srcAccessMask = 0;								
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;		
		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	DN_CORE_SUCCESS_OUTPUT("Image layout transitioned");

	vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	EndAndSubmitCommandBuffer(_logicalDevice, _commandPool, _queue, commandBuffer);
}