#pragma once

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

#include "Model.h"

#include "Texture.h"

/// <summary>
/// ����3Dģ����Ⱦ
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
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);

		//DrawCirle---------------------
		static void DrawCirle(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCirle(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);

		//DrawModel---------------------
		static void DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);
	};
}



