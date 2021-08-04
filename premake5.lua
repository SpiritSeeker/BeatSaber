workspace "BeatSaber"
	architecture "x64"
	startproject "BeatSaber"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	platforms
	{
		"linux",
		"windows",
		"macosx"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ancora/vendor/GLFW/include"
IncludeDir["Glad"] = "Ancora/vendor/Glad/include"
IncludeDir["ImGui"] = "Ancora/vendor/imgui"
IncludeDir["glm"] = "Ancora/vendor/glm"
IncludeDir["stb_image"] = "Ancora/vendor/stb_image"
IncludeDir["assimp"] = "Ancora/vendor/assimp/include"

include "AncoraEngine"

project "BeatSaber"
	location "BeatSaber"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"AncoraEngine/Ancora/vendor/spdlog/include",
		"AncoraEngine/Ancora/src",
		"BeatSaber/src",
		"AncoraEngine/%{IncludeDir.glm}",
		"AncoraEngine/%{IncludeDir.ImGui}",
		"AncoraEngine/%{IncludeDir.assimp}"
	}

	links
	{
		"AncoraEngine",
		"assimp"
	}

	filter "system:linux"

		defines
		{
			"AE_PLATFORM_LINUX",
			"AE_BUILD_DLL"
		}

		links
		{
			"pthread"
		}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AE_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines
		{
			"AE_DEBUG",
			"AE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AE_DIST"
		runtime "Release"
		optimize "on"
