#pragma once

#include "Core.h"
#include "Events/Event.h"

/// <summary>
/// Layer���ø��ָ����������ͺ�˳�����˳�����
/// PS��Hazel2D�汾�ƺ�������˼�������ˣ�����������Ϊ�ⲿ���ÿ�ʱ�̳���
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



