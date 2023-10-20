#pragma once
#include "GodDecay/Core/Base.h"
#include "GodDecay/Core/Logger.h"

#include <entt.hpp>

#include "Scene.h"

/// <summary>
/// 用于管理entity，对其进行添加，删除或者判断它是否存在等等
/// </summary>
namespace GodDecay 
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene * scene);
		Entity(const Entity & other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			GD_ENGINE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			GD_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			GD_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }

		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}



