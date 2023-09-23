#pragma once
/// <summary>
/// �ڸ�����ѡ��RendererAPI�ǶԸ߲�API��init����
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



