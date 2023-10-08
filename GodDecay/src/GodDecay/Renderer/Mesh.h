#pragma once

#include "GodDecay/Core.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

/// <summary>
/// 模型数据存储
/// </summary>
namespace GodDecay 
{
	struct ModelData 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoords;
	};
	struct ModelTexture 
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<ModelData> modelData, std::vector<uint32_t> indices);
		~Mesh() = default;

		inline const std::vector<ModelData>& GetModelData() const { return m_Data; }
		inline const std::vector<uint32_t>& GetModelIndices() const { return m_indices; }
		inline const std::vector<ModelTexture>& GetModelTexture() const { return m_Texture; }

	private:
		std::vector<ModelData> m_Data;
		std::vector<uint32_t> m_indices;
		std::vector<ModelTexture> m_Texture;
	};
}



