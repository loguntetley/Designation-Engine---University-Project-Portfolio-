workspace "VulkanPrototype"
    configurations { "Debug", "Release" }

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VulkanPrototype"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
    objdir ("obj/" .. outputDirectory .. "/%{prj.name}")

	pchheader "PCH.h"
	pchsource "%{prj.name}/PCH.cpp"
	
	forceincludes 
	{ 
		"PCH.h"
	}

    files 
    { 
        "VulkanPrototype/**.h",
        "VulkanPrototype/**.cpp" 
    }
   
    includedirs
    {
        "ThirdParty/GLFW/include/",
        "ThirdParty/GLM/",
        "ThirdParty/Vulkan/VulkanSDk/Include/",
        "VulkanPrototype/",
        "ThirdParty/ASSIMP/include/",
        "ThirdParty/IMGUI/",
        "ThirdParty/IMGUI/backends/"
    }
   
    libdirs 
    { 
        "ThirdParty/GLFW/lib-vc2022",
        "ThirdParty/Vulkan/VulkanSDK/Lib32",
        "ThirdParty/ASSIMP/lib/Release"
    }

    links
    {
        "vulkan-1.lib",
        "glfw3.lib",
        "assimp-vc143-mt.lib"
    }

    filter "configurations:Debug"
        defines { "DN_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "DN_RELEASE" }
        optimize "On"

