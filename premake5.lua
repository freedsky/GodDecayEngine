workspace "GodDecayEngine"
    architecture "x64"

    configurations
    {
        "Debug",
		"Release",
		"Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "SandBox"

project "GodDecay"
    location "GodDecay"
    kind "SharedLib"
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
		"%{prj.name}/vendor/spdlog/include"
	}

    filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

        defines
		{
			"GD_PLATFORM_WINDOWS",
			"GD_BUILD_DLL"
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
		"GodDecay/vendor/spdlog/include",
		"GodDecay/src"
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