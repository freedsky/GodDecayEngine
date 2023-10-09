#include "gdpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GodDecay
{
	struct Renderer2DStorage 
	{
		Ref<VertexArrayBuffer> QuadVertexArray;
		Ref<Shader> BaseShader;
		Ref<Texture2D> WhiteTexture;
	};
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArrayBuffer::Create();

		float squareVertices[4 * 5] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float2,"a_Texcoords"}
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVBO);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIBO);

		//Texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->BaseShader = Shader::Create("assets/shader/Standard2DShader.glsl");
		s_Data->BaseShader->Bind();
		s_Data->BaseShader->SetInt("u_Textue", 0);
	}

	void Renderer2D::Shutdown()
	{
		if (s_Data != nullptr)
			delete s_Data;
		s_Data = nullptr;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->BaseShader->Bind();
		s_Data->BaseShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data->BaseShader->Bind();
		s_Data->BaseShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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
		s_Data->BaseShader->Bind();
		s_Data->BaseShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->BaseShader->SetMat4("u_Transform", transform);


		s_Data->WhiteTexture->Bind();


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->BaseShader->Bind();
		s_Data->BaseShader->SetFloat4("u_Color", glm::vec4(1.0f));

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->BaseShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}