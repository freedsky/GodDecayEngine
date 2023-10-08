#include "gdpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"

#include "VertexArrayBuffer.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace GodDecay 
{
	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;

	struct Renderer3DStorage 
	{
		Ref<VertexArrayBuffer> CubeVertexArray;
		Ref<VertexArrayBuffer> CirleVertexArray;
		Ref<Shader> FlatColorShader;
	};
	static Renderer3DStorage* s_Data;

	void Renderer3D::Init()
	{
		s_Data = new Renderer3DStorage();

		//---------------------Cube--------------------------
		s_Data->CubeVertexArray = VertexArrayBuffer::Create();

		float cubeData[8 * 8] = 
		{
			// front face							 
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
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
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			3,2,6,6,7,3,
			0,1,5,5,4,0,
			4,0,3,3,7,4,
			1,5,6,6,2,1
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

		s_Data->FlatColorShader = Shader::Create("assets/shader/PureColor3D.glsl");
	}

	void Renderer3D::Shutdown()
	{
		if (s_Data != nullptr)
			delete s_Data;
		s_Data = nullptr;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer3D::BeginSceneModel(const PerspectiveCamera& camera, const Ref<Shader>& modelShader)
	{
		std::dynamic_pointer_cast<OpenGLShader>(modelShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(modelShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(modelShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	//==================================================================

	void Renderer3D::DrawCirle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->CirleVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CirleVertexArray, RendererAPI::DrawType::STRIP);
	}

	void Renderer3D::DrawModel(const Ref<RendererModelStorage> modelData, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(modelData->ModelShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(modelData->ModelShader)->UploadUniformFloat4("u_Color", color);

		for (int i = 0; i < modelData->ModelVertexArray.size();++i) 
		{
			modelData->ModelVertexArray[i]->Bind();
			RenderCommand::DrawIndexed(modelData->ModelVertexArray[i]);
		}
		
	}
}
