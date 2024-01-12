#pragma once
#include "RenderCommand.h"
#include "VertexArrayBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"

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
			PureColor,
			HDR
		};

		~SkyBox();

		static Ref<SkyBox> GetInstance();

		//先用路径或者纯色数据对立方体贴图进行初始化
		void Init(std::vector<std::string> paths);
		void Init(uint32_t width, uint32_t height);
		void Init(std::string path);

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
		inline Ref<Texture2D>& GetDisplayHDRSkyTexture() { return m_HDRTexture; }
		inline std::string& GetHDRPath() { return m_HDRPath; }
		inline bool GetCurrentIsHDR() { return m_CurrentIsHDR; }
		inline Ref<TextureCube>& GetIrradianceTexture() { return m_IrradianceTextureCube; }

	private:
		SkyBox();

		//加载显示图片
		void LoadDisplayTexture2D();
		
		Ref<Shader> m_SkyBoxShader;
		Ref<Shader> m_HDRMapCubeShader;
		Ref<Shader> m_IrradianceShader;
		Ref<TextureCube> m_SkyBoxCubeTexture;
		Ref<TextureCube> m_SkyBoxColorTexture;
		Ref<TextureCube> m_CurrentSkyBoxTexture;

		//加载HDR天空
		Ref<Texture2D> m_HDRTexture;
		Ref<TextureCube> m_SkyBoxCubeHDRTexture;

		//纯色变换变量
		glm::vec4 m_SkyBoxColor;
		//顶点缓冲对象
		Ref<VertexArrayBuffer> m_SkyBoxBuffer;
		//图像路径
		std::vector<std::string> m_Paths;
		std::string m_HDRPath;
		//用于显示的2Dtexture集合
		std::vector<Ref<Texture2D>> m_DispalySkyTextures;
		//当前的skybox的渲染类型
		SkyType m_CurrentSkyType;

		//渲染帧缓冲变量
		Ref<Framebuffer> m_SkyBoxHDRFramebuffer;

		//当前立方体贴图是否为HDR
		bool m_CurrentIsHDR;

		//处理辐照度图
		Ref<TextureCube> m_IrradianceTextureCube;
		Ref<Framebuffer> m_IrradianceFramebuffer;

		static Ref<SkyBox> m_Instance;
	};
}



