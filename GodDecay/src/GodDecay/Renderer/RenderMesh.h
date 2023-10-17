#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Renderer/Shader.h"
#include "GodDecay/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace GodDecay 
{
	struct VertexProperties 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		VertexProperties(glm::vec3 position, glm::vec3 normal, glm::vec4 color, glm::vec2 texcoord)
			:Position(position), Normal(normal), Color(color), TexCoord(texcoord)
		{}
	};

	//最多为一个物体支持32个纹理
	struct TextureProperties 
	{
		std::vector<uint32_t> TextureID;
		std::vector<Texture2D> Textures;
		std::vector<std::string> TextureName;
	};

	class RenderMesh
	{
	public:
		RenderMesh() = default;
		~RenderMesh() = default;

		inline std::vector<VertexProperties>& GetVertexProperties() { return m_VertexProperties; }
		inline TextureProperties& GetTexturePropertices() { return m_TexturePropertices; }
		inline std::vector<uint32_t>& GetIndices() { return m_Indices; }

	private:
		std::vector<VertexProperties> m_VertexProperties;
		TextureProperties m_TexturePropertices;
		std::vector<uint32_t> m_Indices;

		virtual glm::vec3 GetTangent(glm::vec3 position1, glm::vec3 position2, glm::vec3 position3, glm::vec2 texcoord1, glm::vec2 texcoord2, glm::vec2 texcoord3) = 0;
	};

	class RenderCubeMesh : public RenderMesh 
	{
	public:
		RenderCubeMesh();
		~RenderCubeMesh();

	private:
		const uint32_t MaxTextures = 16;

		void Init();
		virtual glm::vec3 GetTangent(glm::vec3 position1, glm::vec3 position2, glm::vec3 position3, glm::vec2 texcoord1, glm::vec2 texcoord2, glm::vec2 texcoord3) override;
	};

	class RenderCirleMesh : public RenderMesh
	{
	public:
		RenderCirleMesh();
		~RenderCirleMesh();

	private:
		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359f;

		void Init();
		virtual glm::vec3 GetTangent(glm::vec3 position1, glm::vec3 position2, glm::vec3 position3, glm::vec2 texcoord1, glm::vec2 texcoord2, glm::vec2 texcoord3) override;
	};
}



