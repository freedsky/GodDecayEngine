#pragma once

#include "GodDecay/Renderer/Texture.h"

/// <summary>
/// ��OpenGlƽ̨ʵ������ļ���
/// </summary> 
namespace GodDecay 
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetChannel() const override { return m_Channels; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height, m_Channels;
		uint32_t m_RendererID;
	};
}



