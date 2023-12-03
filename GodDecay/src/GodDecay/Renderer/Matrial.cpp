#include "gdpch.h"
#include "Matrial.h"

namespace GodDecay 
{
	//在构造时先给一个默认的texture和shader
	Matrial::Matrial()
	{
		//在这里应该把所有的内置Shader全部加载好，到时候只需要调用即可[暂时没有自定义Shader]
		m_ShaderList.Load("DefaultShader", "assets/shader/UniversalStand.glsl");

		//默认的texture
		Ref<Texture2D> WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		TextureLibrary defaultT;
		defaultT.AddTexture2D("DefaultTexture", WhiteTexture);
		
		LoadTexture("DefaultShader", defaultT);
		//-------------------------------------------------------------------
		//默认纹理下下标为0，其余想进行加载请从1开始
		m_ShaderList.Get("DefaultShader")->Bind();
		m_ShaderList.Get("DefaultShader")->SetInt("DefaultTexture", 0);
	}
	//加载Shader
	void Matrial::LoadShader(std::string shaderName, std::string path)
	{
		//用的哈希map，所有存在去重复
		m_ShaderList.Load(shaderName, path);
		
	}

	//加载Texture
	void Matrial::LoadTexture(std::string mapName, TextureLibrary textures)
	{
		m_TextureList[mapName] = textures;
		
	}

	void Matrial::LoadShaderToRenderModel(std::string shaderName)
	{
		LoadTest();
	}
	//往后扩展不同的渲染方式(默认渲染方式在构造时初始化这里就不定义了)---------------------------------------
	
	//测试方法
	void Matrial::LoadTest()
	{
		//在这里应该把所有的内置Shader全部加载好，到时候只需要调用即可[暂时没有自定义Shader]
		m_ShaderList.Load("TextShader", "assets/shader/TestShader.glsl");

		//默认的texture
		Ref<Texture2D> WhiteTexture1 = Texture2D::Create(1, 1);
		uint32_t whiteTextureData1 = 0xffffffff;
		WhiteTexture1->SetData(&whiteTextureData1, sizeof(uint32_t));

		Ref<Texture2D> WhiteTexture2 = Texture2D::Create(1, 1);
		uint32_t whiteTextureData2 = 0xffffffff;
		WhiteTexture2->SetData(&whiteTextureData2, sizeof(uint32_t));

		TextureLibrary defaultT;
		defaultT.AddTexture2D("DefaultTexture", WhiteTexture1);
		defaultT.AddTexture2D("Text1", WhiteTexture2);

		LoadTexture("TextShader", defaultT);
		//-------------------------------------------------------------------
		//默认纹理下下标为0，其余想进行加载请从1开始
		m_ShaderList.Get("TextShader")->Bind();
		m_ShaderList.Get("TextShader")->SetInt("DefaultTexture", 0);
		m_ShaderList.Get("TextShader")->SetInt("Text1", 1);
	}
}
