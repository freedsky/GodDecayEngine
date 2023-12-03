#include "gdpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace GodDecay 
{
	Ref<Renderer::SceneData> Renderer::m_SceneData = CreateRef<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		//3D的工作流并不会进行初始化
		//Renderer3D::Init();
	}

	void Renderer::Shutdown() 
	{
		Renderer2D::Shutdown();
		//Renderer3D::Shutdown();
	}

	void Renderer::BeginScene(const Ref<OrthographicCamera>& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}
	void Renderer::BeginScene(const Ref<PerspectiveCamera>& camera)
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
		shader->SetMat4("u_ViewProjection",m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_TransForm",transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

}