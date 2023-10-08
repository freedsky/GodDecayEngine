#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Shader.h"

//#include "Shader.h"����дһ��ǰ��������ֹ������ΪShader��û�г���ϵ������ʱ��д

/// <summary>
/// ������Ҫ��ʹ�������ͼ��API��������Ⱦ
/// </summary>
namespace GodDecay 
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void BeginScene(const Ref<PerspectiveCamera>& camera);
		static void EndScene();

		static void Submit(Ref<Shader> shader, const Ref<VertexArrayBuffer>& vertexArray, const glm::mat4& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void OnWindowResize(uint32_t width, uint32_t height);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData> m_SceneData;
	};
}



