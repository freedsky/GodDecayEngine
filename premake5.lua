workspace "GodDecayEngine"
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
IncludeDir["src"] = "GodDecay/src"
IncludeDir["GLFW"] = "GodDecay/vendor/GLFW/include"
IncludeDir["spdlog"] = "GodDecay/vendor/spdlog/include"
IncludeDir["glad"] = "GodDecay/vendor/glad/include"
IncludeDir["ImGui"] = "GodDecay/vendor/ImGui"

include "GodDecay/vendor/GLFW"
include "GodDecay/vendor/glad"
include "GodDecay/vendor/ImGui"

project "GodDecay"
    location "GodDecay"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gdpch.h"
	pchsource "GodDecay/src/gdpch.cpp"

    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

    includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

    filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

        defines
		{
			"GD_PLATFORM_WINDOWS",
			"GD_ENABLE_ASSERTS",
			"GD_BUILD_DLL",
			"GLFW_INCLUDE_NONE"

		}

        postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox")
		}

		filter "configurations:Debug"
		defines "GD_DEBUG"
		symbols "On"

		filter "configurations:Release"
		defines "GD_RELEASE"
		optimize "On"

		filter "configurations:Dist"
		defines "GD_DIST"
		optimize "On"

project "SandBox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}"
	}

    links
	{
		"GodDecay"
	}

    filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GD_PLATFORM_WINDOWS"
		}

        filter "configurations:Debug"
            defines "GD_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "GD_RELEASE"
            optimize "On"

        filter "configurations:Dist"
            defines "GD_DIST"
            optimize "On"