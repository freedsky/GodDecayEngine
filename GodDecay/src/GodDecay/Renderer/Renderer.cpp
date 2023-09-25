#include "gdpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace GodDecay 
{
	Ref<Renderer::SceneData> Renderer::m_SceneData = CreateRef<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(const Ref<RendererCamera>& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(Ref<Shader> shader, const Ref<VertexArrayBuffer>& vertexArray, const glm::mat4& transform)
	{
		//传输到shader中的uniform的全局变量中
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_TransForm",transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}