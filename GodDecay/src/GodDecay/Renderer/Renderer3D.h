#pragma once

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

#include "Model.h"

/// <summary>
/// 增加3D模型渲染
/// </summary>
namespace GodDecay 
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		static void BeginSceneModel(const PerspectiveCamera& camera, const Ref<Shader>& modelShader);
		static void EndScene();

		//DrawCube---------------------
		static void DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		//DrawCirle---------------------
		static void DrawCirle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		//DrawModel---------------------
		static void DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	};
}



