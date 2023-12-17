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
		BlinnPhongLight();
	}
	//往后扩展不同的渲染方式(默认渲染方式在构造时初始化这里就不定义了)---------------------------------------
	
	//测试方法
	void Matrial::LoadTest()
	{
		//在这里应该把所有的内置Shader全部加载好，到时候只需要调用即可[暂时没有自定义Shader]
		m_ShaderList.Load("TextShader", "assets/shader/TestShader.glsl");

		//默认的texture
		uint32_t whiteTextureData1 = 0xffffffff;
		Ref<Texture2D> WhiteTexture1 = Texture2D::Create(1, 1);
		WhiteTexture1->SetData(&whiteTextureData1, sizeof(uint32_t));

		Ref<Texture2D> WhiteTexture2 = Texture2D::Create(1, 1);
		WhiteTexture2->SetData(&whiteTextureData1, sizeof(uint32_t));

		TextureLibrary defaultT;
		defaultT.AddTexture2D("DefaultTexture", WhiteTexture1);
		defaultT.AddTexture2D("Text1", WhiteTexture2);

		LoadTexture("TextShader", defaultT);
		//-------------------------------------------------------------------
		//默认纹理下下标为0，其余想进行加载请从1开始
		m_ShaderList.Get("TextShader")->Bind();
		m_ShaderList.Get("TextShader")->SetInt("DefaultTexture", 0);
		m_ShaderList.Get("TextShader")->SetInt("Text1", 1);

		//uniform属性值添加
		UniformProperties textP;
		textP.AddProperties("Tcolor1", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		textP.AddProperties("Tcolor2", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		textP.AddProperties("Tcolor3", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_UniformProperties["TextShader"] = textP;
	}

	void Matrial::BlinnPhongLight()
	{
		//Shader的加载
		m_ShaderList.Load("BlinnPhongShader", "assets/shader/BlinnPhongShader.glsl");
		//BlinnPhong有两张贴图一个是漫反射贴图和高光反射贴图
		uint32_t white = 0xffffffff;
		Ref<Texture2D> diffuseTexture = Texture2D::Create(1, 1);
		diffuseTexture->SetData(&white, sizeof(uint32_t));

		Ref<Texture2D> specularTexture = Texture2D::Create(1, 1);
		specularTexture->SetData(&white, sizeof(uint32_t));

		TextureLibrary blinnPhongT;
		blinnPhongT.AddTexture2D("DiffuseTexture", diffuseTexture);
		blinnPhongT.AddTexture2D("SpecularTexture", specularTexture);
		LoadTexture("BlinnPhongShader", blinnPhongT);

		m_ShaderList.Get("BlinnPhongShader")->Bind();
		m_ShaderList.Get("BlinnPhongShader")->SetInt("DiffuseTexture", 0);
		m_ShaderList.Get("BlinnPhongShader")->SetInt("SpecularTexture", 1);
		//初始化光源的数量
		m_ShaderList.Get("BlinnPhongShader")->SetInt("DirectionNumber",0);
		m_ShaderList.Get("BlinnPhongShader")->SetInt("PointNumber",0);
		m_ShaderList.Get("BlinnPhongShader")->SetInt("SpotNumber",0);

		//添加属性,先去创建相应的属性，在后续绘制才去更新
		UniformProperties lightProerties;
		//direction
		lightProerties.AddProperties("direction_rotatiion", glm::vec3(0.0f));
		lightProerties.AddProperties("direction_position", glm::vec3(0.0f));
		lightProerties.AddProperties("direction_lightcolor", glm::vec4(0.0f));
		lightProerties.AddProperties("direction_ambient", glm::vec4(0.0f));
		lightProerties.AddProperties("direction_diffuse", glm::vec4(0.0f));
		lightProerties.AddProperties("direction_specular", glm::vec4(0.0f));
		lightProerties.AddProperties("direction_shininess", 32.0f);
		lightProerties.AddProperties("direction_intensity", 0.2f);
		//point
		//预先把这些属性先创建起来，到时候根据集合和光源计数器来进行更新
		for (uint32_t i = 0; i < 4; ++i) 
		{
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].Position", glm::vec3(0.0f));
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].LightColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].DiffuseColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].SpecularColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].Shininess", 32.0f);
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].Constant", 1.0f);
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].Linear", 0.09f);
			lightProerties.AddProperties("Point[" + std::to_string(i) + "].Quadratic", 0.032f);
		}
		//spot原因同上
		for (uint32_t k = 0; k < 4; ++k) 
		{
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Position", glm::vec3(0.0f));
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Rotation", glm::vec3(0.0f));
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].LightColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].DiffuseColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].SpecularColor", glm::vec4(0.0f));
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Shininess", 32.0f);
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Constant", 1.0f);
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Linear", 0.09f);
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].Quadratic", 0.032f);
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].CutOff", 12.5f);
			lightProerties.AddProperties("Spot[" + std::to_string(k) + "].OuterCutOff", 17.5f);

		}

		m_UniformProperties["BlinnPhongShader"] = lightProerties;
	}
}
