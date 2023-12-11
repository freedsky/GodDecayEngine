#pragma once
#include <entt.hpp>

#include "GodDecay/Tools/TimeTool.h"
#include "GodDecay/Renderer/EditorCamera.h"

/// <summary>
/// 多个GameObject组成场景，
/// 而场景中包含许多个实体[GameObject]
/// Entity中又有许多Components
/// 而此类就是管理Scene在每个entity和components
/// </summary>

namespace GodDecay 
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(float deltaTime);
		void UpdateEditor(float deltaTime, EditorCamera& camera);
		
		void OnViewportResize(uint32_t width, uint32_t height);
		//返回当前激活相机的引用
		Entity GetPrimaryCameraEntity();
	private:
		//在添加组件时，在这之前对其进行必要的修改[比如相机的viewport的修改]
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		//在删除组件时，在这之前对其进行执行
		template<typename T>
		void OnComponentRemove(Entity entity, T& component);
	private:
		//注册表，它的作用相当于存储了entity[实际上就只有一个ID]以及component组件数据
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}



