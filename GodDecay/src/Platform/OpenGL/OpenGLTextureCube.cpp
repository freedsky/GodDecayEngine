#include "gdpch.h"
#include "OpenGLTextureCube.h"

#include <glad/glad.h>

#include <stb_image.h>
namespace GodDecay 
{
	OpenGLTextureCube::OpenGLTextureCube(std::vector<std::string> paths)
		:m_Paths(paths)
	{
		int width, height, channels;
		GLenum internalFormat = 0, dataFormat = 0;
		//stbi_set_flip_vertically_on_load(true);
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		//ѭ��ȥ�����������6���������[��Ϊһ���Լ�����6��û�о���ѹ�������������ڴ�ռ�û�ܴ�]
		for (int i = 0; i < m_Paths.size(); ++i) 
		{
			
			unsigned char* data = stbi_load(m_Paths[i].c_str(), &width, &height, &channels, 0);
			if (data) 
			{
				m_IsLoaded = true;
				 
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

				//��Դͼ�����Ϊ��������
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << m_Paths[i] << std::endl;
				stbi_image_free(data);
			}
		}
		//���û��ƺ͹���ģʽ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_Width = width;
		m_Height = height;
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;
		//��Ϊ6����ͼ������պе�textue��������ͳһΪSkyBox
		m_TextureName = "SkyBox";

		//���
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	OpenGLTextureCube::OpenGLTextureCube(uint32_t width, uint32_t height, bool HDR)
		:m_Width(width), m_Height(height), m_Channels(4)
	{
		if (HDR) 
		{
			m_InternalFormat = GL_RGB16F;
			m_DataFormat = GL_RGB;
		}
		else
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		for (GLuint i = 0; i < 6; i++)
		{
			if(HDR)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_FLOAT, nullptr);
			else
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (HDR)
			m_TextureName = "HDRSkyBox";
		else
			m_TextureName = "SkyBox";
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCube::SetData(void* data, uint32_t size)
	{
		//��ȷ���в���...����֤
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		GD_ENGINE_ASSERT(size == m_Width * m_Height * bpp, "ata must be entire texture!");
		//��Ȼ�Ǵ�ɫbox������һ�����ݼ���
		for (uint32_t i = 0; i < 6; ++i) 
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		}
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		//glBindTextureUnit(slot, m_RendererID);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLTextureCube::DeleteTextureMemory() const
	{
		glDeleteBuffers(1, &m_RendererID);
	}
}
