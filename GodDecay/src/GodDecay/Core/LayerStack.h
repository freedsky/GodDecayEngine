#pragma once

#include "Base.h"
#include "Layer.h"

/// <summary>
/// 管理Layer的栈容器
/// 
/// 这里一个值得考虑的点，layer对象的OnDetach函数在一定程度上取代了析构，帮助我们更加自由去释放资源
/// 但这也导致释放资源的顺序被打乱，所以OnDetach函数只管理本层对象的统一资源管理，对于自己构造的对象则交给析构去释放
/// </summary>
namespace GodDecay 
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}



