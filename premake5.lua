include "./vendor/premake/premake_customization/solution_items.lua"

workspace "GodDecayEngine_3D"
    architecture "x64"
	startproject "GodDecaylnut"

    configurations
    {
        "Debug",
		"Release",
		"Dist"
    }

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["src"] = "%{wks.location}/GodDecay/src"
IncludeDir["GLFW"] = "%{wks.location}/GodDecay/vendor/GLFW/include"
IncludeDir["spdlog"] = "%{wks.location}/GodDecay/vendor/spdlog/include"
IncludeDir["glad"] = "%{wks.location}/GodDecay/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GodDecay/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/GodDecay/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/GodDecay/vendor/stb_image"
IncludeDir["assimp"] = "%{wks.location}/GodDecay/vendor/assimp/include"
IncludeDir["entt"] = "%{wks.location}/GodDecay/vendor/entt/include"
IncludeDir["yaml"] = "%{wks.location}/GodDecay/vendor/yaml/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/GodDecay/vendor/ImGuizmo"

group "Dependencies"
	include "GodDecay/vendor/GLFW"
	include "GodDecay/vendor/glad"
	include "GodDecay/vendor/ImGui"
	include "GodDecay/vendor/yaml"

group ""

include "GodDecay"
include "GodDecaylnut"
include "SandBox"