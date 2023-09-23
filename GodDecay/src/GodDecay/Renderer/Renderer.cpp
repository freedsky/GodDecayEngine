#include "gdpch.h"
#include "Renderer.h"

namespace GodDecay 
{
	Ref<Renderer::SceneData> Renderer::m_SceneData = CreateRef<Renderer::SceneData>();

	void Renderer::BeginScene(const Ref<RendererCamera>& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(Ref<Shader> shader, const Ref<VertexArrayBuffer>& vertexArray)
	{
		//���䵽shader�е�uniform��ȫ�ֱ�����
		//shader->use();
		//shader->Setmatrix4("v_ViewProjection",m_SceneData->ViewProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}