#pragma once
#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"
#include "Texture.h"

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
			PureColor
		};

		~SkyBox();

		static Ref<SkyBox> GetInstance();

		//����·�����ߴ�ɫ���ݶ���������ͼ���г�ʼ��
		void Init(std::vector<std::string> paths);
		void Init(uint32_t width, uint32_t height);

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

	private:
		SkyBox();

		//������ʾͼƬ
		void LoadDispalyTexture2D();
		
		Ref<Shader> m_SkyBoxShader;
		Ref<TextureCube> m_SkyBoxCubeTexture;
		Ref<TextureCube> m_SkyBoxColorTexture;
		Ref<TextureCube> m_CurrentSkyBoxTexture;

		//��ɫ�任����
		glm::vec4 m_SkyBoxColor;
		//���㻺�����
		Ref<VertexArrayBuffer> m_SkyBoxBuffer;
		//ͼ��·��
		std::vector<std::string> m_Paths;
		//������ʾ��2Dtexture����
		std::vector<Ref<Texture2D>> m_DispalySkyTextures;
		//��ǰ��skybox����Ⱦ����
		SkyType m_CurrentSkyType;

		static Ref<SkyBox> m_Instance;
	};
}



