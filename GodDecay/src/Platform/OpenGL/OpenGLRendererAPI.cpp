#include "gdpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GodDecay 
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_DEPTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, DrawType type)
	{
		if (type == DrawType::STRIP)
			glDrawElements(GL_TRIANGLE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		else if (type == DrawType::Normal)
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		else if (type == DrawType::None)
			GD_ENGINE_ERROR("UnKnow this DrawType");
	}
}


