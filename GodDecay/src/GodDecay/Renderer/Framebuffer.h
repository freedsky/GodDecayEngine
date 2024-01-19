#pragma once

#include "GodDecay/Core/Base.h"

/// <summary>
/// 设置多目标渲染对象的设置
/// </summary>
namespace GodDecay 
{
	//设置texture缓冲所需要的数据类型[对于旧代码不给予修改]
	enum class FramebufferTextureFormat
	{
		None = 0,
		//Color
		RGBA8,
		//单色通道[用于存储被选中的entityID]
		RED_INTEGER,
		//Depth
		DEPTH24STENCIL8,
		//Depth值
		Depth
	};

	//Renderbuffer的格式
	enum class FramebufferRenderbufferFormat
	{
		None = 0,
		DEPTH24STENCIL8,
		Depth,
		Depth16,
		Depth24,
		Depth32
	};

	//此结构体针对纹理附件
	struct FramebufferTextureSpecification
	{
		//设置一个纹理附件或者缓冲的数据格式
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format)
		{}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};
	//渲染缓冲对象附件
	struct FramebufferRenderbufferSpecification 
	{
		FramebufferRenderbufferSpecification() = default;
		FramebufferRenderbufferSpecification(FramebufferRenderbufferFormat format)
			:RenderFormat(format)
		{}

		FramebufferRenderbufferFormat RenderFormat = FramebufferRenderbufferFormat::None;
	};

	//这个是设置帧缓冲对象，往帧缓冲对象绑定纹理附件，这里应该加上可以绑定渲染缓冲

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		//颜色附件
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}
		//深度模板附件
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferRenderbufferSpecification> renderbuffers)
			: Renderbuffers(renderbuffers)
		{}
		//双附件
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments, std::initializer_list<FramebufferRenderbufferSpecification> renderbuffers)
			: Attachments(attachments), Renderbuffers(renderbuffers)
		{}
		//此后，Attachments集合仅存储与颜色相关的附件；而Renderbuffers集合则存储与深度模板相关数据的附件。
		std::vector<FramebufferTextureSpecification> Attachments;
		std::vector<FramebufferRenderbufferSpecification> Renderbuffers;

	};

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		FramebufferAttachmentSpecification Attachments;//帧缓冲对象
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		//为帧缓冲附加纹理
		virtual void AppendCubeTextureAttachment(uint32_t id, uint32_t index, uint32_t mip = 0) = 0;
		//为缓冲附加缓冲对象
		virtual void AppendRenderbufferAttachment(uint32_t width, uint32_t height, uint32_t id, FramebufferRenderbufferFormat type) = 0;
		//绑定普通的2D纹理
		virtual void Append2DTextureAttachment(uint32_t id, uint32_t mip = 0) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		//读取屏幕的像素所选中的entityID，并返回
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		//用于清除不必的缓冲数据[对于Red缓冲来说]
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		//通过下标来获取，不同创建的帧缓冲对象，因为在后期渲染中，不可以只渲染一个帧缓冲对象来存储渲染缓冲的数据
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		//返回深度缓冲纹理
		virtual uint32_t GetDepthTextureAttachmentRendererID() const = 0;
		//同样根据下标来去取对于的渲染附件
		virtual uint32_t GetDepthRenderbufferAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}



