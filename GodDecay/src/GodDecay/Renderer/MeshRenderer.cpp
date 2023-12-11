#include "gdpch.h"
#include "MeshRenderer.h"
#include "SceneLightController.h"

namespace GodDecay 
{
	uint32_t typeCount = 0;

	MeshRenderer::MeshRenderer()
	{
		s_Mesh = CreateRef<MeshRenderData>();
		s_Mesh->MeshVertexArray = VertexArrayBuffer::Create();
		//������Կ��Ƕ�matrial���г�ʼ������������Ĭ�ϵ�Shader��Texture
		s_Mesh->ShaderName = "DefaultShader";//����Shader��Ĭ������
		//��ʼ����Ĭ�����Shader��Ϊ�л�Shader������׼����Test��
		s_Mesh->MatrialData.LoadShaderToRenderModel("sda");
	}

	void MeshRenderer::LoadMesh(Mesh& mesh, int entityId)
	{
		//�ȳ�ʼ��mesh
		s_Mesh->MeshData = mesh;
		if (s_Mesh->MeshVertexArray != nullptr) 
		{
			s_Mesh->MeshVertexArray = nullptr;
			s_Mesh->MeshVertexArray = VertexArrayBuffer::Create();
		}
			

		//��id��ӽ���
		for (int i = 0; i < s_Mesh->MeshData.GetMeshData().size(); ++i)
		{
			s_Mesh->MeshData.GetMeshData()[i].EntityID = entityId;
		}

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(s_Mesh->MeshData.GetMeshData().data(), s_Mesh->MeshData.GetMeshData().size() * sizeof(MeshProperty));
		vertexBuffer->Setlayout({
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float3, "a_Normal"   },
			{ShaderDataType::Float3, "a_Tangent"  },
			{ShaderDataType::Float2, "a_Texcoords"},
			{ShaderDataType::Int,    "a_EntityID" }
		});
		s_Mesh->MeshVertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(s_Mesh->MeshData.GetMeshIndices().data(), s_Mesh->MeshData.GetMeshIndices().size());
		s_Mesh->MeshVertexArray->SetIndexBuffer(indexBuffer);
	}

	void MeshRenderer::BeginDrawMesh(const Camera& camera, const glm::mat4& transform, BaseMeshType type)
	{
		//runtime�в���ı�meshtype������ʱ����ȥ���³�ʼ��mesh
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_ViewProjection", viewProj);
		//���������λ���������ݹ�ȥ[��Camera�������û��Position������Ҫ�Լ�ʵ�ֺ���ܽ������Ը�ֵ]
	}

	void MeshRenderer::BeginDrawMesh(const EditorCamera& camera, BaseMeshType type, std::string path)
	{
		//��һ���Ȱ�type�ȸ�ֵ��ȥ��Ȼ���ڴ˺�ÿһ�ζ�Ҫȥ��⣬�����Ƿ��ǰһ����ȣ�����Ⱦ�Ҫ���³�ʼ��
		if (typeCount == 0)
		{
			s_Mesh->type = type;
			s_Mesh->path = path;
			typeCount++;
		}
		else if (s_Mesh->type != type || s_Mesh->path.compare(path))
		{
			if (path.compare(""))
				s_Mesh->MeshData.SetModelPath(path);
			int id = s_Mesh->MeshData.GetMeshData()[0].EntityID;
			s_Mesh->MeshData.ClearMeshData();//�����п��ǰ�ģ�����ݽ�������
			s_Mesh->MeshData.Init((BaseMeshType)type, s_Mesh->MeshData.GetModelPath());
			LoadMesh(s_Mesh->MeshData, id);
			s_Mesh->type = type;
			s_Mesh->path = path;
			GD_ENGINE_INFO("Editor Chanage this model");
		}

		//��ʱ�Ȱ�Ĭ�ϵ���Ⱦ����
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_ViewProjection", camera.GetViewProjection());
		//���������λ���������ݹ�ȥ
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat3("u_ViewPosition", camera.GetPosition());
		//GD_ENGINE_TRACE("x = {0}, y = {1}, z = {2} ", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	}

	void MeshRenderer::DrawMesh(const glm::mat4& transform)
	{
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat4("DefaultColor", s_Mesh->MatrialData.GetMeshColor());
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_Model", transform);

		//���SceneLights���ϲ�Ϊ�վͽ������Ը���
		if (SceneLightController::GetSceneLights().size() > 0)
			UpDateUniformPropertices();

		//��Uniform����
		LoadUniformPropertices();

		//��TextureUniform�±�
		int T_size = s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2DLibraries().size();
		if (T_size == 1) 
		{
			s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).Get("DefaultTexture")->Bind();
		}
		else
		{
			std::vector<std::string> T_name;
			for (auto& t : s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2DLibraries())
			{
				T_name.push_back(t.first);
			}
			for (int i = 0; i < T_size; ++i)
			{
				s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).Get(T_name[i])->Bind(i);
			}
		}

			
		s_Mesh->MeshVertexArray->Bind();
		if(s_Mesh->type == BaseMeshType::CUBE || s_Mesh->type == BaseMeshType::MODEL)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::Normal);
		else if(s_Mesh->type == BaseMeshType::CIRLE)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::STRIP);
	}

	void MeshRenderer::EndDrawMesh()
	{

	}
	void MeshRenderer::ChanageShader(std::string ShaderName)
	{
		if (ShaderName.c_str() != nullptr && ShaderName.size() != 0 && s_Mesh->ShaderName.compare(ShaderName)) 
		{
			//ֻ�ǰ�Shader���Ƹ��滻������
			s_Mesh->ShaderName = ShaderName;
		}
			
	}

	void MeshRenderer::LoadUniformPropertices()
	{
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetBool())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetBool(b.first, b.second);
		}
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetFloat())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat(b.first, b.second);
		}
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetInt())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt(b.first, b.second);
		}
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetVec3())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat3(b.first, b.second);
		}
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetVec4())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat4(b.first, b.second);
		}
		for (auto& b : s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).GetMat4())
		{
			s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4(b.first, b.second);
		}
	}
	

	void MeshRenderer::UpDateUniformPropertices()
	{
		if (SceneLightController::ChangeFlag != 0) 
		{
			//��������ֵ[ֻ����Light���]
			auto lights = SceneLightController::GetSceneLights();
			for (auto light : lights) 
			{
				//���ݹ�Դ���͸��²�ͬ������ֵ
				if (light->GetLightType() == LightType::Direction)
				{
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("direction_rotatiion", light->GetLightRotatetion());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("direction_position", light->GetLightPosition());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("direction_lightcolor", light->GetLightColor());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("direction_ambient", light->GetLightAmbient());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("direction_diffuse", light->GetLightDiffuse());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("direction_specular", light->GetLightSpecular());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("direction_shininess", light->GetLightShininess());
					s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("direction_intensity", SceneLightController::GetEnvironmentLightIntensity());
				}
				else if (light->GetLightType() == LightType::Point)
				{

				}
				else if (light->GetLightType() == LightType::Spot)
				{

				}
			}
			//��������flag������Ϊ0
			SceneLightController::ChangeFlag = 0;
		}
	}
}
