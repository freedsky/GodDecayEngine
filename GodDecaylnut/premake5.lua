project "GodDecaylnut"
    kind "ConsoleApp"
	cppdialect "C++17"
    language "C++"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.src}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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