#include "gdpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GodDecay 
{
	void OpenGLRendererAPI::Init()
	{
		//������Ȳ���
		glEnable(GL_DEPTH_TEST);
		//����͸���Ȳ���
		glEnable(GL_BLEND);
		//����͸���Ȳ��Ե�����
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

	void OpenGLRendererAPI::SetDepthMask(bool flag)
	{
		if (flag)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);
	}

	void OpenGLRendererAPI::SetDepthFunc(DepthFunType type)
	{
		switch (type) 
		{
		case DepthFunType::ALWAYS:
			glDepthFunc(GL_ALWAYS);
			break;
		case DepthFunType::NEVER:
			glDepthFunc(GL_NEVER);
			break;
		case DepthFunType::LESS:
			glDepthFunc(GL_LESS);
			break;
		case DepthFunType::EQUAL:
			glDepthFunc(GL_EQUAL);
			break;
		case DepthFunType::LEQUAL:
			glDepthFunc(GL_LEQUAL);
			break;
		case DepthFunType::GREATER:
			glDepthFunc(GL_GREATER);
			break;
		case DepthFunType::NOTEQUAL:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case DepthFunType::GEQUAL:
			glDepthFunc(GL_GEQUAL);
			break;
		default:
			GD_ENGINE_WARN("Sorry Not this DepthFuncType.");
			break;
		}
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, DrawType type, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		if (type == DrawType::STRIP)
			glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, nullptr);
		else if (type == DrawType::Normal)
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		else if (type == DrawType::None)
			GD_ENGINE_ERROR("UnKnow this DrawType");

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


