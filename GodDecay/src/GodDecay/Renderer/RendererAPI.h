#pragma once

#include <glm/glm.hpp>

#include "GodDecay/Core/Base.h"
#include "VertexArrayBuffer.h"

/// <summary>
/// ��ƽ̨API���г���
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
		//�ò�ͬ�Ķ���Ӧ�÷�������������
		enum class DrawType 
		{
			None = 0,
			Normal = 1,
			STRIP = 2
		};
		//������Ȳ��ԵĲ�ͬ���Է���
		enum class DepthFunType
		{
			ALWAYS = 0,
			NEVER,
			LESS,
			EQUAL,
			LEQUAL,
			GREATER,
			NOTEQUAL,
			GEQUAL
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void ClearDepth() = 0;
		virtual void SetDepthTest(bool flag) = 0;
		virtual void SetDepthMask(bool flag) = 0;
		virtual void SetDepthFunc(DepthFunType type) = 0;
		virtual void SetCubeMapFiltration() = 0;

		virtual void DrawIndexed(const Ref<VertexArrayBuffer>& vertexArray, DrawType type = DrawType::Normal, uint32_t indexCount = 0) = 0;

		inline static API GetAPI() { return m_RednererAPI; }
	private:
		static API m_RednererAPI;
	};
}



