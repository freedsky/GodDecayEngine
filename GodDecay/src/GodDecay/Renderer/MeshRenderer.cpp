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
		//这里可以考虑对matrial进行初始化，比如设置默认的Shader和Texture
		s_Mesh->ShaderName = "DefaultShader";//对象Shader的默认设置
		//一些其他变量的赋值
		s_Mesh->ReflectFlag = 1;
		//初始化除默认外的Shader，为切换Shader数据做准备【Test】
		s_Mesh->MatrialData.LoadShaderToRenderModel("sda");
	}

	void MeshRenderer::LoadMesh(Mesh& mesh, int entityId)
	{
		//先初始化mesh
		s_Mesh->MeshData = mesh;
		if (s_Mesh->MeshVertexArray != nullptr) 
		{
			s_Mesh->MeshVertexArray = nullptr;
			s_Mesh->MeshVertexArray = VertexArrayBuffer::Create();
		}
			

		//把id添加进入
		for (int i = 0; i < s_Mesh->MeshData.GetMeshData().size(); ++i)
		{
			s_Mesh->MeshData.GetMeshData()[i].EntityID = entityId;
		}

		//顶点属性大多相同，顶多是有多余的顶点属性不使用会占用一部分带宽，所以这一部分暂时不做处理
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
		//runtime中不会改变meshtype所以暂时不用去重新初始化mesh
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_ViewProjection", viewProj);
		//把摄像机的位置向量传递过去[但Camera运行相机没有Position属性需要自己实现后才能进行属性赋值]
	}

	void MeshRenderer::BeginDrawMesh(const EditorCamera& camera, BaseMeshType type, std::string path)
	{
		//第一次先把type先赋值过去，然后在此后每一次都要去检测，类型是否和前一次相等，不相等就要重新初始化
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
			s_Mesh->MeshData.ClearMeshData();//这里有考虑把模型数据进行清理
			s_Mesh->MeshData.Init((BaseMeshType)type, s_Mesh->MeshData.GetModelPath());
			LoadMesh(s_Mesh->MeshData, id);
			s_Mesh->type = type;
			s_Mesh->path = path;
			GD_ENGINE_INFO("Editor Chanage this model");
		}

		//暂时先把默认的渲染出来
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->Bind();
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetMat4("u_ViewProjection", camera.GetViewProjection());
		//把摄像机的位置向量传递过去
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetFloat3("u_ViewPosition", camera.GetPosition());
		//GD_ENGINE_TRACE("x = {0}, y = {1}, z = {2} ", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	}

	void MeshRenderer::DrawShadow(const Ref<Shader>& shadowShader, const glm::mat4& transform)
	{
		shadowShader->Bind();
		shadowShader->SetMat4("u_LightModel", transform);

		//因为是深度图，所以不需要任何其他变量或者属性直接绘制[生成深度图]
		s_Mesh->MeshVertexArray->Bind();
		if (s_Mesh->type == BaseMeshType::CUBE || s_Mesh->type == BaseMeshType::MODEL)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::Normal);
		else if (s_Mesh->type == BaseMeshType::CIRLE)
			RenderCommand::DrawIndexed(s_Mesh->MeshVertexArray, RendererAPI::DrawType::STRIP);

	}

	void MeshRenderer::DrawMesh(const glm::mat4& transform)
	{
		//通用属性值更新[基本上所有Shader都会拥有该属性]
		UpdateUniversalUniformPropertices(transform);

		//Shader特性属性值更新
		UpDateUniformPropertices();

		//绑定Uniform变量
		LoadUniformPropertices();

		//GD_ENGINE_TRACE("Diection = {0}, Point = {1}, Spot = {2} ", SceneLightController::DirectionNum, SceneLightController::PointNum, SceneLightController::SpotNum);

		//绑定纹理下标
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
			//只是把Shader名称给替换掉即可
			s_Mesh->ShaderName = ShaderName;
		}
			
	}

	//更新问题？
	void MeshRenderer::LoadUniformPropertices()
	{
		//先把集合中的光源数量进行更新
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("DirectionNumber", SceneLightController::DirectionNum);
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("PointNumber", SceneLightController::PointNum);
		s_Mesh->MatrialData.GetShaderList().Get(s_Mesh->ShaderName)->SetInt("SpotNumber", SceneLightController::SpotNum);
		//因为除开光源之外还有其他属性值要进行更新，要么把这些属性值进行分散更新，要么就在统一更新中进行分门别类地进行更新
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

		//如果SceneLights集合不为空就进行属性更新
		if (!s_Mesh->ShaderName.compare("BlinnPhongShader"))
		{
			//只能是在有定向光源的基础上才能进行更新
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

		//那么就利用方法的模式进行不同Shader之间的属性更新  //根据当前的Shader来更新[自己在内部维护好吧,在架构没想好之前只有这样]
		if (!s_Mesh->ShaderName.compare("FlectOrFractShader"))
		{
			//反射折射着色器更新
			s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateInt("flag", s_Mesh->ReflectFlag);
		}
	}

	void MeshRenderer::LoadTexture2DOrCube()
	{
		//绑定TextureUniform下标
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
			//对Cube进行更新
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
		//更新属性值[只考虑Light情况]
		auto lights = SceneLightController::GetSceneLights();
		//记录 点和聚 光源的数组下标
		uint32_t pointIndex = 0;
		uint32_t spotIndex = 0;
		for (auto light : lights) 
		{
			//根据光源类型更新不同的属性值
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
				//同样可以进行强转
				//但怎么保证集合中的光源顺序和Shader中的光源顺序是对应的？[设置临时下标变量去保证和集合中的下标顺序一致]
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
		//更新完后把flag重新置为0//
		/*
		* BUG:因为它是个全局变量，只要有一个更新，它就会关闭这个更新通道，从而无法进行下一次实体的更新
		* 修改:让它无条件一直更新，或者修改标志值为对象局部
		*/
		//SceneLightController::ChangeFlag = 0;
	}

	void MeshRenderer::UpDatePhysicalBaseRenderUniformPropertices()
	{
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateVec4("DefaultColor", s_Mesh->MatrialData.GetMeshColor());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateMat4("u_LightProjection", Shadow::GetInstance()->GetLightSpaceMatrix());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("u_Metallic", s_Mesh->MatrialData.GetMetallic());
		s_Mesh->MatrialData.GetUniformProperties(s_Mesh->ShaderName).UpdateFloat("u_Roughness", s_Mesh->MatrialData.GetRoughness());
		//更新属性值[只考虑Light情况]
		auto lights = SceneLightController::GetSceneLights();
		//记录 点和聚 光源的数组下标
		uint32_t pointIndex = 0;
		uint32_t spotIndex = 0;
		for (auto light : lights)
		{
			//根据光源类型更新不同的属性值
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
				//同样可以进行强转
				//但怎么保证集合中的光源顺序和Shader中的光源顺序是对应的？[设置临时下标变量去保证和集合中的下标顺序一致]
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
