#include "gdpch.h"
#include "Mesh.h"

namespace GodDecay 
{
	Mesh::Mesh(std::vector<ModelData> modelData, std::vector<uint32_t> indices)
	{
		m_Data = modelData;
		m_indices = indices;
	}

}