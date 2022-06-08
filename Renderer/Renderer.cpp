#include "Renderer.h"

#pragma region Management Functions

int Renderer::Initialise(GLFWwindow* _window)
{
	DN_CORE_INFO_OUTPUT("Renderer initialisation started");
	m_Window = _window;

	try
	{
		DN_CORE_INFO("Renderer creation started");
		CreateInstance();
		CreateSurface();
		GetPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain();
		CreateRenderPass();
		CreateDescriptorSetLayout();
		CreatePushConstantRange();
		CreateGraphicsPipeline();
		CreateDepthBufferImage();
		CreateFramebuffers();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateTextureSampler();
		CreateUniformBuffers();
		CreateDescriptorPool();
		CreateDescriptorSets();
		CreateSynchronisation();
		DN_CORE_INFO_OUTPUT("Renderer creation finished");

		DN_CORE_INFO_OUTPUT("Projection setup");
		m_UniformBufferObjectViewProjection.m_Projection = glm::perspective(glm::radians(45.0f), (float)m_SwapChainExtent.width / (float)m_SwapChainExtent.height, 0.1f, 100.0f);
		m_UniformBufferObjectViewProjection.m_View = glm::lookAt(glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_UniformBufferObjectViewProjection.m_Projection[1][1] *= -1;
		DN_CORE_INFO_OUTPUT("Projection finished");

		CreateTexture("plain.png");
	}
	catch (const std::runtime_error& _error)
	{
		DN_CORE_ERROR_OUTPUT(_error.what());
		//printf("ERROR: %s\n", _error.what());
		return EXIT_FAILURE;
	}

	return 0;
}

void Renderer::Cleanup()
{
	DN_CORE_INFO_OUTPUT("Renderer clean up");

	DN_CORE_INFO_OUTPUT("Device waiting");
	vkDeviceWaitIdle(m_MainDevice.m_LogicalDevice);
	DN_CORE_SUCCESS_OUTPUT("Device finished waiting");


	DN_CORE_INFO_OUTPUT("Destroying all mesh model");
	for (auto _model : m_ModelList)
	{
		_model.DestroyMeshModel();
		DN_CORE_SUCCESS_OUTPUT("Mesh model destroyed");
	}
	DN_CORE_SUCCESS_OUTPUT("All mesh models destroyed");

	DN_CORE_INFO_OUTPUT("Destroying descriptor pool");
	vkDestroyDescriptorPool(m_MainDevice.m_LogicalDevice, m_SamplerDescriptorPool, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Descriptor pool destroyed");

	DN_CORE_INFO_OUTPUT("Destroying descriptor set layout");
	vkDestroyDescriptorSetLayout(m_MainDevice.m_LogicalDevice, m_SamplerSetLayout, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Descriptor set layout destroyed");

	DN_CORE_INFO_OUTPUT("Destroying sampler started");
	vkDestroySampler(m_MainDevice.m_LogicalDevice, m_TextureSampler, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Sampler destroyed");

	DN_CORE_INFO_OUTPUT("Destroying all texture images");
	for (size_t i = 0; i < m_TextureImages.size(); i++)
	{
		vkDestroyImageView(m_MainDevice.m_LogicalDevice, m_TextureImageViews[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Image view destroyed");
		vkDestroyImage(m_MainDevice.m_LogicalDevice, m_TextureImages[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Image destroyed");
		vkFreeMemory(m_MainDevice.m_LogicalDevice, m_TextureImageMemory[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Texture images memory freed");
	}
	DN_CORE_SUCCESS_OUTPUT("All texture images destroyed");

	DN_CORE_INFO_OUTPUT("Destroying image view started");
	vkDestroyImageView(m_MainDevice.m_LogicalDevice, m_DepthBufferImageView, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Image view destroyed");

	DN_CORE_INFO_OUTPUT("Destroying image");
	vkDestroyImage(m_MainDevice.m_LogicalDevice, m_DepthBufferImage, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Image destroyed");

	DN_CORE_INFO_OUTPUT("Freeing depth buffer image memory");
	vkFreeMemory(m_MainDevice.m_LogicalDevice, m_DepthBufferImageMemory, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Depth buffer memory freed");

	DN_CORE_INFO_OUTPUT("Destroying descriptor pool");
	vkDestroyDescriptorPool(m_MainDevice.m_LogicalDevice, m_DescriptorPool, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Descriptor pool destroyed");

	DN_CORE_INFO_OUTPUT("Destroying descriptor set layout");
	vkDestroyDescriptorSetLayout(m_MainDevice.m_LogicalDevice, m_DescriptorSetLayout, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Descriptor set layout destroyed");

	DN_CORE_INFO_OUTPUT("Destroying all view projection data");
	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		vkDestroyBuffer(m_MainDevice.m_LogicalDevice, m_ViewProjectionUniformBuffer[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("View projection uniform buffer destroyed");
		vkFreeMemory(m_MainDevice.m_LogicalDevice, m_ViewProjectionUniformBufferMemory[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("View projection uniform buffer memory freed");
	}
	DN_CORE_SUCCESS_OUTPUT("All projection data destroyed");

	DN_CORE_INFO_OUTPUT("Destroying all semaphores");
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		vkDestroySemaphore(m_MainDevice.m_LogicalDevice, m_RenderFinished[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Renderer finished semaphore destroyed");
		vkDestroySemaphore(m_MainDevice.m_LogicalDevice, m_ImageAvailable[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Image available semaphores destroyed");
		vkDestroyFence(m_MainDevice.m_LogicalDevice, m_DrawFences[i], nullptr);
		DN_CORE_SUCCESS_OUTPUT("Draw fence semaphores destroyed");
	}
	DN_CORE_SUCCESS_OUTPUT("All semaphores destroyed");

	DN_CORE_INFO("Destroying graphics command pool");
	vkDestroyCommandPool(m_MainDevice.m_LogicalDevice, m_GraphicsCommandPool, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Graphics command pool destroyed");

	DN_CORE_INFO_OUTPUT("Destroying frame buffers");
	for (auto _framebuffer : m_SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(m_MainDevice.m_LogicalDevice, _framebuffer, nullptr);
		DN_CORE_SUCCESS_OUTPUT("Frame buffer destroyed");
	}
	DN_CORE_SUCCESS_OUTPUT("All frame buffers destroyed");
		
	DN_CORE_INFO_OUTPUT("Destroying Graphics pipeline");
	vkDestroyPipeline(m_MainDevice.m_LogicalDevice, m_GraphicsPipeline, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Graphics pipeline destroyed");

	DN_CORE_INFO_OUTPUT("Destroying pipeline layout");
	vkDestroyPipelineLayout(m_MainDevice.m_LogicalDevice, m_PipelineLayout, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Pipeline layout destroyed");

	DN_CORE_INFO_OUTPUT("Destroying render pass");
	vkDestroyRenderPass(m_MainDevice.m_LogicalDevice, m_RenderPass, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Render pass destroyed");
	
	DN_CORE_INFO_OUTPUT("Destroying all swap chain images");
	for (auto _image : m_SwapChainImages)
	{
		vkDestroyImageView(m_MainDevice.m_LogicalDevice, _image.m_ImageView, nullptr);
		DN_CORE_SUCCESS_OUTPUT("Swap chain image destroyed");
	}
	DN_CORE_SUCCESS_OUTPUT("All swap chain images destroyed");

	DN_CORE_INFO_OUTPUT("Destroying swap chain");
	vkDestroySwapchainKHR(m_MainDevice.m_LogicalDevice, m_SwapChain, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Swap chain destroyed");

	DN_CORE_INFO("Destroying surface");
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Surface destroyed");

	DN_CORE_INFO("Destroying logical device");
	vkDestroyDevice(m_MainDevice.m_LogicalDevice, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Logical device destroyed");

	DN_CORE_INFO_OUTPUT("Destroying instance");
	vkDestroyInstance(m_Instance, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Vulkan instance destroyed");
}

void Renderer::Draw()
{
	vkWaitForFences(m_MainDevice.m_LogicalDevice, 1, &m_DrawFences[m_CurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(m_MainDevice.m_LogicalDevice, 1, &m_DrawFences[m_CurrentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(m_MainDevice.m_LogicalDevice, m_SwapChain, std::numeric_limits<uint64_t>::max(), m_ImageAvailable[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);
	RecordCommands(imageIndex);
	UpdateUniformBuffers(imageIndex);

	//COMMAND BUFFER//
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;										
	submitInfo.pWaitSemaphores = &m_ImageAvailable[m_CurrentFrame];		

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.pWaitDstStageMask = waitStages;						
	submitInfo.commandBufferCount = 1;								
	submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];		
	submitInfo.signalSemaphoreCount = 1;							
	submitInfo.pSignalSemaphores = &m_RenderFinished[m_CurrentFrame];	

	VkResult result = vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_DrawFences[m_CurrentFrame]);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to submit command buffer to queue");
		throw std::runtime_error("");
	}
		
	//RENDERED IMAGE//
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;										
	presentInfo.pWaitSemaphores = &m_RenderFinished[m_CurrentFrame];			
	presentInfo.swapchainCount = 1;											
	presentInfo.pSwapchains = &m_SwapChain;									
	presentInfo.pImageIndices = &imageIndex;								

	result = vkQueuePresentKHR(m_PresentationQueue, &presentInfo);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to present image");
		throw std::runtime_error("");
	}

	m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAME_DRAWS;
}

#pragma endregion

#pragma region Create Functions

void Renderer::CreateInstance()
{
	DN_CORE_INFO_OUTPUT("Start creation of instance");
	//APPLICATION INFORMATION//
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Designation";					
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);		
	applicationInfo.pEngineName = "Designation Engine";							
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);			
	applicationInfo.apiVersion = VK_API_VERSION_1_0;					

	//INSTACE INFO//
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &applicationInfo;

	//INSTANCE EXTENSION//
	DN_CORE_INFO_OUTPUT("Create instance extensions");
	std::vector<const char*> instanceExtensions = std::vector<const char*>();
	uint32_t glfwExtensionCount = 0;											//GLFW required extensions
	const char** glfwExtensions;					
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
		DN_CORE_SUCCESS_OUTPUT("Extenstion: " + (std::string)glfwExtensions[i] + " added");
	}
	DN_CORE_SUCCESS_OUTPUT("All extensions added");
		
	if (!CheckInstanceExtensionSupport(&instanceExtensions))
	{
		DN_CORE_ERROR_OUTPUT("Instance not supported extensions");
		throw std::runtime_error("");
	}
	
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = nullptr;
	DN_CORE_SUCCESS_OUTPUT("Extensions created");

	VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_Instance);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a vulkan instance");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Instance created");
}

void Renderer::CreateLogicalDevice()
{
	DN_CORE_INFO_OUTPUT("Creating logical device");
	//QUEUE INFORMATION//
	QueueFamilyIndices indices = GetQueueFamilies(m_MainDevice.m_PhysicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	std::set<int> queueFamilyIndices = { indices.m_GraphicsFamily, indices.m_PresentationFamily };

	for (int queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;						
		queueCreateInfo.queueCount = 1;												
		float priority = 1.0f;
		queueCreateInfo.pQueuePriorities = &priority;								
		queueInfos.push_back(queueCreateInfo);
	}

	//LOGICAL DEVICE//
	VkDeviceCreateInfo logicalDeviceInfo = {};
	logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());		
	logicalDeviceInfo.pQueueCreateInfos = queueInfos.data();								
	logicalDeviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());	
	logicalDeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();							

	//PHYSICAL DEVICE FEATURES//
	DN_CORE_INFO_OUTPUT("Physical device features enabling");
	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;		
	DN_CORE_SUCCESS_OUTPUT("Sampler anisotropy enabled");
	logicalDeviceInfo.pEnabledFeatures = &physicalDeviceFeatures;	

	VkResult result = vkCreateDevice(m_MainDevice.m_PhysicalDevice, &logicalDeviceInfo, nullptr, &m_MainDevice.m_LogicalDevice);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a logical device");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Logical device created");

	vkGetDeviceQueue(m_MainDevice.m_LogicalDevice, indices.m_GraphicsFamily, 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_MainDevice.m_LogicalDevice, indices.m_PresentationFamily, 0, &m_PresentationQueue);
}

void Renderer::CreateSurface()
{
	//SURFACE//
	DN_CORE_INFO_OUTPUT("Creating surface");
	VkResult result = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a surface");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Surface created");
}

void Renderer::CreateSwapChain()
{
	//SWAPCHAIN DETAILS//
	DN_CORE_INFO_OUTPUT("Setting up swap chain details");
	SwapChainDetails swapChainDetails = GetSwapChainDetails(m_MainDevice.m_PhysicalDevice);
	VkSurfaceFormatKHR surfaceFormat = SelectBestSurfaceFormat(swapChainDetails.m_Formats);
	VkPresentModeKHR presentMode = SelectBestPresentationMode(swapChainDetails.m_PresentationModes);
	VkExtent2D extent = SelectSwapExtent(swapChainDetails.m_SurfaceCapabilities);
	uint32_t imageCount = swapChainDetails.m_SurfaceCapabilities.minImageCount + 1;
	DN_CORE_SUCCESS_OUTPUT("Swap chain details setup");

	if (swapChainDetails.m_SurfaceCapabilities.maxImageCount > 0 && swapChainDetails.m_SurfaceCapabilities.maxImageCount < imageCount)
		imageCount = swapChainDetails.m_SurfaceCapabilities.maxImageCount;

	//SWAPCHAIN//
	DN_CORE_INFO_OUTPUT("Creating swap chain");
	VkSwapchainCreateInfoKHR swapChainInfo = {};
	swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainInfo.surface = m_Surface;														
	swapChainInfo.imageFormat = surfaceFormat.format;										
	swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;								
	swapChainInfo.presentMode = presentMode;												
	swapChainInfo.imageExtent = extent;													
	swapChainInfo.minImageCount = imageCount;												
	swapChainInfo.imageArrayLayers = 1;													
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;						
	swapChainInfo.preTransform = swapChainDetails.m_SurfaceCapabilities.currentTransform;	
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;						
	swapChainInfo.clipped = VK_TRUE;	

	//QUEUE FAMILIES//
	QueueFamilyIndices indices = GetQueueFamilies(m_MainDevice.m_PhysicalDevice);

	if (indices.m_GraphicsFamily != indices.m_PresentationFamily)
	{
		uint32_t queueFamilyIndices[] =
		{
			(uint32_t)indices.m_GraphicsFamily,
			(uint32_t)indices.m_PresentationFamily
		};

		swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainInfo.queueFamilyIndexCount = 2;
		swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.queueFamilyIndexCount = 0;
		swapChainInfo.pQueueFamilyIndices = nullptr;
	}

	swapChainInfo.oldSwapchain = VK_NULL_HANDLE; //Saftey if changed swapchain

	VkResult result = vkCreateSwapchainKHR(m_MainDevice.m_LogicalDevice, &swapChainInfo, nullptr, &m_SwapChain);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a swapchain");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Swap chain created");

	m_SwapChainImageFormat = surfaceFormat.format;
	m_SwapChainExtent = extent;

	//SWAPCHAIN IMAGES//
	DN_CORE_INFO_OUTPUT("Setting up swap chain images");
	uint32_t swapChainImageCount;
	vkGetSwapchainImagesKHR(m_MainDevice.m_LogicalDevice, m_SwapChain, &swapChainImageCount, nullptr);
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(m_MainDevice.m_LogicalDevice, m_SwapChain, &swapChainImageCount, images.data());

	for (VkImage _image : images)
	{
		SwapChainImage swapChainImage = {};
		swapChainImage.m_Image = _image;
		swapChainImage.m_ImageView = CreateImageView(_image, m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		m_SwapChainImages.push_back(swapChainImage);
		DN_CORE_SUCCESS_OUTPUT("Swap chain image setup");
	}
}

void Renderer::CreateRenderPass()
{
	//COLOUR ATTACHMENT//
	DN_CORE_INFO_OUTPUT("Setting up colour attachment");
	VkAttachmentDescription colourAttachment = {};
	colourAttachment.format = m_SwapChainImageFormat;						
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;			
	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	
	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;	 
	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;			
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;	
	DN_CORE_SUCCESS_OUTPUT("Colour attachement setup");

	//DEPTH ATTACHMENT//
	DN_CORE_INFO_OUTPUT("Setting up depth attachment");
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = SelectSupportedFormat({ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	DN_CORE_SUCCESS_OUTPUT("Depth attachement setup");

	//COLOUR REFRENCE//
	DN_CORE_INFO_OUTPUT("Setting up colour refrence");
	VkAttachmentReference colourAttachmentReference = {};
	colourAttachmentReference.attachment = 0;
	colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	DN_CORE_SUCCESS_OUTPUT("Colour refrence setup");

	//DEPTH REFRENCE//
	DN_CORE_INFO_OUTPUT("Setting up depth refrence");
	VkAttachmentReference depthAttachmentReference = {};
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	DN_CORE_SUCCESS_OUTPUT("Depth attachement setup");

	//GRAPHICS SUBPASS//
	DN_CORE_INFO_OUTPUT("Setting up graphics subpass");
	VkSubpassDescription graphicsSubpass = {};
	graphicsSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;	
	graphicsSubpass.colorAttachmentCount = 1;
	graphicsSubpass.pColorAttachments = &colourAttachmentReference;
	graphicsSubpass.pDepthStencilAttachment = &depthAttachmentReference;
	DN_CORE_SUCCESS_OUTPUT("Graphics subpass setup");

	//SUBPASS DEPENDANCIES//

	std::array<VkSubpassDependency, 2> subpassDependencies;
	DN_CORE_INFO_OUTPUT("Setting up sub dependencies");
	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;						
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;		
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;				
	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[0].dependencyFlags = 0;

	subpassDependencies[1].srcSubpass = 0;
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;;
	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[1].dependencyFlags = 0;
	DN_CORE_SUCCESS_OUTPUT("Sub dependecies setup");

	std::array<VkAttachmentDescription, 2> renderPassAttachments = { colourAttachment, depthAttachment };

	DN_CORE_INFO_OUTPUT("Create render pass");
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(renderPassAttachments.size());
	renderPassInfo.pAttachments = renderPassAttachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &graphicsSubpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
	renderPassInfo.pDependencies = subpassDependencies.data();

	VkResult result = vkCreateRenderPass(m_MainDevice.m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a render pass");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Render pass created");		
}

void Renderer::CreateGraphicsPipeline()
{
	//SPIR-V SHADERS//
	DN_CORE_INFO_OUTPUT("Reading SPIR-V shaders from file");
	auto vertexShaderCode = ReadFile("VulkanPrototype/Shaders/vert.spv");
	auto fragmentShaderCode = ReadFile("VulkanPrototype/Shaders/frag.spv");
	DN_CORE_SUCCESS_OUTPUT("Fragment and vertex files read");

	DN_CORE_INFO_OUTPUT("Creating shader modules");
	VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);
	DN_CORE_SUCCESS_OUTPUT("Fragment and vertex shader modules created");

	//SHADER STAGE//
	DN_CORE_INFO_OUTPUT("Setting up vertex shader stage");
	VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
	vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;				
	vertexShaderStageInfo.module = vertexShaderModule;						
	vertexShaderStageInfo.pName = "main";		
	DN_CORE_SUCCESS_OUTPUT("Vertex shader stage setup");

	DN_CORE_INFO_OUTPUT("Setting up fragment shader stage");
	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
	fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;				
	fragmentShaderStageInfo.module = fragmentShaderModule;						
	fragmentShaderStageInfo.pName = "main";									
	DN_CORE_SUCCESS_OUTPUT("Fragment shader stage setup");

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

	DN_CORE_INFO_OUTPUT("Setting up vertex binding");
	VkVertexInputBindingDescription vertexInputBindingDescription = {};
	vertexInputBindingDescription.binding = 0;									
	vertexInputBindingDescription.stride = sizeof(Vertex);						
	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;		
	DN_CORE_SUCCESS_OUTPUT("Vertex binding setup");

	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

	DN_CORE_INFO_OUTPUT("Setting up attribute binding");
	// POSITION ATTRIBUTE//
	DN_CORE_INFO_OUTPUT("Setting up position attribute binding");
	attributeDescriptions[0].binding = 0;							
	attributeDescriptions[0].location = 0;							
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;	
	attributeDescriptions[0].offset = offsetof(Vertex, m_Position);	
	DN_CORE_SUCCESS_OUTPUT("Position attribute binding setup");

	//COLOUR ATTRIBUTE//
	DN_CORE_INFO_OUTPUT("Setting up colour attribute binding");
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, m_Colour);
	DN_CORE_SUCCESS_OUTPUT("Colour attribute binding setup");

	//TEXTURE ATTRIBUTE//
	DN_CORE_INFO_OUTPUT("Setting up texture attribute binding");
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, m_Texture);
	DN_CORE_SUCCESS_OUTPUT("Texture attribute binding setup");

	//VERTEX INPUT STATE//
	DN_CORE_INFO_OUTPUT("Creating vertex shader state");
	VkPipelineVertexInputStateCreateInfo vertexInputStageInfo = {};
	vertexInputStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStageInfo.vertexBindingDescriptionCount = 1;
	vertexInputStageInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;											
	vertexInputStageInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputStageInfo.pVertexAttributeDescriptions = attributeDescriptions.data();		
	DN_CORE_SUCCESS_OUTPUT("Vertex shader state created");

	//INPUT ASSEMBLY STATE//
	DN_CORE_INFO_OUTPUT("Creating input assembly state");
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
	inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;		
	inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;	
	DN_CORE_SUCCESS_OUTPUT("Input assembly state created");

	//VIEWPORT STAGE//
	DN_CORE_INFO_OUTPUT("Setting up viewport");
	VkViewport viewport = {};
	viewport.x = 0.0f;									
	viewport.y = 0.0f;									
	viewport.width = (float)m_SwapChainExtent.width;		
	viewport.height = (float)m_SwapChainExtent.height;	
	viewport.minDepth = 0.0f;							
	viewport.maxDepth = 1.0f;		
	DN_CORE_SUCCESS_OUTPUT("Viewport data setup");

	DN_CORE_INFO_OUTPUT("Applying scissoring");
	VkRect2D scissor = {};
	scissor.offset = { 0,0 };							
	scissor.extent = m_SwapChainExtent;
	DN_CORE_SUCCESS_OUTPUT("Scissoring applied");

	DN_CORE_INFO_OUTPUT("Creating viewport state");
	VkPipelineViewportStateCreateInfo viewportStateInfo = {};
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;
	DN_CORE_SUCCESS_OUTPUT("Viewport state created");

	//RASTERIZER STATE//
	DN_CORE_INFO_OUTPUT("Creating rasterizer state");
	VkPipelineRasterizationStateCreateInfo rasterizerStateInfo = {};
	rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerStateInfo.depthClampEnable = VK_FALSE;					
	rasterizerStateInfo.rasterizerDiscardEnable = VK_FALSE;			 
	rasterizerStateInfo.polygonMode = VK_POLYGON_MODE_FILL;			
	rasterizerStateInfo.lineWidth = 1.0f;								
	rasterizerStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;				
	rasterizerStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;	
	rasterizerStateInfo.depthBiasEnable = VK_FALSE;				
	DN_CORE_SUCCESS_OUTPUT("Rasterizer state created");

	//MULTISAMPLING STATE//
	DN_CORE_INFO_OUTPUT("Creating multisampling state");
	VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
	multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateInfo.sampleShadingEnable = VK_FALSE;					
	multisamplingStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;	
	DN_CORE_SUCCESS_OUTPUT("Multisampling state created");

	//BLENDING STATE//
	DN_CORE_INFO_OUTPUT("Setting up blending attachment state");
	VkPipelineColorBlendAttachmentState colourBlendAttachmentState = {};
	colourBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colourBlendAttachmentState.blendEnable = VK_TRUE;													
	colourBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colourBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colourBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colourBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colourBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colourBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	DN_CORE_SUCCESS_OUTPUT("blending attacgment state set up");

	DN_CORE_INFO_OUTPUT("Creating blending state");
	VkPipelineColorBlendStateCreateInfo colourBlendingStateInfo = {};
	colourBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colourBlendingStateInfo.logicOpEnable = VK_FALSE;				
	colourBlendingStateInfo.attachmentCount = 1;
	colourBlendingStateInfo.pAttachments = &colourBlendAttachmentState;
	DN_CORE_SUCCESS_OUTPUT("Blending state created");

	//PIPELINE LAYOUT//
	std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = { m_DescriptorSetLayout, m_SamplerSetLayout };

	DN_CORE_INFO_OUTPUT("Creating pipeline layout");
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &m_PushConstantRange;

	VkResult result = vkCreatePipelineLayout(m_MainDevice.m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("failed to create ppeline layout");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Pipeline layout created");
	
	//DEPTH STENCIL STATE//
	DN_CORE_INFO_OUTPUT("Creating depth stencil state");
	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateInfo.depthTestEnable = VK_TRUE;				
	depthStencilStateInfo.depthWriteEnable = VK_TRUE;				
	depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;		
	depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;		
	depthStencilStateInfo.stencilTestEnable = VK_FALSE;		
	DN_CORE_SUCCESS_OUTPUT("Depth stencil created");

	//GRAPHICS PIPELINE//
	DN_CORE_INFO_OUTPUT("Creating graphics pipeline");
	VkGraphicsPipelineCreateInfo GraphicsPipelineInfo = {};
	GraphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	GraphicsPipelineInfo.stageCount = 2;									
	GraphicsPipelineInfo.pStages = shaderStages;							
	GraphicsPipelineInfo.pVertexInputState = &vertexInputStageInfo;		
	GraphicsPipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
	GraphicsPipelineInfo.pViewportState = &viewportStateInfo;
	GraphicsPipelineInfo.pDynamicState = nullptr;
	GraphicsPipelineInfo.pRasterizationState = &rasterizerStateInfo;
	GraphicsPipelineInfo.pMultisampleState = &multisamplingStateInfo;
	GraphicsPipelineInfo.pColorBlendState = &colourBlendingStateInfo;
	GraphicsPipelineInfo.pDepthStencilState = &depthStencilStateInfo;
	GraphicsPipelineInfo.layout = m_PipelineLayout;							
	GraphicsPipelineInfo.renderPass = m_RenderPass;							
	GraphicsPipelineInfo.subpass = 0;										
	GraphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;	
	GraphicsPipelineInfo.basePipelineIndex = -1;				

	result = vkCreateGraphicsPipelines(m_MainDevice.m_LogicalDevice, VK_NULL_HANDLE, 1, &GraphicsPipelineInfo, nullptr, &m_GraphicsPipeline);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a graphics pipeline");
		throw std::runtime_error("");
	}
		
	DN_CORE_SUCCESS_OUTPUT("Graphics pipeline created");
	
	vkDestroyShaderModule(m_MainDevice.m_LogicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(m_MainDevice.m_LogicalDevice, vertexShaderModule, nullptr);
}

void Renderer::CreateDescriptorSetLayout()
{
	//VIEW PROJECTION DESCRIPTOR SET LAYOUT//
	DN_CORE_INFO_OUTPUT("Setting up view projection layout binding");
	VkDescriptorSetLayoutBinding viewProjectionLayoutBinding = {};
	viewProjectionLayoutBinding.binding = 0;											
	viewProjectionLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;	
	viewProjectionLayoutBinding.descriptorCount = 1;									
	viewProjectionLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;				
	viewProjectionLayoutBinding.pImmutableSamplers = nullptr;							
	DN_CORE_SUCCESS_OUTPUT("View projection layout binding setup");
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings = { viewProjectionLayoutBinding };

	DN_CORE_INFO_OUTPUT("Setting up projection layout");
	VkDescriptorSetLayoutCreateInfo viewProjectionlayoutInfo = {};
	viewProjectionlayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	viewProjectionlayoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());	
	viewProjectionlayoutInfo.pBindings = layoutBindings.data();								

	VkResult result = vkCreateDescriptorSetLayout(m_MainDevice.m_LogicalDevice, &viewProjectionlayoutInfo, nullptr, &m_DescriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create view projection descriptor set layout");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("View projection layout setup");

	//TEXTURE DESCRIPTOR SET LAYOUT//
	DN_CORE_INFO_OUTPUT("Setting up sampler layout binding");
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	DN_CORE_SUCCESS_OUTPUT("Sampler layout binding setup");

	DN_CORE_INFO_OUTPUT("Creating texture layout");
	VkDescriptorSetLayoutCreateInfo textureLayoutInfo = {};
	textureLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	textureLayoutInfo.bindingCount = 1;
	textureLayoutInfo.pBindings = &samplerLayoutBinding;

	result = vkCreateDescriptorSetLayout(m_MainDevice.m_LogicalDevice, &textureLayoutInfo, nullptr, &m_SamplerSetLayout);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create sampler descriptor Set layout");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Texture layout created");
}

void Renderer::CreatePushConstantRange() 
{
	//PUSH CONATANT VALUES//
	DN_CORE_INFO_OUTPUT("Setting up push constant data");
	m_PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	
	m_PushConstantRange.offset = 0;								
	m_PushConstantRange.size = sizeof(Model);
	DN_CORE_SUCCESS_OUTPUT("Push constant data setup");
}

void Renderer::CreateDepthBufferImage()
{
	//DEPTH BUFFER IMAGE//
	DN_CORE_INFO_OUTPUT("Setting up push constant data");
	VkFormat depthFormat = SelectSupportedFormat({ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	m_DepthBufferImage = CreateImage(m_SwapChainExtent.width, m_SwapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_DepthBufferImageMemory);
	m_DepthBufferImageView = CreateImageView(m_DepthBufferImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	DN_CORE_SUCCESS_OUTPUT("Push constant data setup");
}

void Renderer::CreateFramebuffers()
{
	//FRAME BUFFER//
	m_SwapChainFramebuffers.resize(m_SwapChainImages.size());
	DN_CORE_INFO_OUTPUT("Creating all frame buffers");
	for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++)
	{
		std::array<VkImageView, 2> attachments = 
		{
			m_SwapChainImages[i].m_ImageView,
			m_DepthBufferImageView
		};

		DN_CORE_INFO_OUTPUT("Creating frame buffer");
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_RenderPass;										
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();							
		framebufferInfo.width = m_SwapChainExtent.width;								
		framebufferInfo.height = m_SwapChainExtent.height;								
		framebufferInfo.layers = 1;													

		VkResult result = vkCreateFramebuffer(m_MainDevice.m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);
		if (result != VK_SUCCESS)
		{
			DN_CORE_ERROR_OUTPUT("Failed to create a framebuffer");
			throw std::runtime_error("");
		}
		DN_CORE_SUCCESS_OUTPUT("Frame buffer created");	
	}
}

void Renderer::CreateCommandPool()
{
	//COMMAND POOL//
	QueueFamilyIndices queueFamilyIndices = GetQueueFamilies(m_MainDevice.m_PhysicalDevice);

	DN_CORE_INFO_OUTPUT("Creating command pool");
	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily;	

	VkResult result = vkCreateCommandPool(m_MainDevice.m_LogicalDevice, &commandPoolInfo, nullptr, &m_GraphicsCommandPool);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a command pool");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Command pool created");
}

void Renderer::CreateCommandBuffers()
{
	//COMMAND BUFFER//
	m_CommandBuffers.resize(m_SwapChainFramebuffers.size());
	DN_CORE_INFO_OUTPUT("Creating command pool");
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = m_GraphicsCommandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;														
	commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

	VkResult result = vkAllocateCommandBuffers(m_MainDevice.m_LogicalDevice, &commandBufferAllocateInfo, m_CommandBuffers.data());
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to allocate command buffers");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Command buffer created");
}

void Renderer::CreateSynchronisation()
{
	m_ImageAvailable.resize(MAX_FRAME_DRAWS);
	m_RenderFinished.resize(MAX_FRAME_DRAWS);
	m_DrawFences.resize(MAX_FRAME_DRAWS);

	//SEMAPHORE//
	DN_CORE_INFO_OUTPUT("Creating semaphore");
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	//FENCE//
	DN_CORE_INFO_OUTPUT("Creating fence");
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		if (vkCreateSemaphore(m_MainDevice.m_LogicalDevice, &semaphoreInfo, nullptr, &m_ImageAvailable[i]) != VK_SUCCESS ||
			vkCreateSemaphore(m_MainDevice.m_LogicalDevice, &semaphoreInfo, nullptr, &m_RenderFinished[i]) != VK_SUCCESS ||
			vkCreateFence(m_MainDevice.m_LogicalDevice, &fenceInfo, nullptr, &m_DrawFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a semaphore/fence");
		}
		DN_CORE_SUCCESS_OUTPUT("Semaphore/Fence created");
	}
}

void Renderer::CreateTextureSampler()
{
	//SAMPLER//
	DN_CORE_INFO_OUTPUT("Creating texture sampler");
	VkSamplerCreateInfo TextureSamplerInfo = {};
	TextureSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureSamplerInfo.magFilter = VK_FILTER_LINEAR;						
	TextureSamplerInfo.minFilter = VK_FILTER_LINEAR;						
	TextureSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;	
	TextureSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;	
	TextureSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;	
	TextureSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;	
	TextureSamplerInfo.unnormalizedCoordinates = VK_FALSE;				
	TextureSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;		
	TextureSamplerInfo.mipLodBias = 0.0f;								
	TextureSamplerInfo.minLod = 0.0f;									
	TextureSamplerInfo.maxLod = 0.0f;									
	TextureSamplerInfo.anisotropyEnable = VK_TRUE;						
	TextureSamplerInfo.maxAnisotropy = 16;								

	VkResult result = vkCreateSampler(m_MainDevice.m_LogicalDevice, &TextureSamplerInfo, nullptr, &m_TextureSampler);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a texture sampler");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Texture sampler created");	
}

void Renderer::CreateUniformBuffers()
{
	//VIEW PROJECTION UNIFORM BUFFER//
	DN_CORE_INFO_OUTPUT("Creating View projection uniform buffer");
	VkDeviceSize viewProjectionBufferSize = sizeof(UniformBufferObjectViewProjection);
	m_ViewProjectionUniformBuffer.resize(m_SwapChainImages.size());
	m_ViewProjectionUniformBufferMemory.resize(m_SwapChainImages.size());

	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		CreateBuffer(m_MainDevice.m_PhysicalDevice, m_MainDevice.m_LogicalDevice, viewProjectionBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &m_ViewProjectionUniformBuffer[i], &m_ViewProjectionUniformBufferMemory[i]);
		DN_CORE_SUCCESS_OUTPUT("View projection uniform buffer created");
	}
}

void Renderer::CreateDescriptorPool()
{
	//VIEW PROJECTION DESCRIPTOR POOL//
	DN_CORE_INFO_OUTPUT("Creating view projection descriptor pool");
	VkDescriptorPoolSize viewProjectionPoolSize = {};
	viewProjectionPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	viewProjectionPoolSize.descriptorCount = static_cast<uint32_t>(m_ViewProjectionUniformBuffer.size());

	std::vector<VkDescriptorPoolSize> descriptorPoolSizes = { viewProjectionPoolSize };

	VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.maxSets = static_cast<uint32_t>(m_SwapChainImages.size());				
	descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());		
	descriptorPoolInfo.pPoolSizes = descriptorPoolSizes.data();								

	VkResult result = vkCreateDescriptorPool(m_MainDevice.m_LogicalDevice, &descriptorPoolInfo, nullptr, &m_DescriptorPool);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create view projection descriptor pool");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("View projection descriptor created");
	
	//SAMPLER DESCRIPTOR POOL//
	DN_CORE_INFO_OUTPUT("Creating sampler descriptor pool");
	VkDescriptorPoolSize samplerDescriptorPoolSize = {};
	samplerDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerDescriptorPoolSize.descriptorCount = MAX_OBJECTS;

	VkDescriptorPoolCreateInfo samplerPoolInfo = {};
	samplerPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	samplerPoolInfo.maxSets = MAX_OBJECTS;
	samplerPoolInfo.poolSizeCount = 1;
	samplerPoolInfo.pPoolSizes = &samplerDescriptorPoolSize;

	result = vkCreateDescriptorPool(m_MainDevice.m_LogicalDevice, &samplerPoolInfo, nullptr, &m_SamplerDescriptorPool);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create sampler descriptor pool");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Sampler descriptor created");
}

void Renderer::CreateDescriptorSets()
{
	m_DescriptorSets.resize(m_SwapChainImages.size());
	std::vector<VkDescriptorSetLayout> setLayouts(m_SwapChainImages.size(), m_DescriptorSetLayout);

	//ALLOCATE DESCRIPTOR SET//
	DN_CORE_INFO_OUTPUT("Allocating descriptor pool");
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = m_DescriptorPool;									
	descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(m_SwapChainImages.size());
	descriptorSetAllocateInfo.pSetLayouts = setLayouts.data();							

	VkResult result = vkAllocateDescriptorSets(m_MainDevice.m_LogicalDevice, &descriptorSetAllocateInfo, m_DescriptorSets.data());
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to allocate descriptor sets");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Descriptor pool allocated");

	for (size_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		//VIEW PROJECTION DESCRIPTOR//
		DN_CORE_INFO_OUTPUT("Setting up view projection buffer");
		VkDescriptorBufferInfo viewProjectionBufferInfo = {};
		viewProjectionBufferInfo.buffer = m_ViewProjectionUniformBuffer[i];		
		viewProjectionBufferInfo.offset = 0;					
		viewProjectionBufferInfo.range = sizeof(UniformBufferObjectViewProjection);		
		DN_SUCCESS_OUTPUT("View projection buffer setup");

		//BINDING DATA//
		DN_CORE_INFO_OUTPUT("Write view projection buffer");
		VkWriteDescriptorSet writeViewProjectionSet = {};
		writeViewProjectionSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeViewProjectionSet.dstSet = m_DescriptorSets[i];								
		writeViewProjectionSet.dstBinding = 0;											
		writeViewProjectionSet.dstArrayElement = 0;									
		writeViewProjectionSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;		
		writeViewProjectionSet.descriptorCount = 1;									
		writeViewProjectionSet.pBufferInfo = &viewProjectionBufferInfo;							

		std::vector<VkWriteDescriptorSet> setWrites = { writeViewProjectionSet };
		vkUpdateDescriptorSets(m_MainDevice.m_LogicalDevice, static_cast<uint32_t>(setWrites.size()), setWrites.data(), 0, nullptr);
		DN_CORE_SUCCESS_OUTPUT("View projection buffer written");
	}
}

VkImage Renderer::CreateImage(uint32_t _width, uint32_t _height, VkFormat _format, VkImageTiling _tiling, VkImageUsageFlags _useFlags, VkMemoryPropertyFlags _propFlags, VkDeviceMemory* _imageMemory)
{
	//CREATE IMAGE//
	DN_CORE_INFO_OUTPUT("Creating image");
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;						
	imageCreateInfo.extent.width = _width;							
	imageCreateInfo.extent.height = _height;								
	imageCreateInfo.extent.depth = 1;								
	imageCreateInfo.mipLevels = 1;										
	imageCreateInfo.arrayLayers = 1;									
	imageCreateInfo.format = _format;									
	imageCreateInfo.tiling = _tiling;									
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;			
	imageCreateInfo.usage = _useFlags;									
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;					
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;			

	VkImage image;
	VkResult result = vkCreateImage(m_MainDevice.m_LogicalDevice, &imageCreateInfo, nullptr, &image);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create an image");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Image created");

	//CREATE MEMORY//
	DN_CORE_INFO_OUTPUT("Setting up memory requirements");
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(m_MainDevice.m_LogicalDevice, image, &memoryRequirements);
	DN_CORE_SUCCESS_OUTPUT("Memory requirements setup");

	DN_CORE_INFO_OUTPUT("allocating memory requirements");
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryTypeIndex(m_MainDevice.m_PhysicalDevice, memoryRequirements.memoryTypeBits, _propFlags);

	result = vkAllocateMemory(m_MainDevice.m_LogicalDevice, &memoryAllocateInfo, nullptr, _imageMemory);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to allocate memory for image");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Memory requirements Allocated");

	vkBindImageMemory(m_MainDevice.m_LogicalDevice, image, *_imageMemory, 0);

	return image;
}

VkImageView Renderer::CreateImageView(VkImage _image, VkFormat _format, VkImageAspectFlags _aspectFlags)
{
	//IMAGE VIEW//
	DN_CORE_INFO_OUTPUT("Creating image view");
	VkImageViewCreateInfo imageViewInfo = {};
	imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewInfo.image = _image;											
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;						
	imageViewInfo.format = _format;											
	imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;			
	imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.subresourceRange.aspectMask = _aspectFlags;				
	imageViewInfo.subresourceRange.baseMipLevel = 0;						
	imageViewInfo.subresourceRange.levelCount = 1;							
	imageViewInfo.subresourceRange.baseArrayLayer = 0;						
	imageViewInfo.subresourceRange.layerCount = 1;							

	VkImageView imageView;
	VkResult result = vkCreateImageView(m_MainDevice.m_LogicalDevice, &imageViewInfo, nullptr, &imageView);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create an image view");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Image view created");
		
	return imageView;
}

VkShaderModule Renderer::CreateShaderModule(const std::vector<char>& _code)
{
	//SHADER MODULE//
	DN_CORE_INFO_OUTPUT("Creating shader module");
	VkShaderModuleCreateInfo shaderModuleInfo = {};
	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = _code.size();										
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(_code.data());		

	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(m_MainDevice.m_LogicalDevice, &shaderModuleInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create a shader module");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Shader module created");
	
	return shaderModule;
}

int Renderer::CreateTextureImage(std::string _fileName)
{
	//LOAD IMAGE//
	DN_CORE_INFO_OUTPUT("Loading image");
	int width, height;
	VkDeviceSize imageSize;
	stbi_uc* imageData = LoadTextureFile(_fileName, &width, &height, &imageSize);
	DN_CORE_SUCCESS_OUTPUT("Image loaded");

	//STAGING BUFFER//
	DN_CORE_INFO_OUTPUT("Creating staging buffer");
	VkBuffer imageStagingBuffer;
	VkDeviceMemory imageStagingBufferMemory;
	CreateBuffer(m_MainDevice.m_PhysicalDevice, m_MainDevice.m_LogicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &imageStagingBuffer, &imageStagingBufferMemory);
	DN_CORE_SUCCESS_OUTPUT("Staging buffer created");

	//COPY IMAGE//
	DN_CORE_INFO_OUTPUT("Copying image data");
	void* data;
	vkMapMemory(m_MainDevice.m_LogicalDevice, imageStagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imageData, static_cast<size_t>(imageSize));
	vkUnmapMemory(m_MainDevice.m_LogicalDevice, imageStagingBufferMemory);
	stbi_image_free(imageData);
	DN_CORE_SUCCESS_OUTPUT("Image data copied");

	//IMAGE TEXTURE//
	DN_CORE_INFO_OUTPUT("Creating texture image");
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	textureImage = CreateImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImageMemory);
	DN_CORE_SUCCESS_OUTPUT("Texture image created");

	//DATA TRANSITION//
	DN_CORE_INFO_OUTPUT("Transitioning image data");
	TransitionImageLayout(m_MainDevice.m_LogicalDevice, m_GraphicsQueue, m_GraphicsCommandPool, textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyImageBuffer(m_MainDevice.m_LogicalDevice, m_GraphicsQueue, m_GraphicsCommandPool, imageStagingBuffer, textureImage, width, height);
	TransitionImageLayout(m_MainDevice.m_LogicalDevice, m_GraphicsQueue, m_GraphicsCommandPool, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	m_TextureImages.push_back(textureImage);
	m_TextureImageMemory.push_back(textureImageMemory);
	vkDestroyBuffer(m_MainDevice.m_LogicalDevice, imageStagingBuffer, nullptr);
	vkFreeMemory(m_MainDevice.m_LogicalDevice, imageStagingBufferMemory, nullptr);
	DN_CORE_SUCCESS_OUTPUT("Image data transitioned");

	return m_TextureImages.size() - 1;
}

int Renderer::CreateTexture(std::string _fileName)
{
	//TEXTURE PROPERTIES//
	DN_CORE_INFO_OUTPUT("Setting up texture properties");
	int textureImageLoc = CreateTextureImage(_fileName);
	VkImageView m_ImageView = CreateImageView(m_TextureImages[textureImageLoc], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	m_TextureImageViews.push_back(m_ImageView);
	int descriptorLoc = CreateTextureDescriptor(m_ImageView);
	DN_CORE_SUCCESS_OUTPUT("Texture properties setup");
	return descriptorLoc;
}

int Renderer::CreateTextureDescriptor(VkImageView _textureImage)
{
	//DESCRIPTOR SET//
	VkDescriptorSet descriptorSet;

	DN_CORE_INFO_OUTPUT("Allocating texture descriptor");
	VkDescriptorSetAllocateInfo setAllocateInfo = {};
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.descriptorPool = m_SamplerDescriptorPool;
	setAllocateInfo.descriptorSetCount = 1;
	setAllocateInfo.pSetLayouts = &m_SamplerSetLayout;

	VkResult result = vkAllocateDescriptorSets(m_MainDevice.m_LogicalDevice, &setAllocateInfo, &descriptorSet);
	if (result != VK_SUCCESS)
	{
		DN_CORE_ERROR_OUTPUT("Failed to allocate texture descriptor sets");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Texture descriptor allocated");

	// TEXTURE INFORMATION//
	DN_CORE_INFO_OUTPUT("Setting up image descriptor");
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;	
	imageInfo.imageView = _textureImage;									
	imageInfo.sampler = m_TextureSampler;		
	DN_CORE_SUCCESS_OUTPUT("Image descriptor setup");

	DN_CORE_INFO_OUTPUT("Writing image descriptor set");
	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(m_MainDevice.m_LogicalDevice, 1, &writeDescriptorSet, 0, nullptr);
	m_SamplerDescriptorSets.push_back(descriptorSet);
	DN_CORE_SUCCESS_OUTPUT("Image descriptor written");
	return m_SamplerDescriptorSets.size() - 1;
}

#pragma endregion

#pragma region Getter Functions

void Renderer::GetPhysicalDevice()
{
	DN_CORE_INFO_OUTPUT("Getting physical device");
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		DN_CORE_ERROR_OUTPUT("Can't find GPU that supports instance");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("GPU found that supports instance");

	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(m_Instance, &deviceCount, deviceList.data());

	//ALLOCATE DEVICE//
	for (const auto& _device : deviceList)
	{
		if (CheckDeviceSuitable(_device))
		{
			m_MainDevice.m_PhysicalDevice = _device;
			DN_CORE_SUCCESS_OUTPUT("Physical device selected");
			break;
		}
	}

	//NEW DEVICE PROPERTIES//
	DN_CORE_INFO_OUTPUT("Setting physical device properties");
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_MainDevice.m_PhysicalDevice, &deviceProperties);
	DN_CORE_SUCCESS_OUTPUT("Physical device properties setup");
}

QueueFamilyIndices Renderer::GetQueueFamilies(VkPhysicalDevice _physicalDevice)
{
	DN_CORE_INFO_OUTPUT("Getting queue families");
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilyList.data());

	int index = 0;
	for (const auto& _queueFamily : queueFamilyList)
	{
		if (_queueFamily.queueCount > 0 && _queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.m_GraphicsFamily = index;		

		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, index, m_Surface, &presentationSupport);

		if (_queueFamily.queueCount > 0 && presentationSupport)
			indices.m_PresentationFamily = index;

		if (indices.IsValid())
			break;

		index++;
	}
	DN_CORE_SUCCESS_OUTPUT("Queue family selected");

	return indices;
}

SwapChainDetails Renderer::GetSwapChainDetails(VkPhysicalDevice _physicalDevice)
{
	SwapChainDetails swapChainDetails;

	//SURFACE CAPABILITIES & FORMATS//
	DN_CORE_INFO_OUTPUT("Setting up capabilities and formats");
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, m_Surface, &swapChainDetails.m_SurfaceCapabilities);
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, m_Surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		swapChainDetails.m_Formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, m_Surface, &formatCount, swapChainDetails.m_Formats.data());
		DN_CORE_SUCCESS_OUTPUT("Format selected");
	}

	//PRESENTATION MODES//
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, m_Surface, &presentationCount, nullptr);

	if (presentationCount != 0)
	{
		swapChainDetails.m_PresentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, m_Surface, &presentationCount, swapChainDetails.m_PresentationModes.data());
		DN_CORE_SUCCESS_OUTPUT("Presentation mode selected");
	}

	return swapChainDetails;
}

#pragma endregion

#pragma region Checker Functions

bool Renderer::CheckInstanceExtensionSupport(std::vector<const char*>* _checkExtensions)
{
	DN_CORE_INFO_OUTPUT("Checking instance extension support");
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	//AVALIBLE EXTENSIONS//
	for (const auto& _checkExtension : *_checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& _extension : extensions)
		{
			if (strcmp(_checkExtension, _extension.extensionName) == 0)
			{
				hasExtension = true;
				DN_CORE_SUCCESS_OUTPUT("Extension: " + (std::string)_extension.extensionName + " supported");
				break;
			}
		}

		if (!hasExtension)
		{
			DN_CORE_WARNING_OUTPUT("Extension: " + (std::string)_checkExtension + " not supported");
			return false;
		}	
	}
	DN_CORE_SUCCESS_OUTPUT("Instance extensions checked");

	return true;
}

bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice _physicalDevice)
{
	DN_CORE_INFO_OUTPUT("Checking device extension support");
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);

	if (extensionCount == 0)
	{
		DN_CORE_WARNING_OUTPUT("Device extensions not supported");
		return false;
	}

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, extensions.data());

	//AVALIBLE EXTENSIONS//
	for (const auto& _deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto& _extension : extensions)
		{
			if (strcmp(_deviceExtension, _extension.extensionName) == 0)
			{
				hasExtension = true;
				DN_CORE_SUCCESS_OUTPUT("Extension: " + (std::string)_extension.extensionName + " supported");
				break;
			}
		}

		if (!hasExtension)
		{
			DN_CORE_WARNING_OUTPUT("Extension: " + (std::string)_deviceExtension + " not supported");
			return false;
		}	
	}
	DN_CORE_SUCCESS_OUTPUT("Device extensions checked");

	return true;
}

bool Renderer::CheckDeviceSuitable(VkPhysicalDevice _physicalDevice)
{
	DN_CORE_INFO_OUTPUT("Checking device suitability");
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(_physicalDevice, &physicalDeviceFeatures);
	QueueFamilyIndices indices = GetQueueFamilies(_physicalDevice);
	bool extensionsSupported = CheckDeviceExtensionSupport(_physicalDevice);

	bool swapChainValid = false;
	if (extensionsSupported)
	{
		SwapChainDetails swapChainDetails = GetSwapChainDetails(_physicalDevice);
		swapChainValid = !swapChainDetails.m_PresentationModes.empty() && !swapChainDetails.m_Formats.empty();
	}
	DN_CORE_SUCCESS_OUTPUT("Device suitable");

	return indices.IsValid() && extensionsSupported && swapChainValid && physicalDeviceFeatures.samplerAnisotropy;
}

#pragma endregion

#pragma region Select Functions

VkSurfaceFormatKHR Renderer::SelectBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _formats)
{
	DN_CORE_INFO_OUTPUT("Selecting best surface format");
	//ALL formats available
	if (_formats.size() == 1 && _formats[0].format == VK_FORMAT_UNDEFINED)
	{
		DN_CORE_WARNING_OUTPUT("Default surface format selected");
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}
		

	//Search optimal format
	for (const auto& _format : _formats)
	{
		if ((_format.format == VK_FORMAT_R8G8B8A8_UNORM || _format.format == VK_FORMAT_B8G8R8A8_UNORM) && _format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			DN_CORE_SUCCESS_OUTPUT("Optimal surface format selected");
			return _format;
		}
	}

	//Can't find optimal format
	DN_CORE_WARNING_OUTPUT("Can't find optimal surface format");
	return _formats[0];
}

VkPresentModeKHR Renderer::SelectBestPresentationMode(const std::vector<VkPresentModeKHR> _PresentationModes)
{
	DN_CORE_INFO_OUTPUT("Selecting best presentation mode");
	for (const auto& _presentationMode : _PresentationModes)
	{
		if (_presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			DN_CORE_SUCCESS_OUTPUT("Mailbox KHR presentation mode selected");
			return _presentationMode;
		}	
	}

	//DEFAULT PRESENTATION MODE//
	DN_CORE_WARNING_OUTPUT("Default presentation mode selected");
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& _SurfaceCapabilities)
{
	DN_CORE_INFO_OUTPUT("Selecting swap chain extent");
	if (_SurfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		DN_CORE_SUCCESS_OUTPUT("Swap chain extent selected");
		return _SurfaceCapabilities.currentExtent;
	}
		
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		VkExtent2D extent2D = {};
		extent2D.width = static_cast<uint32_t>(width);
		extent2D.height = static_cast<uint32_t>(height);
		extent2D.width = std::max(_SurfaceCapabilities.minImageExtent.width, std::min(_SurfaceCapabilities.maxImageExtent.width, extent2D.width));
		extent2D.height = std::max(_SurfaceCapabilities.minImageExtent.height, std::min(_SurfaceCapabilities.maxImageExtent.height, extent2D.height));

		DN_CORE_SUCCESS_OUTPUT("Swap chain extent setup");
		return extent2D;
	}
}

VkFormat Renderer::SelectSupportedFormat(const std::vector<VkFormat>& _formats, VkImageTiling _tiling, VkFormatFeatureFlags _featureFlags)
{
	DN_CORE_INFO_OUTPUT("Selecting supported format");
	for (VkFormat _format : _formats)
	{
		//DEVICE PROPERTIES//
		VkFormatProperties FormatProperties;
		vkGetPhysicalDeviceFormatProperties(m_MainDevice.m_PhysicalDevice, _format, &FormatProperties);

		if (_tiling == VK_IMAGE_TILING_LINEAR && (FormatProperties.linearTilingFeatures & _featureFlags) == _featureFlags)
		{
			DN_CORE_SUCCESS_OUTPUT("Linear tiling format selected");
			return _format;
		}
		else if (_tiling == VK_IMAGE_TILING_OPTIMAL && (FormatProperties.optimalTilingFeatures & _featureFlags) == _featureFlags)
		{
			DN_CORE_SUCCESS_OUTPUT("optimal tiling format selected");
			return _format;
		}
	}
	DN_CORE_ERROR_OUTPUT("Failed to find a matching format");
	throw std::runtime_error("");
}

#pragma endregion

#pragma region Record Functions

void Renderer::RecordCommands(uint32_t _currentImage)
{
	//COMMAND BUFFER//
	//DN_CORE_INFO_OUTPUT("Beginning command buffer");
	VkCommandBufferBeginInfo commandBufferInfo = {};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//REDNDER PASS//
	//DN_CORE_INFO_OUTPUT("Beginning render pass");
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass;							
	renderPassInfo.renderArea.offset = { 0, 0 };						
	renderPassInfo.renderArea.extent = m_SwapChainExtent;				

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.6f, 0.65f, 0.4f, 1.0f };
	clearValues[1].depthStencil.depth = 1.0f;

	renderPassInfo.pClearValues = clearValues.data();					
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.framebuffer = m_SwapChainFramebuffers[_currentImage];

	//RECORDING//
	//DN_CORE_INFO_OUTPUT("Starting to record commands");
	VkResult result = vkBeginCommandBuffer(m_CommandBuffers[_currentImage], &commandBufferInfo);
	if (result != VK_SUCCESS)
	{
		//DN_CORE_ERROR_OUTPUT("Failed to start recording a command buffer");
		throw std::runtime_error("");
	}
	//DN_CORE_SUCCESS_OUTPUT("Command buffer recording");
	
	vkCmdBeginRenderPass(m_CommandBuffers[_currentImage], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(m_CommandBuffers[_currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

	for (size_t i = 0; i < m_ModelList.size(); i++)
	{
		MeshModel model = m_ModelList[i];
		vkCmdPushConstants(m_CommandBuffers[_currentImage], m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Model),	&model.GetModel());			
		//DN_CORE_SUCCESS_OUTPUT("Push constants recorded");

		for (size_t j = 0; j < model.GetMeshCount(); j++)
		{
			VkBuffer vertexBuffers[] = { model.GetMesh(j)->GetVertexBuffer() };					
			VkDeviceSize offsets[] = { 0 };	

			vkCmdBindVertexBuffers(m_CommandBuffers[_currentImage], 0, 1, vertexBuffers, offsets);	
			//DN_CORE_SUCCESS_OUTPUT("Vertex buffers recorded");
			vkCmdBindIndexBuffer(m_CommandBuffers[_currentImage], model.GetMesh(j)->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
			//DN_CORE_SUCCESS_OUTPUT("Index buffer recorded");

			std::array<VkDescriptorSet, 2> descriptorSetGroup = { m_DescriptorSets[_currentImage], m_SamplerDescriptorSets[model.GetMesh(j)->GetTextureId()] };
			vkCmdBindDescriptorSets(m_CommandBuffers[_currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0, nullptr);
			//DN_CORE_SUCCESS_OUTPUT("Descriptor sets recorded");

			vkCmdDrawIndexed(m_CommandBuffers[_currentImage], model.GetMesh(j)->GetIndexCount(), 1, 0, 0, 0);
			//DN_CORE_SUCCESS_OUTPUT("Draw index recorded");
		}
	}
	vkCmdEndRenderPass(m_CommandBuffers[_currentImage]);
	//DN_CORE_SUCCESS_OUTPUT("Command buffer ended render pass recording");

	result = vkEndCommandBuffer(m_CommandBuffers[_currentImage]);
	if (result != VK_SUCCESS)
	{
		//DN_CORE_ERROR_OUTPUT("Failed to stop recording a command buffer");
		throw std::runtime_error("");
	}
	//DN_CORE_SUCCESS_OUTPUT("Recording finished");
}

#pragma endregion

#pragma region Update Functions

void Renderer::UpdateUniformBuffers(uint32_t _imageIndex)
{
	//VIEW PROJECTION DATA//
	//DN_CORE_INFO_OUTPUT("Setting up view projection data");
	void* data;
	vkMapMemory(m_MainDevice.m_LogicalDevice, m_ViewProjectionUniformBufferMemory[_imageIndex], 0, sizeof(UniformBufferObjectViewProjection), 0, &data);
	memcpy(data, &m_UniformBufferObjectViewProjection, sizeof(UniformBufferObjectViewProjection));
	vkUnmapMemory(m_MainDevice.m_LogicalDevice, m_ViewProjectionUniformBufferMemory[_imageIndex]);
	//DN_CORE_SUCCESS_OUTPUT("View projection data setup");
}

#pragma endregion

#pragma region Load Functions

stbi_uc* Renderer::LoadTextureFile(std::string _fileName, int* _width, int* _height, VkDeviceSize* _imageSize)
{
	DN_CORE_INFO_OUTPUT("Loading texture file");
	int channels;
	std::string fileLocation = "VulkanPrototype/Textures/" + _fileName;
	stbi_uc* image = stbi_load(fileLocation.c_str(), _width, _height, &channels, STBI_rgb_alpha);

	if (!image)
	{
		DN_CORE_ERROR_OUTPUT("Failed to load a texture file:! (" + _fileName + ")");
		throw std::runtime_error("");
	}

	*_imageSize = *_width * *_height * 4;
	DN_CORE_SUCCESS_OUTPUT("Texture file loaded");

	return image;
}

#pragma endregion

#pragma region Usage Functions

void Renderer::UpdateModel(int modelId, glm::mat4 _model)
{
	//DN_CORE_INFO_OUTPUT("Updating model");
	if (modelId >= m_ModelList.size())
	{
		//DN_CORE_WARNING_OUTPUT("ModelId is out of range");
		return;
	}
	m_ModelList[modelId].SetModel(_model);
	//DN_CORE_SUCCESS_OUTPUT("Model updated");
}

int Renderer::CreateMeshModel(std::string _modelFile)
{
	//IMPORT MODEL//
	DN_CORE_INFO_OUTPUT("Creating model mesh");
	Assimp::Importer importer;
	const aiScene* model = importer.ReadFile(_modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!model)
	{
		DN_CORE_ERROR_OUTPUT("Failed to load model: (" + _modelFile + ")");
		throw std::runtime_error("");
	}
	DN_CORE_SUCCESS_OUTPUT("Model loaded from file");

	//TEXTURE ASSIGNMENT//
	DN_CORE_INFO_OUTPUT("Assigning texture to model");
	std::vector<std::string> textureNames = MeshModel::LoadMaterials(model);
	std::vector<int> materialToTexture(textureNames.size());

	for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames[i].empty())
		{
			DN_CORE_WARNING_OUTPUT("No texture data");
			materialToTexture[i] = 0;
		}
		else
		{
			materialToTexture[i] = CreateTexture(textureNames[i]);
			DN_CORE_SUCCESS_OUTPUT("Texture for mesh created");
		}		
	}

    //LOAD MESH//
	DN_CORE_INFO_OUTPUT("Loading mesh");
	std::vector<Mesh> modelMeshes = MeshModel::LoadNode(m_MainDevice.m_PhysicalDevice, m_MainDevice.m_LogicalDevice, m_GraphicsQueue, m_GraphicsCommandPool, model->mRootNode, model, materialToTexture);
	MeshModel meshModel = MeshModel(modelMeshes);
	m_ModelList.push_back(meshModel);
	DN_CORE_SUCCESS_OUTPUT("Mesh loaded");

	return m_ModelList.size() - 1;
}

#pragma endregion
