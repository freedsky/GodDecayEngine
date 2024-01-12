#include "gdpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

//���ڴ���֡�������Ĺ���
namespace GodDecay::Utils
{
	//�����жϸ�����ĸ�ʽ
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	//����һ�����������
	static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count, bool isShadow)
	{
		if (isShadow)
			glGenTextures(1, outID);
		else
			glCreateTextures(TextureTarget(multisampled), count, outID);
	}
	//����һ����Ⱦ�������
	static void CreateRenderbuffer(uint32_t* outID, uint32_t count)
	{
		//��δ��
		glCreateRenderbuffers(count, outID);
	}

	//��һ���������
	static void BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}
	//����Ⱦ�������
	static void BindRenderbuffer(uint32_t id) 
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}

	//Ϊ֡�����һ����ɫ���������
	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		//�����ֹ��һ������������ʹ��glTexImage2DMultisample����
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			//������Ӧ�����ö༶��Զ����Ļ��ж�����Ļ��ƺ͹���
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		//�󶨵�֡���������ȥ
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	//����Ⱥ�ģ��ȸ�ʽ��������󶨵�֡������ȥ
	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	//=============================================================================================
	//�󶨲�ͬDepthֵ
	static void AttachDepthRenderbuffer(uint32_t id, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, id);
	}

	//=============================================================================================

	//��ShadowMap�ȸ�ʽ��������󶨵�֡������ȥ
	static void AttachShadowTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
	}

	//����Ƿ�Ϊ���������ĸ�ʽ����ȷ��
	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		case FramebufferTextureFormat::Depth: return true;
		}
		return false;
	}

	//���ڼ����ɫ����ֵ�Ƿ�Ϊ��
	static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		GD_ENGINE_ASSERT(false, "this is UnKnow");
		return 0;
	}

}

namespace GodDecay 
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		//��֮ǰ��ӵļ��뵽������[֮ǰ�����ݵ���]
		for (auto spec : m_Specification.Attachments.Attachments)
		{//һ����˵����Ȼ���ģ�建����һ��֡����ֻ����һ�ݼ���
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}
		//��Ⱦ��������ݵ��뵽���༯����
		for (auto render : m_Specification.Attachments.Renderbuffers) 
		{
			//������Ͳ������ж��ˣ�ֱ����ӵ�������
			m_RenderbufferAttachmentSpecifications.emplace_back(render);
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteRenderbuffers(m_DepthAttachments.size(), m_DepthAttachments.data());
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
			glDeleteRenderbuffers(m_DepthAttachments.size(), m_DepthAttachments.data());
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;
		if (m_ColorAttachmentSpecifications.size() || m_RenderbufferAttachmentSpecifications.size() || m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			m_DepthAttachments.resize(m_RenderbufferAttachmentSpecifications.size());

			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size(), false);
			Utils::CreateRenderbuffer(m_DepthAttachments.data(), m_DepthAttachments.size());


			for (int i = 0; i < m_ColorAttachments.size(); ++i)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}

			if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
			{
				Utils::CreateTextures(multisample, &m_DepthAttachment, 1, m_DepthAttachmentSpecification.TextureFormat == FramebufferTextureFormat::Depth ? true : false);
				Utils::BindTexture(multisample, m_DepthAttachment);
				switch (m_DepthAttachmentSpecification.TextureFormat)
				{
					//������Բ��û����������������������õ�
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				case FramebufferTextureFormat::Depth:
					//����Ӧ����ShadowMap����
					Utils::AttachShadowTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				}
			}

			//��������RendBufferֵ
			for (int i = 0; i < m_DepthAttachments.size(); ++i) 
			{
				Utils::BindRenderbuffer(m_DepthAttachments[i]);

				//����ֻŪDepth���
				switch (m_RenderbufferAttachmentSpecifications[i].RenderFormat)
				{
				case FramebufferRenderbufferFormat::Depth:
					Utils::AttachDepthRenderbuffer(m_DepthAttachments[i], GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				case FramebufferRenderbufferFormat::Depth16:
					Utils::AttachDepthRenderbuffer(m_DepthAttachments[i], GL_DEPTH_COMPONENT16, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				case FramebufferRenderbufferFormat::Depth24:
					Utils::AttachDepthRenderbuffer(m_DepthAttachments[i], GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				case FramebufferRenderbufferFormat::Depth32:
					Utils::AttachDepthRenderbuffer(m_DepthAttachments[i], GL_DEPTH_COMPONENT32, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				}
			}

			if (m_ColorAttachments.size() > 1)
			{
				//��Ϊ����Ĭ��һ���Դ���Ϊ4�����������4�����൱��������ĸ�������
				GD_ENGINE_ASSERT(m_ColorAttachments.size() <= 4, "ColorAttachments is 4 and size <= 4");
				GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				glDrawBuffers(m_ColorAttachments.size(), buffers);
			}
			else if (m_ColorAttachments.empty() && m_DepthAttachments.empty() && m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::Depth)
			{
				glDrawBuffer(GL_NONE);
			}
			else if (m_ColorAttachments.empty() && m_DepthAttachmentSpecification.TextureFormat == FramebufferTextureFormat::Depth) 
			{
				//��������Ӱ���ɹ������ǲ���������κβ���[����������ڴ˸���]
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}
		}
		//�����֡����ĸ�����������[��Ϊ֡��������������Ҫһ����ɫ���������������ǲ���Ҫ��ɫ��������������Ҫ����һ���ж�]
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE && 
			m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::Depth && (m_ColorAttachments.size() >= 1 && m_DepthAttachments.size() >= 1))
			GD_ENGINE_ASSERT(false, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::AppendCubeTextureAttachment(uint32_t id, uint32_t index)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, id, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			GD_ENGINE_ASSERT("Attachment to resize framebuffer to {0} , {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}
	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		GD_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "ColorAttackments is empty");

		//��ȡShader�е�Layout�е��±��е�ֵ
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		GD_ENGINE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "this Attachments is empty");

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::HazelFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}
