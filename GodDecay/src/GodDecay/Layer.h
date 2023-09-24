#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Platform/PlatformTools/OpenGLTimeTool.h"

/// <summary>
/// Layer���ø��ָ����������ͺ�˳�����˳�����
/// PS��Hazel2D�汾�ƺ�������˼�������ˣ�����������Ϊ�ⲿ���ÿ�ʱ�̳���
/// 
/// Layer�������Լ��Ĳ�ͬϵͳ������Layerջ��������֮�ϵģ�ͨ���̳������ĸ�������ջ�ķ�ʽ��˳����и���[��PS�ĸ���]
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
		virtual void OnEvents(const Event& e) {}
		virtual void OnImGuiRender() {}

		//just bebug
		inline const std::string& GetLayerName() const { return m_LayerName; }
	private:
		std::string m_LayerName;
	};
}



