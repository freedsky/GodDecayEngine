#include "gdpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

#include <stb_image.h>

namespace GodDecay 
{
	OpenGLTexture::OpenGLTexture(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);//OpenGL��������y���෴�ģ���������˷�ת
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		GD_ENGINE_ASSERT(data, "Failed to Load Image");
		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		GD_ENGINE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		//���������������Լ�����������ģʽ
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//����������������ڷŴ����Сʱ����Ӧ�õĲ�ֵ��ʽ
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height), m_Channels(4)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	uint32_t OpenGLTexture::LoadModelFileTexture(const char* path, const std::string& directory)
	{
		std::string filename = std::string(path);
		filename = directory + "/" + filename;

		uint32_t id;
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum internalFormat = 0, dataFormat = 0;
			if (nrComponents == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (nrComponents == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (nrComponents == 1)
			{
				internalFormat = GL_RED;
				dataFormat = GL_RED;
			}
			GD_ENGINE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			//���������������Լ�����������ģʽ
			glCreateTextures(GL_TEXTURE_2D, 1, &id);
			glTextureStorage2D(id, 1, internalFormat, width, height);

			//����������������ڷŴ����Сʱ����Ӧ�õĲ�ֵ��ʽ
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			GD_ENGINE_ERROR("Texture failed to load at path : ");
			stbi_image_free(data);
		}
		return id;
	}
}



