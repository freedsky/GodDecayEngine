#pragma once
#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"

#include "Camera.h"
#include "EditorCamera.h"
/// <summary>
/// ��պж���Ӧ����ȫ�ֽ�һ�ݣ�ʹ�õ���ģʽ
/// </summary>

namespace GodDecay 
{
	class SkyBox
	{
	public:
		enum class SkyType 
		{
			Normal,
			PureColor,
			HDR
		};

		~SkyBox();

		static Ref<SkyBox> GetInstance();

		//����·�����ߴ�ɫ���ݶ���������ͼ���г�ʼ��
		void Init(std::vector<std::string> paths);
		void Init(uint32_t width, uint32_t height);
		void Init(std::string path);

		//��պ�����ת��
		void ChangeSkyBoxType(SkyType type);

		//������պ�
		
		//ʵʱ���λ����Ⱦ
		void Update(const Camera& camera, const glm::mat4& transform);
		//�༭�����Ⱦ
		void Update(const EditorCamera& camera);

		inline void SetSkyBoxColor(glm::vec4& color) { m_SkyBoxColor = color; }
		inline glm::vec4& GetSkyBoxColor() { return m_SkyBoxColor; }
		inline void SetSkyBoxType(SkyType type) { m_CurrentSkyType = type; }
		inline SkyType& GetSkyBoxType() { return m_CurrentSkyType; }
		inline Ref<TextureCube>& GetCurrentSkyBoxTexture() { return m_CurrentSkyBoxTexture; }
		inline std::vector<std::string>& GetPaths() { return m_Paths; }
		inline std::vector<Ref<Texture2D>>& GetDispalySkyTextures() { return m_DispalySkyTextures; }
		inline Ref<Texture2D>& GetDisplayHDRSkyTexture() { return m_HDRTexture; }
		inline std::string& GetHDRPath() { return m_HDRPath; }
		inline bool GetCurrentIsHDR() { return m_CurrentIsHDR; }
		inline Ref<TextureCube>& GetIrradianceTexture() { return m_IrradianceTextureCube; }

	private:
		SkyBox();

		//������ʾͼƬ
		void LoadDisplayTexture2D();
		
		Ref<Shader> m_SkyBoxShader;
		Ref<Shader> m_HDRMapCubeShader;
		Ref<Shader> m_IrradianceShader;
		Ref<TextureCube> m_SkyBoxCubeTexture;
		Ref<TextureCube> m_SkyBoxColorTexture;
		Ref<TextureCube> m_CurrentSkyBoxTexture;

		//����HDR���
		Ref<Texture2D> m_HDRTexture;
		Ref<TextureCube> m_SkyBoxCubeHDRTexture;

		//��ɫ�任����
		glm::vec4 m_SkyBoxColor;
		//���㻺�����
		Ref<VertexArrayBuffer> m_SkyBoxBuffer;
		//ͼ��·��
		std::vector<std::string> m_Paths;
		std::string m_HDRPath;
		//������ʾ��2Dtexture����
		std::vector<Ref<Texture2D>> m_DispalySkyTextures;
		//��ǰ��skybox����Ⱦ����
		SkyType m_CurrentSkyType;

		//��Ⱦ֡�������
		Ref<Framebuffer> m_SkyBoxHDRFramebuffer;

		//��ǰ��������ͼ�Ƿ�ΪHDR
		bool m_CurrentIsHDR;

		//������ն�ͼ
		Ref<TextureCube> m_IrradianceTextureCube;
		Ref<Framebuffer> m_IrradianceFramebuffer;

		static Ref<SkyBox> m_Instance;
	};
}



