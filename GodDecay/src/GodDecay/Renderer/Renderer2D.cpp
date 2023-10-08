#include "gdpch.h"
#include "Renderer2D.h"

#include "GodDecay/Renderer/RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GodDecay
{
	struct Renderer2DStorage 
	{
		Ref<VertexArrayBuffer> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArrayBuffer::Create();

		float squareVertices[4 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVBO);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		s_Data->FlatColorShader = Shader::Create("assets/shader/PureColor2D.glsl");
	}

	void Renderer2D::Shutdown()
	{
		if (s_Data != nullptr)
			delete s_Data;
		s_Data = nullptr;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::BeginScene(const PerspectiveCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}