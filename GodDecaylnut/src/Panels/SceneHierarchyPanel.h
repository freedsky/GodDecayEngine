#pragma once

#include "GodDecay/Core/Base.h"
#include "GodDecay/Scene/Entity.h"
#include "GodDecay/Scene/Scene.h"

/// <summary>
/// 编辑UI面板绘制
/// </summary>
namespace GodDecay
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		//渲染面板的内容
		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		//绘制场景实体的面板内容
		void DrawEntityNode(Entity entity);
		//绘制[被选中]实体的属性面板绘制
		void DrawComponents(Entity entity);
		//添加组件<利用泛型>
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entityName);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}

