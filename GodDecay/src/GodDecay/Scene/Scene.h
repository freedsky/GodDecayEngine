#pragma once

#include <entt.hpp>

#include "GodDecay/Tools/TimeTool.h"

/// <summary>
/// 多个GameObject组成场景，
/// 而场景中包含许多个实体[GameObject]
/// Entity中又有许多Components
/// 而此类就是管理Scene在每个entity和components
/// </summary>

namespace GodDecay 
{
	class SceneHierarchyPanel;
	class Entity;
	
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdata(float deltaTime);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		//注册表，它的作用相当于存储了entity[实际上就只有一个ID]以及component组件数据
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}



