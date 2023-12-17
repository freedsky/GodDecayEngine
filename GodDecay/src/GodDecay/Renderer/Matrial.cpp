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
		BlinnPhongLight();
	}
	//������չ��ͬ����Ⱦ��ʽ(Ĭ����Ⱦ��ʽ�ڹ���ʱ��ʼ������Ͳ�������)---------------------------------------
	
	//���Է���
	void Matrial::LoadTest()
	{
		//������Ӧ�ð����е�����Shaderȫ�����غã���ʱ��ֻ��Ҫ���ü���[��ʱû���Զ���Shader]
		m_ShaderList.Load("TextShader", "assets/shader/TestShader.glsl");

		//Ĭ�ϵ�texture
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
		//Ĭ���������±�Ϊ0����������м������1��ʼ
		m_ShaderList.Get("TextShader")->Bind();
		m_ShaderList.Get("TextShader")->SetInt("DefaultTexture", 0);
		m_ShaderList.Get("TextShader")->SetInt("Text1", 1);

		//uniform����ֵ���
		UniformProperties textP;
		textP.AddProperties("Tcolor1", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		textP.AddProperties("Tcolor2", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		textP.AddProperties("Tcolor3", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_UniformProperties["TextShader"] = textP;
	}

	void Matrial::BlinnPhongLight()
	{
		//Shader�ļ���
		m_ShaderList.Load("BlinnPhongShader", "assets/shader/BlinnPhongShader.glsl");
		//BlinnPhong��������ͼһ������������ͼ�͸߹ⷴ����ͼ
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
		//��ʼ����Դ������
		m_ShaderList.Get("BlinnPhongShader")->SetInt("DirectionNumber",0);
		m_ShaderList.Get("BlinnPhongShader")->SetInt("PointNumber",0);
		m_ShaderList.Get("BlinnPhongShader")->SetInt("SpotNumber",0);

		//�������,��ȥ������Ӧ�����ԣ��ں������Ʋ�ȥ����
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
		//Ԥ�Ȱ���Щ�����ȴ�����������ʱ����ݼ��Ϻ͹�Դ�����������и���
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
		//spotԭ��ͬ��
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
