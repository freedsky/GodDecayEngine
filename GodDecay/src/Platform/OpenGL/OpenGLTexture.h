#pragma once

#include "GodDecay/Renderer/Texture.h"

#include <glad/glad.h>
/// <summary>
/// 在OpenGl平台实现纹理的加载
/// 
/// 对于不同类型的纹理加载应该编写相应的通用接口[而不是用bool来做区分]，比如framebuffer的通用帧缓冲接口
/// </summary> 
namespace GodDecay 
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path, bool HDR);
		OpenGLTexture(uint32_t width, uint32_t height, bool depth);
		OpenGLTexture(std::string name, uint32_t textureID, uint32_t width, uint32_t height);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetChannel() const override { return m_Channels; }

		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual std::string GetTextureName() const override {return m_TextureName;}

		virtual void SetData(void* data, uint32_t size, bool depth) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void DeleteTextureMemory() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		std::string m_TextureName;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height, m_Channels;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}



