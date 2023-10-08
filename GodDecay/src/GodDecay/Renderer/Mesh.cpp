#include "gdpch.h"
#include "Mesh.h"

namespace GodDecay 
{
	Mesh::Mesh(std::vector<ModelData> modelData, std::vector<uint32_t> indices)
	{
		this->m_Data = modelData;
		this->m_indices = indices;
	}

}