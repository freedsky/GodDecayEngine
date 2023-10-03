workspace "GodDecayEngine_3D"
    architecture "x64"
	startproject "SandBox"

    configurations
    {
        "Debug",
		"Release",
		"Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["src"] = "GodDecay_3D/src"
IncludeDir["GLFW"] = "GodDecay_3D/vendor/GLFW/include"
IncludeDir["spdlog"] = "GodDecay_3D/vendor/spdlog/include"
IncludeDir["glad"] = "GodDecay_3D/vendor/glad/include"
IncludeDir["ImGui"] = "GodDecay_3D/vendor/ImGui"
IncludeDir["glm"] = "GodDecay_3D/vendor/glm"
IncludeDir["stb_image"] = "GodDecay_3D/vendor/stb_image"

group "Dependencies"
	include "GodDecay_3D/vendor/GLFW"
	include "GodDecay_3D/vendor/glad"
	include "GodDecay_3D/vendor/ImGui"

project "GodDecay_3D"
    location "GodDecay"
    kind "StaticLib"
	cppdialect "C++17"
    language "C++"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gdpch.h"
	pchsource "GodDecay_3D/src/gdpch.cpp"

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
		"GodDecay_3D"
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