#pragma once
#include "GodDecay/Renderer/Texture.h"

#include <glad/glad.h>

/// <summary>
/// �������ֻ��������պеĹ������ݺͷ�ʽ
/// ���������޸�Ϊ����ͨ��
/// 
/// warring�� ��������պ�ʱ����δʹ�õ�Cube��setdata����������ֱ���ڹ��캯����ȥ��
/// </summary>

namespace GodDecay 
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(std::vector<std::string> paths);
		//���ô�ɫ��պ�
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
		//��Ϊ����������ͼ��������Ӧ����6���������
		std::vector<std::string> m_Paths;
		std::string m_TextureName;//Ĭ��ΪSkyBox
		bool m_IsLoaded = false;
		//������һ����պе�ͼ���ʽ��һ�µģ�����ֻ�洢һ������
		uint32_t m_Width, m_Height, m_Channels;
		GLenum m_InternalFormat, m_DataFormat;
	};
}



