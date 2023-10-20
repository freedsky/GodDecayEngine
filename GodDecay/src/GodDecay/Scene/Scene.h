#pragma once

#include <entt.hpp>

#include "GodDecay/Tools/TimeTool.h"

/// <summary>
/// ���GameObject��ɳ�����
/// �������а�������ʵ��[GameObject]
/// Entity���������Components
/// ��������ǹ���Scene��ÿ��entity��components
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
		//ע������������൱�ڴ洢��entity[ʵ���Ͼ�ֻ��һ��ID]�Լ�component�������
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}



