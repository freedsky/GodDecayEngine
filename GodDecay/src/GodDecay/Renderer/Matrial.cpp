#include "gdpch.h"
#include "Matrial.h"

namespace GodDecay 
{
	//�ڹ���ʱ�ȸ�һ��Ĭ�ϵ�texture��shader
	Matrial::Matrial()
	{
		//������Ӧ�ð����е�����Shaderȫ�����غã���ʱ��ֻ��Ҫ���ü���[��ʱû���Զ���Shader]
		m_ShaderList.Load("DefaultShader", "assets/shader/UniversalStand.glsl");

		//Ĭ�ϵ�texture
		Ref<Texture2D> WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		TextureLibrary defaultT;
		defaultT.AddTexture2D("DefaultTexture", WhiteTexture);
		
		LoadTexture("DefaultShader", defaultT);
		//-------------------------------------------------------------------
		//Ĭ���������±�Ϊ0����������м������1��ʼ
		m_ShaderList.Get("DefaultShader")->Bind();
		m_ShaderList.Get("DefaultShader")->SetInt("DefaultTexture", 0);
	}
	//����Shader
	void Matrial::LoadShader(std::string shaderName, std::string path)
	{
		//�õĹ�ϣmap�����д���ȥ�ظ�
		m_ShaderList.Load(shaderName, path);
		
	}

	//����Texture
	void Matrial::LoadTexture(std::string mapName, TextureLibrary textures)
	{
		m_TextureList[mapName] = textures;
		
	}

	void Matrial::LoadShaderToRenderModel(std::string shaderName)
	{
		LoadTest();
	}
	//������չ��ͬ����Ⱦ��ʽ(Ĭ����Ⱦ��ʽ�ڹ���ʱ��ʼ������Ͳ�������)---------------------------------------
	
	//���Է���
	void Matrial::LoadTest()
	{
		//������Ӧ�ð����е�����Shaderȫ�����غã���ʱ��ֻ��Ҫ���ü���[��ʱû���Զ���Shader]
		m_ShaderList.Load("TextShader", "assets/shader/TestShader.glsl");

		//Ĭ�ϵ�texture
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
		//Ĭ���������±�Ϊ0����������м������1��ʼ
		m_ShaderList.Get("TextShader")->Bind();
		m_ShaderList.Get("TextShader")->SetInt("DefaultTexture", 0);
		m_ShaderList.Get("TextShader")->SetInt("Text1", 1);
	}
}
