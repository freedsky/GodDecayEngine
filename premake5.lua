workspace "GodDecayEngine_2D"
    architecture "x64"
	startproject "GodDecay-Editor"

    configurations
    {
        "Debug",
		"Release",
		"Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["src"] = "GodDecay/src"
IncludeDir["GLFW"] = "GodDecay/vendor/GLFW/include"
IncludeDir["spdlog"] = "GodDecay/vendor/spdlog/include"
IncludeDir["glad"] = "GodDecay/vendor/glad/include"
IncludeDir["ImGui"] = "GodDecay/vendor/ImGui"
IncludeDir["glm"] = "GodDecay/vendor/glm"
IncludeDir["stb_image"] = "GodDecay/vendor/stb_image"

group "Dependencies"
	include "GodDecay/vendor/GLFW"
	include "GodDecay/vendor/glad"
	include "GodDecay/vendor/ImGui"

project "GodDecay"
    location "GodDecay"
    kind "StaticLib"
	cppdialect "C++17"
    language "C++"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gdpch.h"
	pchsource "GodDecay/src/gdpch.cpp"

    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

    filter "system:windows"
		systemversion "latest"

        defines
		{
			"GD_PLATFORM_WINDOWS",
			"GD_BUILD_DLL",
			"GD_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		filter "configurations:Debug"
			defines "GD_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "GD_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "GD_DIST"
			runtime "Release"
			optimize "on"


project "GodDecay-Editor"
    location "GodDecay-Editor"
    kind "ConsoleApp"
	cppdialect "C++17"
    language "C++"
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
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

    links
	{
		"GodDecay"
	}

    filter "system:windows"
		systemversion "latest"

		defines
		{
			"GD_ENABLE_ASSERTS"
		}

        filter "configurations:Debug"
            defines "GD_DEBUG"
			runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "GD_RELEASE"
			runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "GD_DIST"
			runtime "Release"
            optimize "on"

project "SandBox"
    location "Sandbox"
    kind "ConsoleApp"
	cppdialect "C++17"
    language "C++"
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
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

    links
	{
		"GodDecay"
	}

    filter "system:windows"
		systemversion "latest"

		defines
		{
			"GD_ENABLE_ASSERTS"
		}

        filter "configurations:Debug"
            defines "GD_DEBUG"
			runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "GD_RELEASE"
			runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "GD_DIST"
			runtime "Release"
            optimize "on"