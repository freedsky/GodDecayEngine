#pragma once

#include "GodDecay/Renderer/RendererAPI.h"

/// <summary>
/// ��OpenGL API�ľ���ʵ��
/// </summary>
namespace GodDecay 
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray) override;
	};
}



