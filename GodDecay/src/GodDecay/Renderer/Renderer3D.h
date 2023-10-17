#pragma once

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

#include "Model.h"

#include "Texture.h"

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
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& textures, const glm::vec4& texColor = glm::vec4(1.0f));
		static void DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color);
		static void DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f));

		//DrawCirle---------------------
		static void DrawCirle(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCirle(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f));
		static void DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color);
		static void DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f));

		//DrawModel---------------------
		static void DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f));
		static void DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color);
		static void DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f));
	};
}



