#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "PCH.h"

#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "ViewProjection.h"
#include "Manipulation.h"

GLFWwindow* window;
Renderer renderer;
Manipulation manipulation = Manipulation(&renderer);
ViewProjection viewProjection = ViewProjection(&renderer.m_UniformBufferObjectViewProjection.m_Projection, &renderer.m_UniformBufferObjectViewProjection.m_View);

void InitialiseWindow(std::string _name = "The Designation Engine Window", const int _width = 1920, const int _height = 1080)
{
	//Initialise GLFW to not operate with OpenGL

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
}

int main()
{
	//CREATE WINDOW//
	DN_CORE_IMPORTANT_INFO_OUTPUT("Initialising the Designation Engine window");
	InitialiseWindow("The Designation Engine Window", 1920, 1080);
	DN_CORE_SUCCESS_OUTPUT("Designation Engine window initialised");

	//CREATE RENDER INSTANCE//
	DN_CORE_IMPORTANT_INFO_OUTPUT("Creating render instance");
	if (renderer.Initialise(window) == EXIT_FAILURE)
	{
		DN_CORE_ERROR_OUTPUT("Failed to create render instance");
		return EXIT_FAILURE;
	}
	DN_CORE_SUCCESS_OUTPUT("Render instance created");

	manipulation.Instanciate();

	//MAIN LOOP//
	DN_CORE_IMPORTANT_INFO_OUTPUT("Initialising main loop");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		manipulation.Update();

		//CAMERA CONTROL//
		auto UserInput = std::async(std::launch::async, []
			{
				char input;
				std::cin >> input;
				std::cout << std::endl;
				if (viewProjection.updateViewProjection(input))
				{
					return true;
				}
			});

		while (UserInput.wait_for(1s) != std::future_status::ready)
		{
			DN_CORE_INFO_OUTPUT("Draw call");
			renderer.Draw();
		}
	}
	DN_CORE_SUCCESS_OUTPUT("Main loop ended");

	//TERMINATION//
	DN_CORE_IMPORTANT_INFO_OUTPUT("Terminating application");
	renderer.Cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
	DN_CORE_SUCCESS_OUTPUT("Application terminated");

	return 0;
}

