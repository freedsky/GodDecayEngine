#pragma once

#include <glm/glm.hpp>

#include "GodDecay/Core.h"
#include "VertexArrayBuffer.h"

/// <summary>
/// 把平台API进行抽象
/// </summary>
namespace GodDecay 
{
	class RendererAPI
	{
	public:
		enum class API 
		{
			None = 0,
			OpenGL = 1,
			Vulkan = 2
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray) = 0;

		inline static API GetAPI() { return m_RednererAPI; }
	private:
		static API m_RednererAPI;
	};
}



