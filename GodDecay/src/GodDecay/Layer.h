#pragma once

#include "Core.h"
#include "Events/Event.h"

/// <summary>
/// Layer是让各种更新依照类型和顺序进行顺序更新
/// PS：Hazel2D版本似乎把这种思想遗弃了？它仅仅是作为外部调用库时继承用
/// </summary>
namespace GodDecay 
{
	class GODDECAY_API Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpDate() {}
		virtual void OnEvents(const Event& e) {}

		//just bebug
		inline const std::string& GetLayerName() const { return m_LayerName; }
	private:
		std::string m_LayerName;
	};
}



