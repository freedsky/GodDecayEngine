#pragma once
#include <entt.hpp>

#include "GodDecay/Tools/TimeTool.h"
#include "GodDecay/Renderer/EditorCamera.h"

/// <summary>
/// ���GameObject��ɳ�����
/// �������а�������ʵ��[GameObject]
/// Entity���������Components
/// ��������ǹ���Scene��ÿ��entity��components
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
		//���ص�ǰ�������������
		Entity GetPrimaryCameraEntity();
	private:
		//��������ʱ������֮ǰ������б�Ҫ���޸�[���������viewport���޸�]
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		//��ɾ�����ʱ������֮ǰ�������ִ��
		template<typename T>
		void OnComponentRemove(Entity entity, T& component);
	private:
		//ע������������൱�ڴ洢��entity[ʵ���Ͼ�ֻ��һ��ID]�Լ�component�������
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}



