#include "gdpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"

#include "VertexArrayBuffer.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtx/transform.hpp>

namespace GodDecay 
{
	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;

	struct Renderer3DStorage 
	{
		Ref<VertexArrayBuffer> CubeVertexArray;
		Ref<VertexArrayBuffer> CirleVertexArray;
		Ref<Shader> StandardShader;
		Ref<Texture2D> WhiteTexture;
	};
	static Renderer3DStorage* s_Data;

	void Renderer3D::Init()
	{
		s_Data = new Renderer3DStorage();

		//---------------------Cube--------------------------
		s_Data->CubeVertexArray = VertexArrayBuffer::Create();

		float cubeData[36 * 8] = 
		{
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
		};

		Ref<VertexBuffer> cubeVBO = VertexBuffer::Create(cubeData, sizeof(cubeData));
		cubeVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float3,"a_Normal"},
			{ShaderDataType::Float2,"a_Coords"}
		});
		s_Data->CubeVertexArray->AddVertexBuffer(cubeVBO);

		uint32_t cubeIndices[6 * 6] =
		{
			0,1,2,3,4,5,
			6,7,8,9,10,11,
			12,13,14,15,16,17,
			18,19,20,21,22,23,
			24,25,26,27,28,29,
			30,31,32,33,34,35
		};
		Ref<IndexBuffer> cubeEBO = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeEBO);

		//---------------------Cirle-------------------------------
		s_Data->CirleVertexArray = VertexArrayBuffer::Create();

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}
		std::vector<float> cirleData;
		for (unsigned int i = 0; i < positions.size(); ++i)
		{
			cirleData.push_back(positions[i].x);
			cirleData.push_back(positions[i].y);
			cirleData.push_back(positions[i].z);
			if (normals.size() > 0)
			{
				cirleData.push_back(normals[i].x);
				cirleData.push_back(normals[i].y);
				cirleData.push_back(normals[i].z);
			}
			if (uv.size() > 0)
			{
				cirleData.push_back(uv[i].x);
				cirleData.push_back(uv[i].y);
			}
		}

		Ref<VertexBuffer> cirleVBO = VertexBuffer::Create(cirleData.data(), (uint32_t)cirleData.size() * sizeof(float));
		cirleVBO->Setlayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float3,"a_Normal"},
			{ShaderDataType::Float2,"a_Coords"}
		});
		s_Data->CirleVertexArray->AddVertexBuffer(cirleVBO);

		std::vector<uint32_t> cirleIndices;
		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow)
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					cirleIndices.push_back(y * (X_SEGMENTS + 1) + x);
					cirleIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					cirleIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					cirleIndices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		Ref<IndexBuffer> cirleEBO = IndexBuffer::Create(cirleIndices.data(), (uint32_t)cirleIndices.size() * sizeof(uint32_t));
		s_Data->CirleVertexArray->SetIndexBuffer(cirleEBO);

		s_Data->StandardShader = Shader::Create("assets/shader/Standard3DShader.glsl");

		//Texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->StandardShader->Bind();
		s_Data->StandardShader->SetInt("u_Texture", 0);
	}

	void Renderer3D::Shutdown()
	{
		if (s_Data != nullptr)
			delete s_Data;
		s_Data = nullptr;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->StandardShader->Bind();
		s_Data->StandardShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data->StandardShader->Bind();
		s_Data->StandardShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::BeginSceneModel(const PerspectiveCamera& camera, const Ref<Shader>& modelShader)
	{
		modelShader->Bind();
		modelShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer3D::DrawRotationCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	//==================================================================

	void Renderer3D::DrawCirle(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CirleVertexArray, RendererAPI::DrawType::STRIP);
	}

	void Renderer3D::DrawCirle(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data->CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CirleVertexArray, RendererAPI::DrawType::STRIP);
	}

	void Renderer3D::DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CirleVertexArray, RendererAPI::DrawType::STRIP);
	}

	void Renderer3D::DrawRotationCirle(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		s_Data->StandardShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		s_Data->StandardShader->SetMat4("u_Transform", transform);

		s_Data->StandardShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		s_Data->CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CirleVertexArray, RendererAPI::DrawType::STRIP);
	}

	//=========================================================================================================================================

	void Renderer3D::DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		modelData->ModelShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		modelData->ModelShader->SetMat4("u_Transform", transform);

		modelData->ModelShader->SetFloat4("u_Color", color);

		modelData->m_WhiteTexture->Bind();

		for (int i = 0; i < modelData->ModelVertexArray.size();++i) 
		{
			modelData->ModelVertexArray[i]->Bind();
			RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
		}
		
	}
	void Renderer3D::DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		modelData->ModelShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		modelData->ModelShader->SetMat4("u_Transform", transform);

		modelData->ModelShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
		{
			modelData->ModelVertexArray[i]->Bind();
			RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
		}
	}

	void Renderer3D::DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const glm::vec4& color)
	{
		modelData->ModelShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		modelData->ModelShader->SetMat4("u_Transform", transform);

		modelData->ModelShader->SetFloat4("u_Color", color);

		modelData->m_WhiteTexture->Bind();

		for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
		{
			modelData->ModelVertexArray[i]->Bind();
			RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
		}
	}

	void Renderer3D::DrawRotationModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec3& axle, const Ref<Texture2D>& texture, const glm::vec4& texColor)
	{
		modelData->ModelShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axle) * glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
		modelData->ModelShader->SetMat4("u_Transform", transform);

		modelData->ModelShader->SetFloat4("u_Color", texColor);

		texture->Bind();

		for (int i = 0; i < modelData->ModelVertexArray.size(); ++i)
		{
			modelData->ModelVertexArray[i]->Bind();
			RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
		}
	}
}
