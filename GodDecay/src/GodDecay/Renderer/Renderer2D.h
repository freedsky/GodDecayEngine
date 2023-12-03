#pragma once

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Camera.h"
#include "EditorCamera.h"

#include "Texture.h"

#include "GodDecay/Scene/Components.h"

/// <summary>
/// 采样立即渲染的方式，创建一个2D渲染对象
/// </summary>
namespace GodDecay 
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		//entity渲染
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		//EditorCamera
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		//---------------------
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		//---------------------
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f), const float tilingFactor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& texColor = glm::vec4(1.0f), const float tilingFactor = 1.0f);

		//entt---------------------------------
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		//用于Sprite的渲染
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		//描述绘制的状态
		struct Statistics2D
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics2D GetStats();
	private:
		static void StartBatch();
		static void NextBatch();
	};
}



