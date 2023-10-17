#pragma once

#include "Base.h"
#include "GodDecay/Events/Event.h"
#include "Platform/PlatformTools/OpenGLTimeTool.h"

/// <summary>
/// Layer是让各种更新依照类型和顺序进行顺序更新
/// PS：Hazel2D版本似乎把这种思想遗弃了？它仅仅是作为外部调用库时继承用
/// 
/// Layer层是让自己的不同系统建立在Layer栈容器管理之上的，通过继承来的四个方法以栈的方式和顺序进行更新[对PS的更正]
/// </summary>
namespace GodDecay 
{
	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpDate(float deltaTime) {}
		virtual void OnEvents(Event& e) {}
		virtual void OnImGuiRender() {}

		//just bebug
		inline const std::string& GetLayerName() const { return m_LayerName; }
	private:
		std::string m_LayerName;
	};
}



