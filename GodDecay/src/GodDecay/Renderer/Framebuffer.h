#pragma once

#include "GodDecay/Core/Base.h"

/// <summary>
/// ���ö�Ŀ����Ⱦ���������
/// </summary>
namespace GodDecay 
{
	//����texture��������Ҫ����������[���ھɴ��벻�����޸�]
	enum class FramebufferTextureFormat
	{
		None = 0,
		//Color
		RGBA8,
		//��ɫͨ��[���ڴ洢��ѡ�е�entityID]
		RED_INTEGER,
		//Depth
		DEPTH24STENCIL8,
		//Depthֵ
		Depth
	};

	//Renderbuffer�ĸ�ʽ
	enum class FramebufferRenderbufferFormat
	{
		None = 0,
		DEPTH24STENCIL8,
		Depth,
		Depth16,
		Depth24,
		Depth32
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
	//��Ⱦ������󸽼�
	struct FramebufferRenderbufferSpecification 
	{
		FramebufferRenderbufferSpecification() = default;
		FramebufferRenderbufferSpecification(FramebufferRenderbufferFormat format)
			:RenderFormat(format)
		{}

		FramebufferRenderbufferFormat RenderFormat = FramebufferRenderbufferFormat::None;
	};

	//���������֡���������֡��������������������Ӧ�ü��Ͽ��԰���Ⱦ����

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		//��ɫ����
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}
		//���ģ�帽��
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferRenderbufferSpecification> renderbuffers)
			: Renderbuffers(renderbuffers)
		{}
		//˫����
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments, std::initializer_list<FramebufferRenderbufferSpecification> renderbuffers)
			: Attachments(attachments), Renderbuffers(renderbuffers)
		{}
		//�˺�Attachments���Ͻ��洢����ɫ��صĸ�������Renderbuffers������洢�����ģ��������ݵĸ�����
		std::vector<FramebufferTextureSpecification> Attachments;
		std::vector<FramebufferRenderbufferSpecification> Renderbuffers;

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

		//Ϊ֡���帽������
		virtual void AppendCubeTextureAttachment(uint32_t id, uint32_t index, uint32_t mip = 0) = 0;
		//Ϊ���帽�ӻ������
		virtual void AppendRenderbufferAttachment(uint32_t width, uint32_t height, uint32_t id, FramebufferRenderbufferFormat type) = 0;
		//����ͨ��2D����
		virtual void Append2DTextureAttachment(uint32_t id, uint32_t mip = 0) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		//��ȡ��Ļ��������ѡ�е�entityID��������
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		//����������صĻ�������[����Red������˵]
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		//ͨ���±�����ȡ����ͬ������֡���������Ϊ�ں�����Ⱦ�У�������ֻ��Ⱦһ��֡����������洢��Ⱦ���������
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		//������Ȼ�������
		virtual uint32_t GetDepthTextureAttachmentRendererID() const = 0;
		//ͬ�������±���ȥȡ���ڵ���Ⱦ����
		virtual uint32_t GetDepthRenderbufferAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}



