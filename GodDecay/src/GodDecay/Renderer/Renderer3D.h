#pragma once

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Camera.h"
#include "EditorCamera.h"

#include "Texture.h"

#include "GodDecay/Scene/Components.h"
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
		//entity��Ⱦ
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		//EditorCamera
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		//=====================================================
		//entt---------------------------------
		static void DrawCubeMesh(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawCubeMesh(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		//����Sprite����Ⱦ
		static void DrawCubeMeshRenderer(const glm::mat4& transform, MeshRenderComponent& src, int entityID);

		//�������Ƶ�״̬
		struct Statistics3D
		{
			uint32_t DrawCalls = 0;
			uint32_t CubeCount = 0;

			uint32_t GetTotalVertexCount() { return CubeCount * 36; }
			uint32_t GetTotalIndexCount() { return CubeCount * 36; }
		};
		static void ResetStats();
		static Statistics3D GetStats();
	private:
		static void StartBatch();
		static void NextBatch();

		static void LoadMesh();
	};
}



