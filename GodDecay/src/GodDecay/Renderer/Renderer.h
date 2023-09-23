#pragma once
#include "RenderCommand.h"
#include "RendererCamera.h"

//#include "Shader.h"����дһ��ǰ��������ֹ��������ΪShader��û�г���ϵ������ʱ��д
class Shader;

/// <summary>
/// ������Ҫ��ʹ�������ͼ��API��������Ⱦ
/// </summary>
namespace GodDecay 
{
	class Renderer
	{
	public:
		static void BeginScene(const Ref<RendererCamera>& camera);
		static void EndScene();

		static void Submit(Ref<Shader> shader, const Ref<VertexArrayBuffer>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData> m_SceneData;
	};
}


