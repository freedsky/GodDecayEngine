#pragma once
#include "GodDecay/Renderer/Texture.h"

#include <glad/glad.h>

/// <summary>
/// 这里好像只适配了天空盒的构造数据和方式
/// 后面会进行修改为更加通用
/// 
/// warring： 在设置天空盒时好像并未使用到Cube的setdata函数，而是直接在构造函数中去绑定
/// </summary>

namespace GodDecay 
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(std::vector<std::string> paths);
		//设置纯色天空盒
		OpenGLTextureCube(uint32_t width, uint32_t height, bool HDR, bool mipmap);
		~OpenGLTextureCube();

		virtual uint32_t GetWidth() const override { return m_Width; };

		virtual uint32_t GetHeight() const override { return m_Height; };

		virtual uint32_t GetChannel() const override { return m_Channels; };

		virtual uint32_t GetRendererID() const override { return m_RendererID; };

		virtual std::string GetTextureName() const override { return m_TextureName; };

		virtual void SetData(void* data, uint32_t size, bool depth) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void DeleteTextureMemory() const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; };

		virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTextureCube&)other).m_RendererID; };
	private:
		uint32_t m_RendererID;
		//因为是立方体贴图所以数据应该是6个面的数据
		std::vector<std::string> m_Paths;
		std::string m_TextureName;//默认为SkyBox
		bool m_IsLoaded = false;
		//基本上一个天空盒的图像格式是一致的，所以只存储一个即可
		uint32_t m_Width, m_Height, m_Channels;
		GLenum m_InternalFormat, m_DataFormat;
	};
}



