project "GodDecay"
    kind "StaticLib"
	cppdialect "C++17"
    language "C++"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gdpch.h"
	pchsource "src/gdpch.cpp"

    files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"yaml",
		"opengl32.lib"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

    filter "system:windows"
		systemversion "latest"

        defines
		{
			"GD_PLATFORM_WINDOWS",
			"GD_BUILD_DLL",
			"GD_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE",
			"IMGUI_DEFINE_MATH_OPERATORS"
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