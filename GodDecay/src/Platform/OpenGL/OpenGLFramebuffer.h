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

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			GD_ENGINE_ASSERT(index < m_ColorAttachments.size(), "not find colorAttachemnts or Attachments is Emty");
			return m_ColorAttachments[index];
		}

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		//存储framebuffer对象的集合和深度模板对象
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		//颜色纹理集合和深度纹理，一个framebuffer在一般是存在几个ColorAttachment的，但深度和模板格式的缓冲只有一份
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}



