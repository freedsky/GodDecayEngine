#pragma once
#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"
#include "EditorCamera.h"
/// <summary>
/// 天空盒对象应该是全局仅一份，使用单例模式
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

		//先用路径或者纯色数据对立方体贴图进行初始化
		void Init(std::vector<std::string> paths);
		void Init(uint32_t width, uint32_t height);

		//天空盒类型转换
		void ChangeSkyBoxType(SkyType type);

		//绘制天空盒
		
		//实时相机位置渲染
		void Update(const Camera& camera, const glm::mat4& transform);
		//编辑相机渲染
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

		//加载显示图片
		void LoadDispalyTexture2D();
		
		Ref<Shader> m_SkyBoxShader;
		Ref<TextureCube> m_SkyBoxCubeTexture;
		Ref<TextureCube> m_SkyBoxColorTexture;
		Ref<TextureCube> m_CurrentSkyBoxTexture;

		//纯色变换变量
		glm::vec4 m_SkyBoxColor;
		//顶点缓冲对象
		Ref<VertexArrayBuffer> m_SkyBoxBuffer;
		//图像路径
		std::vector<std::string> m_Paths;
		//用于显示的2Dtexture集合
		std::vector<Ref<Texture2D>> m_DispalySkyTextures;
		//当前的skybox的渲染类型
		SkyType m_CurrentSkyType;

		static Ref<SkyBox> m_Instance;
	};
}



