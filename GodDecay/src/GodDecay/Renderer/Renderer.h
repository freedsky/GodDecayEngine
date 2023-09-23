#pragma once
/// <summary>
/// 在该类中选择RendererAPI是对高层API的init抽象
/// </summary>
namespace GodDecay 
{
	enum class RendererAPI 
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return m_RenderAPI; }
	private:
		static RendererAPI m_RenderAPI;
	};
}



