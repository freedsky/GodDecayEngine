#pragma once

#include "RendererAPI.h"


/// <summary>
/// 从RendererAPI中使用抽象后的API来把原始的API转换为自己引擎的API使用
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

		inline static void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, RendererAPI::DrawType type = RendererAPI::DrawType::Normal)
		{
			m_RendererAPI->DrawIndexed(vertexArray, type);
		}
	private:
		static Ref<RendererAPI> m_RendererAPI;
	};
}



