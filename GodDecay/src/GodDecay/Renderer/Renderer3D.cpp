#include "gdpch.h"
#include "Renderer3D.h"

#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "MeshRenderer.h"

#include "Shader.h"
#include "Texture.h"

#include <glm/gtx/transform.hpp>

namespace GodDecay 
{
	//批处理的顶点属性结构
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
 		glm::vec4 Color;
		float TexIndex;
		float TilingFactor;
		int EntityID;
	};
	//矩形
	struct Renderer3DStorage
	{
		//最大限制绘制数[批处理]//这里要对模型的mesh的类型进行处理
		static const uint32_t MaxMeshes = 100;
		static const uint32_t MaxTextureSlots = 32;
		static const uint32_t MaxCubeVertices = MaxMeshes * 36;
		static const uint32_t MaxCubeIndices = MaxMeshes * 36;	

		Ref<VertexArrayBuffer> MeshCubeVertexArray;
		Ref<VertexBuffer> MeshCubeVertexBuffer;

		Ref<Shader> BaseShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t MeshCubeIndexCount = 0;

		//进行偏移处理
		MeshVertex* MeshCubeVertexBufferBase = nullptr;
		MeshVertex* MeshCubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;//默认的纹理下标为白色纹理，其纹理下标为0

		glm::vec4 MeshCubeVertexPositions[36];

		Renderer3D::Statistics3D Stats;
	};
	Renderer3DStorage s_MeshData;

	void Renderer3D::Init()
	{
		//加载vbo和ebo数据
		LoadMesh();
		//对顶点缓冲大小进行初始化
		s_MeshData.MeshCubeVertexBufferBase = new MeshVertex[s_MeshData.MaxCubeVertices];

		//设置默认的Texture和Shader
		int32_t samplers[s_MeshData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_MeshData.MaxTextureSlots; ++i)
			samplers[i] = i;

		s_MeshData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_MeshData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_MeshData.BaseShader = Shader::Create("assets/shader/Standard3DShader.glsl");
		s_MeshData.BaseShader->Bind();

		s_MeshData.BaseShader->SetIntArray("u_Textures", samplers, s_MeshData.MaxTextureSlots);
		s_MeshData.TextureSlots[0] = s_MeshData.WhiteTexture;
	}

	void Renderer3D::LoadMesh()
	{
		s_MeshData.MeshCubeVertexArray = VertexArrayBuffer::Create();
		s_MeshData.MeshCubeVertexBuffer = VertexBuffer::Create(s_MeshData.MaxCubeIndices * sizeof(MeshVertex));
		s_MeshData.MeshCubeVertexBuffer->Setlayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float2, "a_Texcoords"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float,	 "a_TexIndex"},
			{ShaderDataType::Float,  "a_TilingFactor"},
			{ShaderDataType::Int,    "a_EntityID"    }
			});
		s_MeshData.MeshCubeVertexArray->AddVertexBuffer(s_MeshData.MeshCubeVertexBuffer);

		//顶点索引
		uint32_t* cubemeshIndex = new uint32_t[s_MeshData.MaxCubeIndices];
		uint32_t offset = 0;
		uint32_t IndexOffset = 36;
		for (uint32_t i = 0; i < s_MeshData.MaxCubeIndices; i += 36)
		{
			for (int j = 0; j < IndexOffset; ++j)
				cubemeshIndex[i + j] = offset + j;
			offset += 36;
		}
		Ref<IndexBuffer> cubemeshIBO = IndexBuffer::Create(cubemeshIndex, s_MeshData.MaxCubeIndices);
		s_MeshData.MeshCubeVertexArray->SetIndexBuffer(cubemeshIBO);
		delete[] cubemeshIndex; cubemeshIndex = nullptr;

		//初始化位置向量
		// back face
		s_MeshData.MeshCubeVertexPositions[0] = { -1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[1] = { 1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[2] = { 1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[3] = { 1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[4] = { -1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[5] = { -1.0f,  1.0f, -1.0f, 1.0f };
		// front face
		s_MeshData.MeshCubeVertexPositions[6] = { -1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[7] = { 1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[8] = { 1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[9] = { 1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[10] = { -1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[11] = { -1.0f, -1.0f,  1.0f, 1.0f };
		// left face
		s_MeshData.MeshCubeVertexPositions[12] = { -1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[13] = { -1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[14] = { -1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[15] = { -1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[16] = { -1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[17] = { -1.0f,  1.0f,  1.0f, 1.0f };
		// right face
		s_MeshData.MeshCubeVertexPositions[18] = { 1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[19] = { 1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[20] = { 1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[21] = { 1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[22] = { 1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[23] = { 1.0f, -1.0f,  1.0f, 1.0f };
		// bottom face
		s_MeshData.MeshCubeVertexPositions[24] = { -1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[25] = { 1.0f, -1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[26] = { 1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[27] = { 1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[28] = { -1.0f, -1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[29] = { -1.0f, -1.0f, -1.0f, 1.0f };
		// top face
		s_MeshData.MeshCubeVertexPositions[30] = { -1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[31] = { 1.0f,  1.0f , 1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[32] = { 1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[33] = { 1.0f,  1.0f,  1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[34] = { -1.0f,  1.0f, -1.0f, 1.0f };
		s_MeshData.MeshCubeVertexPositions[35] = { -1.0f,  1.0f,  1.0f, 1.0f };
	}


	void Renderer3D::Shutdown()
	{
		delete s_MeshData.MeshCubeVertexBufferBase;
	}

	void Renderer3D::BeginScene(const OrthographicCamera& camera)
	{
		s_MeshData.BaseShader->Bind();
		s_MeshData.BaseShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_MeshData.MeshCubeIndexCount = 0;
		s_MeshData.MeshCubeVertexBufferPtr = s_MeshData.MeshCubeVertexBufferBase;

		s_MeshData.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_MeshData.BaseShader->Bind();
		s_MeshData.BaseShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_MeshData.MeshCubeIndexCount = 0;
		s_MeshData.MeshCubeVertexBufferPtr = s_MeshData.MeshCubeVertexBufferBase;

		s_MeshData.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_MeshData.BaseShader->Bind();
		s_MeshData.BaseShader->SetMat4("u_ViewProjection", viewProj);

		s_MeshData.MeshCubeIndexCount = 0;
		s_MeshData.MeshCubeVertexBufferPtr = s_MeshData.MeshCubeVertexBufferBase;

		s_MeshData.TextureSlotIndex = 1;
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		s_MeshData.BaseShader->Bind();
		s_MeshData.BaseShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		s_MeshData.MeshCubeIndexCount = 0;
		s_MeshData.MeshCubeVertexBufferPtr = s_MeshData.MeshCubeVertexBufferBase;

		s_MeshData.TextureSlotIndex = 1;
	}

	void Renderer3D::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_MeshData.MeshCubeVertexBufferPtr - (uint8_t*)s_MeshData.MeshCubeVertexBufferBase);
		s_MeshData.MeshCubeVertexBuffer->SetData(s_MeshData.MeshCubeVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer3D::Flush()
	{
		if (s_MeshData.MeshCubeIndexCount == 0)
			return;

		for (uint32_t i = 0; i < s_MeshData.TextureSlotIndex; ++i)
			s_MeshData.TextureSlots[i]->Bind(i);

		s_MeshData.MeshCubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_MeshData.MeshCubeVertexArray, RendererAPI::DrawType::Normal, s_MeshData.MeshCubeIndexCount);
		
		s_MeshData.Stats.DrawCalls++;
	}

	void Renderer3D::StartBatch()
	{
		s_MeshData.MeshCubeIndexCount = 0;
		s_MeshData.MeshCubeVertexBufferPtr = s_MeshData.MeshCubeVertexBufferBase;

		s_MeshData.TextureSlotIndex = 1;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer3D::DrawCubeMesh(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		if (s_MeshData.MeshCubeIndexCount >= Renderer3DStorage::MaxCubeIndices)
			NextBatch();
		constexpr size_t meshVertexCount = 36;
		const float texIndex = 0.0f;//设置默认纹理
		const float tilingFactor = 1.0f;
		constexpr glm::vec2 textureCoords[] = 
		{ 
			{ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f }, { 0.0f, 1.0f },
			{ 0.0f, 0.0f } ,{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f }, { 0.0f, 0.0f },

			{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f },	{ 0.0f, 0.0f }, { 1.0f, 0.0f },
			{ 1.0f, 0.0f } ,{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },	{ 1.0f, 0.0f }, { 0.0f, 0.0f },

			{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f },	{ 0.0f, 0.0f }, { 0.0f, 1.0f },
			{ 0.0f, 1.0f } ,{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },	{ 0.0f, 1.0f }, { 0.0f, 0.0f }
		};

		constexpr glm::vec3 normal[] =
		{
			{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},
			{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},
			{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},
			{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},
			{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},
			{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f}
		};

		for (int i = 0; i < meshVertexCount; ++i)
		{
			s_MeshData.MeshCubeVertexBufferPtr->Position = transform * s_MeshData.MeshCubeVertexPositions[i];
			s_MeshData.MeshCubeVertexBufferPtr->Normal = normal[i];
			s_MeshData.MeshCubeVertexBufferPtr->Color = color;
			s_MeshData.MeshCubeVertexBufferPtr->TexCoord = textureCoords[i];
			s_MeshData.MeshCubeVertexBufferPtr->TexIndex = texIndex;
			s_MeshData.MeshCubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_MeshData.MeshCubeVertexBufferPtr->EntityID = entityID;
			s_MeshData.MeshCubeVertexBufferPtr++;
		}
		s_MeshData.MeshCubeIndexCount += 36;
		s_MeshData.Stats.CubeCount++;
	}

	void Renderer3D::DrawCubeMesh(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		if (s_MeshData.MeshCubeIndexCount >= Renderer3DStorage::MaxCubeIndices)
			NextBatch();

		glm::vec4 color = tintColor;

		float textureIndex = 0;
		for (uint32_t i = 1; i < s_MeshData.TextureSlotIndex; ++i)
		{
			if (*s_MeshData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			//如果它超过了纹理的上限
			if (s_MeshData.TextureSlotIndex >= Renderer3DStorage::MaxTextureSlots)
				Flush();

			textureIndex = (float)s_MeshData.TextureSlotIndex;
			s_MeshData.TextureSlots[s_MeshData.TextureSlotIndex] = texture;
			s_MeshData.TextureSlotIndex++;
		}

		constexpr size_t meshVertexCount = 36;
		constexpr glm::vec2 textureCoords[] =
		{
			{ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f }, { 0.0f, 1.0f },
			{ 0.0f, 0.0f } ,{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f }, { 0.0f, 0.0f },

			{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f },	{ 0.0f, 0.0f }, { 1.0f, 0.0f },
			{ 1.0f, 0.0f } ,{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },	{ 1.0f, 0.0f }, { 0.0f, 0.0f },

			{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f },	{ 0.0f, 0.0f }, { 0.0f, 1.0f },
			{ 0.0f, 1.0f } ,{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },	{ 0.0f, 1.0f }, { 0.0f, 0.0f }
			
		};
		constexpr glm::vec3 normal[] =
		{
			{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},{ 0.0f,  0.0f, -1.0f},
			{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},{ 0.0f,  0.0f,  1.0f},
			{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},{-1.0f,  0.0f,  0.0f},
			{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},{ 1.0f,  0.0f,  0.0f},
			{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},{ 0.0f, -1.0f,  0.0f},
			{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f},{ 0.0f,  1.0f,  0.0f}
		};

		for (int i = 0; i < meshVertexCount; ++i)
		{
			s_MeshData.MeshCubeVertexBufferPtr->Position = transform * s_MeshData.MeshCubeVertexPositions[i];
			s_MeshData.MeshCubeVertexBufferPtr->Normal = normal[i];
			s_MeshData.MeshCubeVertexBufferPtr->Color = color;
			s_MeshData.MeshCubeVertexBufferPtr->TexCoord = textureCoords[i];
			s_MeshData.MeshCubeVertexBufferPtr->TexIndex = textureIndex;
			s_MeshData.MeshCubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_MeshData.MeshCubeVertexBufferPtr->EntityID = entityID;
			s_MeshData.MeshCubeVertexBufferPtr++;
		}
		s_MeshData.MeshCubeIndexCount += 36;
		s_MeshData.Stats.CubeCount++;
	}

	void Renderer3D::DrawCubeMeshRenderer(const glm::mat4& transform, MeshRenderComponent& src, int entityID)
	{
		/*if (src.Texture)
		{
			DrawCubeMesh(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		}
		else
		{
			DrawCubeMesh(transform, src.Color, entityID);

		}*/
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_MeshData.Stats, 0, sizeof(Statistics3D));
	}

	GodDecay::Renderer3D::Statistics3D Renderer3D::GetStats()
	{
		return s_MeshData.Stats;
	}

	

}