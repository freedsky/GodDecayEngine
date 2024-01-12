#include "gdpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include <stb_image.h>

namespace GodDecay 
{
	OpenGLTexture::OpenGLTexture(const std::string& path, bool HDR)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		float* dataHDR = nullptr;
		stbi_uc* data = nullptr;
		if (HDR)
			dataHDR = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		else
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data || dataHDR)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				if (HDR) 
				{
					internalFormat = GL_RGBA16F;
					dataFormat = GL_RGBA;
				}
				else
				{
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
				}
			}
			else if (channels == 3)
			{
				if (HDR)
				{
					internalFormat = GL_RGB16F;
					dataFormat = GL_RGB;
				}
				else
				{
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
				}
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			GD_ENGINE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			if (HDR) 
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			if (HDR)
			{
				glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT, dataHDR);
			}
			else
			{
				glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			}
			
			if (HDR)
				stbi_image_free(dataHDR);
			else
				stbi_image_free(data);

			//截取文件名称
			auto lastSlash = path.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = path.rfind('.');
			auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
			m_TextureName = path.substr(lastSlash, count);
		}
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height), m_Channels(4)
	{
		//自己创建的Texture是没有文件名称的，这里置为空的标识符
		m_TextureName = "";

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::OpenGLTexture(std::string name, uint32_t textureID, uint32_t width, uint32_t height)
	{
		m_RendererID = textureID;
		m_Width = width;
		m_Height = height;
		//0表示没有数据
		m_Channels = 0;
		m_DataFormat = 0;
		m_InternalFormat = 0;
		m_IsLoaded = true;
		m_Path = "";
		m_TextureName = name;
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		GD_ENGINE_ASSERT(size == m_Width * m_Height * bpp, "ata must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture::DeleteTextureMemory() const
	{
		glDeleteBuffers(1, &m_RendererID);
	}
}



