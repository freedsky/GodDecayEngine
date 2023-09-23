#pragma once

#include "RendererAPI.h"


/// <summary>
/// ��RendererAPI��ʹ�ó�����API����ԭʼ��APIת��Ϊ�Լ������APIʹ��
/// </summary>
namespace GodDecay 
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			m_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			m_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray)
		{
			m_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static Ref<RendererAPI> m_RendererAPI;
	};
}



