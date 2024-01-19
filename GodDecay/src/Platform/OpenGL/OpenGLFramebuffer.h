#pragma once

#include "GodDecay/Core/Base.h"

#include "GodDecay/Renderer/Framebuffer.h"

namespace GodDecay
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void AppendCubeTextureAttachment(uint32_t id, uint32_t index, uint32_t mip) override;
		virtual void AppendRenderbufferAttachment(uint32_t width, uint32_t height, uint32_t id, FramebufferRenderbufferFormat type) override;
		virtual void Append2DTextureAttachment(uint32_t id, uint32_t mip = 0) override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			GD_ENGINE_ASSERT(index < m_ColorAttachments.size(), "not find colorAttachemnts or Attachments is Emty");
			return m_ColorAttachments[index];
		}

		virtual uint32_t GetDepthTextureAttachmentRendererID() const override
		{
			//������ʱ���ü��
			return m_DepthAttachment;
		}

		virtual uint32_t GetDepthRenderbufferAttachmentRendererID(uint32_t index = 0) const override
		{
			GD_ENGINE_ASSERT(index < m_DepthAttachments.size(), "not find depthAttachemnts or Attachments is Emty");
			return m_DepthAttachments[index];
		}

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		//�洢framebuffer����ļ��Ϻ����ģ�����
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		std::vector<FramebufferRenderbufferSpecification> m_RenderbufferAttachmentSpecifications;

		//��ɫ�����Ϻ��������һ��framebuffer��һ���Ǵ��ڼ���ColorAttachment�ģ�����Ⱥ�ģ���ʽ�Ļ���ֻ��һ��
		std::vector<uint32_t> m_ColorAttachments;
		std::vector<uint32_t> m_DepthAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}



