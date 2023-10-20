#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Scene/Entity.h"
#include "GodDecay/Scene/Scene.h"

/// <summary>
/// �༭UI������
/// </summary>
namespace GodDecay
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		//��Ⱦ��������
		void OnImGuiRender();
	private:
		//���Ƴ���ʵ����������
		void DrawEntityNode(Entity entity);
		//����[��ѡ��]ʵ�������������
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}

