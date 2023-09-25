#pragma once
#include "RenderCommand.h"
#include "RendererCamera.h"
#include "Shader.h"


//#include "Shader.h"这里写一个前向声明防止报错，因为Shader还没有成体系所以暂时不写

/// <summary>
/// 该类主要是使用引擎的图像API来进行渲染
/// </summary>
namespace GodDecay 
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const Ref<RendererCamera>& camera);
		static void EndScene();

		static void Submit(Ref<Shader> shader, const Ref<VertexArrayBuffer>& vertexArray, const glm::mat4& transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData> m_SceneData;
	};
}



