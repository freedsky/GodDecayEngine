#pragma once

#include "GodDecay/Core/Base.h"

/// <summary>
/// ���ö�Ŀ����Ⱦ���������
/// </summary>
namespace GodDecay 
{
	//����texture��������Ҫ����������
	enum class FramebufferTextureFormat
	{
		None = 0,
		//Color
		RGBA8,
		//��ɫͨ��[���ڴ洢��ѡ�е�entityID]
		RED_INTEGER,
		//Depth
		DEPTH24STENCIL8,

		//����Ĭ��ֵ
		Depth = DEPTH24STENCIL8
	};

	//�˽ṹ�����������
	struct FramebufferTextureSpecification
	{
		//����һ�����������߻�������ݸ�ʽ
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format)
		{}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	//���������֡���������֡��������������������Ӧ�ü��Ͽ��԰���Ⱦ����

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
		FramebufferAttachmentSpecification Attachments;//֡�������
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		//��ȡ��Ļ��������ѡ�е�entityID��������
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		//����������صĻ�������[����Red������˵]
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		//ͨ���±�����ȡ����ͬ������֡���������Ϊ�ں�����Ⱦ�У�������ֻ��Ⱦһ��֡����������洢��Ⱦ���������
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}



