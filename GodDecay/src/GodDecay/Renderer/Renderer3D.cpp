#include "gdpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"

#include "VertexArrayBuffer.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "GodDecay/Renderer/RenderMesh.h"

#include <glm/gtx/transform.hpp>

namespace GodDecay 
{
	

	struct Renderer3DStorage 
	{
		Ref<RenderMesh> CubeMesh;
		Ref<VertexArrayBuffer> CubeVertexArray;
		Ref<RenderMesh> CirleMesh;
		Ref<VertexArrayBuffer> CirleVertexArray;
		Ref<Shader> StandardShader;
		Ref<Texture2D> WhiteTexture;
	};
	Renderer3DStorage s_Data;

	void Renderer3D::Init()
	{
		//---------------------Cube--------------------------
		s_Data.CubeVertexArray = VertexArrayBuffer::Create();
		s_Data.CubeMesh = CreateRef<RenderCubeMesh>();
		std::vector<float> cubeData;
		for (unsigned int i = 0; i < s_Data.CubeMesh->GetVertexProperties().size(); ++i)
		{
			cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Position.x);
			cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Position.y);
			cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Position.z);
			if (s_Data.CubeMesh->GetVertexProperties()[i].Normal.length > 0)
			{
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Normal.x);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Normal.y);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Normal.z);
			}
			if (s_Data.CubeMesh->GetVertexProperties()[i].TexCoord.length > 0)
			{
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].TexCoord.x);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].TexCoord.y);
			}
			if (s_Data.CubeMesh->GetVertexProperties()[i].Tangent.length > 0)
			{
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Tangent.x);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Tangent.y);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Tangent.z);
			}
			if (s_Data.CubeMesh->GetVertexProperties()[i].Color.length > 0)
			{
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Color.x);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Color.y);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Color.z);
				cubeData.push_back(s_Data.CubeMesh->GetVertexProperties()[i].Color.w);
			}
		}

		Ref<VertexBuffer> cubeVBO = VertexBuffer::Create(cubeData.data(), cubeData.size() * sizeof(float));
		cubeVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float3,"a_Normal"},
			{ShaderDataType::Float2,"a_Coords"},
			{ShaderDataType::Float3,"a_Tangent"},
			{ShaderDataType::Float4,"a_Color"}
		});
		s_Data.CubeVertexArray->AddVertexBuffer(cubeVBO);
		

		
		Ref<IndexBuffer> cubeEBO = IndexBuffer::Create(s_Data.CubeMesh->GetIndices().data(), s_Data.CubeMesh->GetIndices().size() * sizeof(uint32_t));
		s_Data.CubeVertexArray->SetIndexBuffer(cubeEBO);

		//---------------------Cirle------------------------------------------------------
		s_Data.CirleVertexArray = VertexArrayBuffer::Create();
		s_Data.CirleMesh = CreateRef<RenderCirleMesh>();
		std::vector<float> cirleData;

		for (unsigned int i = 0; i < s_Data.CirleMesh->GetVertexProperties().size(); ++i)
		{
			cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Position.x);
			cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Position.y);
			cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Position.z);
			if (s_Data.CirleMesh->GetVertexProperties()[i].Normal.length > 0)
			{
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Normal.x);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Normal.y);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Normal.z);
			}
			if (s_Data.CirleMesh->GetVertexProperties()[i].TexCoord.length > 0)
			{
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].TexCoord.x);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].TexCoord.y);
			}
			if (s_Data.CirleMesh->GetVertexProperties()[i].Tangent.length > 0)
			{
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Tangent.x);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Tangent.y);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Tangent.z);
			}
			if (s_Data.CirleMesh->GetVertexProperties()[i].Color.length > 0)
			{
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Color.x);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Color.y);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Color.z);
				cirleData.push_back(s_Data.CirleMesh->GetVertexProperties()[i].Color.w);
			}
		}

		Ref<VertexBuffer> cirleVBO = VertexBuffer::Create(cirleData.data(), cirleData.size() * sizeof(float));
		cirleVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float3,"a_Normal"},
			{ShaderDataType::Float2,"a_Coords"},
			{ShaderDataType::Float3,"a_Tangent"},
			{ShaderDataType::Float4,"a_Color"}
			});
		s_Data.CirleVertexArray->AddVertexBuffer(cirleVBO);

		Ref<IndexBuffer> cirleEBO = IndexBuffer::Create(s_Data.CirleMesh->GetIndices().data(), s_Data.CirleMesh->GetIndices().size() * sizeof(uint32_t));
		s_Data.CirleVertexArray->SetIndexBuffer(cirleEBO);

		s_Data.StandardShader = Shader::Create("assets/shader/Standard3DShader.glsl");

		//Texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetInt("u_Texture", 0);
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data.StandardShader->Bind();
		s_Data.StandardShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginSceneModel(const PerspectiveCamera& camera, const Ref<Shader>& modelShader)
	{
		modelShader->Bind();
		modelShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::EndScene()
	{
	}

	//================================================================================================
	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", color);

		s_Data.WhiteTexture->Bind();

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		s_Data.CubeVertexArray->UnBind();
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& textures, const glm::vec4& texColor)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", texColor);

		//使用mesh类的方法进行提前的静态绑定，也就是创建的物体是被决定好的
		textures->Bind();

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		s_Data.CubeVertexArray->UnBind();
	}

	void Renderer3D::DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", color);

		s_Data.WhiteTexture->Bind();

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		s_Data.CubeVertexArray->UnBind();
	}

	void Renderer3D::DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
		s_Data.CubeVertexArray->UnBind();
	}

	////==================================================================

	void Renderer3D::DrawCirle(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", color);

		s_Data.WhiteTexture->Bind();

		s_Data.CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CirleVertexArray, RendererAPI::DrawType::STRIP);
		s_Data.CirleVertexArray->UnBind();
	}

	void Renderer3D::DrawCirle(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data.CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CirleVertexArray, RendererAPI::DrawType::STRIP);
		s_Data.CirleVertexArray->UnBind();
	}

	void Renderer3D::DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", color);

		s_Data.WhiteTexture->Bind();

		s_Data.CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CirleVertexArray, RendererAPI::DrawType::STRIP);
		s_Data.CirleVertexArray->UnBind();
	}

	void Renderer3D::DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data.StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data.StandardShader->SetMat4("u_Transform", transform);

		s_Data.StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data.CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CirleVertexArray, RendererAPI::DrawType::STRIP);
		s_Data.CirleVertexArray->UnBind();
	}

	////=========================================================================================================================================

	//void Renderer3D::DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	//{
	//	modelData->ModelShader->Bind();

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
	//	modelData->ModelShader->SetMat4("u_Transform", transform);

	//	modelData->ModelShader->SetFloat4("u_Color", color);

	//	modelData->WhiteTexture->Bind();

	//	for (int i = 0; i < modelData->ModelVertexArray.size();++i) 
	//	{
	//		modelData->ModelVertexArray[i]->Bind();
	//		RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
	//	}
	//	
	//}
	//void Renderer3D::DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	//{
	//	modelData->ModelShader->Bind();

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
	//	modelData->ModelShader->SetMat4("u_Transform", transform);

	//	modelData->ModelShader->SetFloat4("u_Color", texColor);

	//	texture->Bind();

	//	for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
	//	{
	//		modelData->ModelVertexArray[i]->Bind();
	//		RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
	//		modelData->ModelVertexArray[i]->UnBind();
	//	}
	//}

	//void Renderer3D::DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	//{
	//	modelData->ModelShader->Bind();

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
	//	modelData->ModelShader->SetMat4("u_Transform", transform);

	//	modelData->ModelShader->SetFloat4("u_Color", color);

	//	modelData->WhiteTexture->Bind();

	//	for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
	//	{
	//		modelData->ModelVertexArray[i]->Bind();
	//		RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
	//	}
	//}

	//void Renderer3D::DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	//{
	//	modelData->ModelShader->Bind();

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
	//	modelData->ModelShader->SetMat4("u_Transform", transform);

	//	modelData->ModelShader->SetFloat4("u_Color", texColor);

	//	for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
	//	{	
	//		modelData->ModelShader->SetInt("u_Texture", i);
	//		texture->Bind(0);
	//		modelData->ModelVertexArray[i]->Bind();
	//		RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
	//	}
	//}
}
