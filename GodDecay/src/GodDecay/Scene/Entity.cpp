#include "gdpch.h"
#include "Entity.h"

namespace GodDecay 
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}
}
