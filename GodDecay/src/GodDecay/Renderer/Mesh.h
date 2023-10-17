#pragma once

#include "GodDecay/Core/Base.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

/// <summary>
/// ģ�����ݴ洢
/// ģ�͵�����Ӧ���Ǹ߶��Զ���ģ���ȥ�ڼ���ģ��ʱȥ������������ͨ���ֶ���ӵķ�ʽ
/// </summary>
namespace GodDecay 
{
	struct ModelData 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoords;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<ModelData> modelData, std::vector<uint32_t> indices);
		~Mesh() = default;

		inline const std::vector<ModelData>& GetModelData() const { return m_Data; }
		inline const std::vector<uint32_t>& GetModelIndices() const { return m_indices; }

	private:
		std::vector<ModelData> m_Data;
		std::vector<uint32_t> m_indices;
	};
}



