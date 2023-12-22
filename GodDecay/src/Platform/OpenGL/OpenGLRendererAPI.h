#pragma once

#include "GodDecay/Renderer/RendererAPI.h"

/// <summary>
/// 对OpenGL API的具体实现
/// </summary>
namespace GodDecay 
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void SetDepthMask(bool flag);
		virtual void SetDepthFunc(DepthFunType type);

		virtual void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, DrawType type = DrawType::Normal, uint32_t indexCount = 0) override;
	};
}



