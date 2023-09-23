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



