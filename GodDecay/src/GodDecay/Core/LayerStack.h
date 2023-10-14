#pragma once

#include "Base.h"
#include "Layer.h"

/// <summary>
/// ����Layer��ջ����
/// 
/// ����һ��ֵ�ÿ��ǵĵ㣬layer�����OnDetach������һ���̶���ȡ�����������������Ǹ�������ȥ�ͷ���Դ
/// ����Ҳ�����ͷ���Դ��˳�򱻴��ң�����OnDetach����ֻ����������ͳһ��Դ���������Լ�����Ķ����򽻸�����ȥ�ͷ�
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

		//���ӷ���ָ������
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
		
		//���ӵ�������ָ��
		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}



