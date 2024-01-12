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
		inline static void Init() 
		{
			m_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			m_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			m_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			m_RendererAPI->Clear();
		}

		inline static void ClearDepth() 
		{
			m_RendererAPI->ClearDepth();
		}

		inline static void SetDepthTest(bool flag) 
		{
			m_RendererAPI->SetDepthTest(flag);
		}

		inline static void SetDepthMask(bool flag) 
		{
			m_RendererAPI->SetDepthMask(flag);
		}

		inline static void SetDepthFunc(RendererAPI::DepthFunType type)
		{
			m_RendererAPI->SetDepthFunc(type);
		}

		inline static void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, RendererAPI::DrawType type = RendererAPI::DrawType::Normal, uint32_t indexCount = 0)
		{
			m_RendererAPI->DrawIndexed(vertexArray, type, indexCount);
		}
	private:
		static Ref<RendererAPI> m_RendererAPI;
	};
}



