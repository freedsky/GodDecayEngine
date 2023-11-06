#pragma once

#include "GodDecay/Core/Base.h"

/// <summary>
/// 设置多目标渲染对象的设置
/// </summary>
namespace GodDecay 
{
	//设置texture缓冲所需要的数据类型
	enum class FramebufferTextureFormat
	{
		None = 0,
		//Color
		RGBA8,
		//单色通道[用于存储被选中的entityID]
		RED_INTEGER,
		//Depth
		DEPTH24STENCIL8,

		//设置默认值
		Depth = DEPTH24STENCIL8
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

	//这个是设置帧缓冲对象，往帧缓冲对象绑定纹理附件，这里应该加上可以绑定渲染缓冲

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}

		std::vector<FramebufferTextureSpecification> Attachments;

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

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		//读取屏幕的像素所选中的entityID，并返回
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		//用于清除不必的缓冲数据[对于Red缓冲来说]
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		//通过下标来获取，不同创建的帧缓冲对象，因为在后期渲染中，不可以只渲染一个帧缓冲对象来存储渲染缓冲的数据
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}



