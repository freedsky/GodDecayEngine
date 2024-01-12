#include "gdpch.h"
#include "MeshRenderer.h"
#include "SceneLightController.h"
#include "GodDecay/Renderer/Light/PointLight.h"
#include "GodDecay/Renderer/Light/SpotLight.h"

#include "Shadow.h"

namespace GodDecay 
{
	uint32_t typeCount = 0;

	MeshRenderer::MeshRenderer()
	{
		s_Mesh = CreateRef<MeshRenderData>();
		s_Mesh->MeshVertexArray = VertexArrayBuffer::Create();
		//������Կ��Ƕ�matrial���г�ʼ������������Ĭ�ϵ�Shader��Texture
		s_Mesh->ShaderName = "DefaultShader";//����Shader��Ĭ������
		//һЩ���������ĸ�ֵ
		s_Mesh->ReflectFlag = 1;
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

		//�������Դ����ͬ���������ж���Ķ������Բ�ʹ�û�ռ��һ���ִ���������һ������ʱ��������
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

	void MeshRenderer::DrawShadow(const Ref<Shader>& shadowShader, const glm::mat4& transform)
	{
		shadowShader->Bind();
		shadowShader->SetMat4("u_LightModel", transform);

		//��Ϊ�����ͼ�����Բ���Ҫ�κ�����������������ֱ�ӻ���[�������ͼ]
		s_Mesh->MeshVertexArray->Bind();
		if (s_Mesh->type == BaseMeshType::CUBE || s_Mesh->type == BaseMeshType::MODEL)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::Normal);
		else if (s_Mesh->type == BaseMeshType::CIRLE)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::STRIP);

	}

	void MeshRenderer::DrawMesh(const glm::mat4& transform)
	{
		//ͨ������ֵ����[����������Shader����ӵ�и�����]
		UpdateUniversalUniformPropertices(transform);

		//Shader��������ֵ����
		UpDateUniformPropertices();

		//��Uniform����
		LoadUniformPropertices();

		//GD_ENGINE_TRACE("Diection = {0}, Point = {1}, Spot = {2} ", SceneLightController::DirectionNum, SceneLightController::PointNum, SceneLightController::SpotNum);

		//�������±�
		LoadTexture2DOrCube();
			
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

	//�������⣿
	void MeshRenderer::LoadUniformPropertices()
	{
		//�ȰѼ����еĹ�Դ�������и���
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("DirectionNumber", SceneLightController::DirectionNum);
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("PointNumber", SceneLightController::PointNum);
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("SpotNumber", SceneLightController::SpotNum);
		//��Ϊ������Դ֮�⻹����������ֵҪ���и��£�Ҫô����Щ����ֵ���з�ɢ���£�Ҫô����ͳһ�����н��з��ű���ؽ��и���
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
		if (!s_Mesh->ShaderName.compare("TextShader"))
		{
			s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("DefaultColor", s_Mesh->MatrialData.GetMeshColor());
		}

		//���SceneLights���ϲ�Ϊ�վͽ������Ը���
		if (!s_Mesh->ShaderName.compare("BlinnPhongShader"))
		{
			//ֻ�������ж����Դ�Ļ����ϲ��ܽ��и���
			if (SceneLightController::GetSceneLights().size() > 0)
			{
				UpDateBlinnPhongUniformPropertices();

			}
		}
		if (!s_Mesh->ShaderName.compare("PhysicalBaseRenderShader")) 
		{
			if (SceneLightController::GetSceneLights().size() > 0) 
			{
				UpDatePhysicalBaseRenderUniformPropertices();
			}
		}

		//��ô�����÷�����ģʽ���в�ͬShader֮������Ը���  //���ݵ�ǰ��Shader������[�Լ����ڲ�ά���ð�,�ڼܹ�û���֮ǰֻ������]
		if (!s_Mesh->ShaderName.compare("FlectOrFractShader"))
		{
			//����������ɫ������
			s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateInt("flag", s_Mesh->ReflectFlag);
		}
	}

	void MeshRenderer::LoadTexture2DOrCube()
	{
		//��TextureUniform�±�
		int T2D_size = s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2DLibraries().size();
		int TCube_size = s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTextureCubeLibraries().size();
		if (T2D_size == 1)
		{
			s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2D("DefaultTexture")->Bind();
		}
		else
		{
			std::vector<std::string> T_name;
			for (auto& t : s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2DLibraries())
			{
				T_name.push_back(t.first);
			}
			uint32_t textureIndex = 0;
			for (int i = 0; i < T2D_size; ++i)
			{
				s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTexture2D(T_name[i])->Bind(textureIndex++);
			}

			T_name.clear();
			//��Cube���и���
			for (auto& t : s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTextureCubeLibraries())
			{
				T_name.push_back(t.first);
			}
			for (int i = 0; i < TCube_size; ++i)
			{
				s_Mesh->MatrialData.GetTextureList(s_Mesh->ShaderName).GetTextureCube(T_name[i])->Bind(textureIndex++);
			}
			textureIndex = 0;
		}
	}

	void MeshRenderer::UpdateUniversalUniformPropertices(const glm::mat4& transform)
	{
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();

		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_Model", transform);
	}

	void MeshRenderer::UpDateBlinnPhongUniformPropertices()
	{
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("DefaultColor", s_Mesh->MatrialData.GetMeshColor());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateMat4("u_LightProjection", Shadow::GetInstance()->GetLightSpaceMatrix());
		//��������ֵ[ֻ����Light���]
		auto lights = SceneLightController::GetSceneLights();
		//��¼ ��;� ��Դ�������±�
		uint32_t pointIndex = 0;
		uint32_t spotIndex = 0;
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
				//ͬ�����Խ���ǿת
				//����ô��֤�����еĹ�Դ˳���Shader�еĹ�Դ˳���Ƕ�Ӧ�ģ�[������ʱ�±����ȥ��֤�ͼ����е��±�˳��һ��]
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Point[" + std::to_string(pointIndex) + "].Position", light->GetLightPosition());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Point[" + std::to_string(pointIndex) + "].LightColor", light->GetLightColor());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Point[" + std::to_string(pointIndex) + "].DiffuseColor", light->GetLightDiffuse());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Point[" + std::to_string(pointIndex) + "].SpecularColor", light->GetLightSpecular());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Point[" + std::to_string(pointIndex) + "].Shininess", light->GetLightShininess());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Point[" + std::to_string(pointIndex) + "].Constant", dynamic_cast<PointLight*>(light.get())->GetLightConstant());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Point[" + std::to_string(pointIndex) + "].Linear", dynamic_cast<PointLight*>(light.get())->GetLightLinear());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Point[" + std::to_string(pointIndex) + "].Quadratic", dynamic_cast<PointLight*>(light.get())->GetLightQuadratic());
				pointIndex++;
			}
			else if (light->GetLightType() == LightType::Spot)
			{
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Spot[" + std::to_string(spotIndex) + "].Position", light->GetLightPosition());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Spot[" + std::to_string(spotIndex) + "].Rotation", light->GetLightRotatetion());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Spot[" + std::to_string(spotIndex) + "].LightColor", light->GetLightColor());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Spot[" + std::to_string(spotIndex) + "].DiffuseColor", light->GetLightDiffuse());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Spot[" + std::to_string(spotIndex) + "].SpecularColor", light->GetLightSpecular());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].Shininess", light->GetLightShininess());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].Constant", dynamic_cast<SpotLight*>(light.get())->GetLightConstant());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].Linear", dynamic_cast<SpotLight*>(light.get())->GetLightLinear());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].Quadratic", dynamic_cast<SpotLight*>(light.get())->GetLightQuadratic());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].CutOff", glm::cos(glm::radians(dynamic_cast<SpotLight*>(light.get())->GetLightCutOff())));
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].OuterCutOff", glm::cos(glm::radians(dynamic_cast<SpotLight*>(light.get())->GetLightOuterCutOff())));
				spotIndex++;
			}
		}
		//��������flag������Ϊ0//
		/*
		* BUG:��Ϊ���Ǹ�ȫ�ֱ�����ֻҪ��һ�����£����ͻ�ر��������ͨ�����Ӷ��޷�������һ��ʵ��ĸ���
		* �޸�:����������һֱ���£������޸ı�־ֵΪ����ֲ�
		*/
		//SceneLightController::ChangeFlag = 0;
	}

	void MeshRenderer::UpDatePhysicalBaseRenderUniformPropertices()
	{
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("DefaultColor", s_Mesh->MatrialData.GetMeshColor());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateMat4("u_LightProjection", Shadow::GetInstance()->GetLightSpaceMatrix());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("u_Metallic", s_Mesh->MatrialData.GetMetallic());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("u_Roughness", s_Mesh->MatrialData.GetRoughness());
		//��������ֵ[ֻ����Light���]
		auto lights = SceneLightController::GetSceneLights();
		//��¼ ��;� ��Դ�������±�
		uint32_t pointIndex = 0;
		uint32_t spotIndex = 0;
		for (auto light : lights)
		{
			//���ݹ�Դ���͸��²�ͬ������ֵ
			if (light->GetLightType() == LightType::Direction)
			{
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Direction.Rotatiion", light->GetLightRotatetion());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Direction.Position", light->GetLightPosition());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Direction.LightColor", light->GetLightColor());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Direction.Ambient", light->GetLightAmbient());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Direction.Diffuse", light->GetLightDiffuse());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Direction.Intensity", SceneLightController::GetEnvironmentLightIntensity());
			}
			else if (light->GetLightType() == LightType::Point)
			{
				//ͬ�����Խ���ǿת
				//����ô��֤�����еĹ�Դ˳���Shader�еĹ�Դ˳���Ƕ�Ӧ�ģ�[������ʱ�±����ȥ��֤�ͼ����е��±�˳��һ��]
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Point[" + std::to_string(pointIndex) + "].Position", light->GetLightPosition());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Point[" + std::to_string(pointIndex) + "].LightColor", light->GetLightColor());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Point[" + std::to_string(pointIndex) + "].DiffuseColor", light->GetLightDiffuse());
				pointIndex++;
			}
			else if (light->GetLightType() == LightType::Spot)
			{
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Spot[" + std::to_string(spotIndex) + "].Position", light->GetLightPosition());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec3("Spot[" + std::to_string(spotIndex) + "].Rotation", light->GetLightRotatetion());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Spot[" + std::to_string(spotIndex) + "].LightColor", light->GetLightColor());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("Spot[" + std::to_string(spotIndex) + "].DiffuseColor", light->GetLightDiffuse());
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].CutOff", glm::cos(glm::radians(dynamic_cast<SpotLight*>(light.get())->GetLightCutOff())));
				s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("Spot[" + std::to_string(spotIndex) + "].OuterCutOff", glm::cos(glm::radians(dynamic_cast<SpotLight*>(light.get())->GetLightOuterCutOff())));
				spotIndex++;
			}
		}
	}
}
