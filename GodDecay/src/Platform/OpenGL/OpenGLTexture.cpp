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

		//���������������Լ�����������ģʽ
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//����������������ڷŴ����Сʱ����Ӧ�õĲ�ֵ��ʽ
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}



