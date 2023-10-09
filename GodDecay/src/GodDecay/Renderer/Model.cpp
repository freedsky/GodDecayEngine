#include "gdpch.h"
#include "Model.h"

namespace GodDecay 
{
	Model::Model(const std::string& path)
	{
		m_ModelData = CreateRef<RendererModelStorage>();
		loadModel(path);
		BindModelData();
	}

	void Model::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			GD_ENGINE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}
		//得到模型文件所在位置，也是纹理加载的位置[后期可以自己把纹理文件位置进行调整]
		directory = path.substr(0, path.find_last_of('/'));
		//GD_ENGINE_INFO(directory);
		//从根节点出发递归处理每个子节点数据
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i) 
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i) 
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<ModelData> data;
		std::vector<uint32_t> indices;
		std::vector<ModelTexture> textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i) 
		{
			ModelData temp_data;
			//顶点
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			temp_data.Position = position;
			//法线
			if (mesh->HasNormals()) 
			{
				glm::vec3 normal;
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				temp_data.Normal = normal;
			}
			//textureUV
			if (mesh->mTextureCoords[0]) 
			{
				glm::vec2 texcoords;
				texcoords.x = mesh->mTextureCoords[0][i].x;
				texcoords.y = mesh->mTextureCoords[0][i].y;
				temp_data.TextureCoords = texcoords;
			}
			else
			{
				temp_data.TextureCoords = glm::vec2(0.0f, 0.0f);
			}
			data.push_back(temp_data);
		}
		//导入index数据
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i) 
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t k = 0; k < face.mNumIndices; ++k)
				indices.push_back(face.mIndices[k]);
		}

		//材质索引==========================================================================================================
		if (mesh->mMaterialIndex >= 0) 
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			//漫反射贴图
			std::vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			//高光反射贴图
			std::vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//法线贴图
			std::vector<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}

		//==========================================================================================================
		return Mesh(data, indices, textures);

	}

	std::vector<ModelTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<ModelTexture> textures;
		for (int i = 0; i < mat->GetTextureCount(type); ++i) 
		{
			//对应类型的纹理的文件名称
			aiString str;
			mat->GetTexture(type, i, &str);
			ModelTexture texture;
			texture.id = m_ModelData->m_WhiteTexture->LoadModelFileTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
		}
		return textures;
	}

	void Model::BindModelData()
	{
		std::vector<float> modelData;
		std::vector<uint32_t> modelIndices;
		for (int i = 0; i < m_Meshes.size(); ++i) 
		{
			for (int k = 0; k < m_Meshes[i].GetModelData().size(); ++k) 
			{
				
				modelData.push_back(m_Meshes[i].GetModelData()[k].Position.x);
				modelData.push_back(m_Meshes[i].GetModelData()[k].Position.y);
				modelData.push_back(m_Meshes[i].GetModelData()[k].Position.z);

				modelData.push_back(m_Meshes[i].GetModelData()[k].Normal.x);
				modelData.push_back(m_Meshes[i].GetModelData()[k].Normal.y);
				modelData.push_back(m_Meshes[i].GetModelData()[k].Normal.z);
				
				modelData.push_back(m_Meshes[i].GetModelData()[k].TextureCoords.x);
				modelData.push_back(m_Meshes[i].GetModelData()[k].TextureCoords.y);
				
			}

			for (int j = 0; j < m_Meshes[i].GetModelIndices().size(); ++j)
			{
				modelIndices.push_back(m_Meshes[i].GetModelIndices()[j]);
			}
			m_ModelData->ModelVertexArray.push_back(VertexArrayBuffer::Create());

			Ref<VertexBuffer> modelVBO = VertexBuffer::Create(modelData.data(), (uint32_t)modelData.size() * sizeof(float));
			modelVBO->Setlayout({
				{ShaderDataType::Float3,"a_Position"},
				{ShaderDataType::Float3,"a_Normal"},
				{ShaderDataType::Float2,"a_Coords"}
			});

			m_ModelData->ModelVertexArray[i]->AddVertexBuffer(modelVBO);

			Ref<IndexBuffer> modelEBO = IndexBuffer::Create(modelIndices.data(), (uint32_t)modelIndices.size() * sizeof(uint32_t));
			m_ModelData->ModelVertexArray[i]->SetIndexBuffer(modelEBO);

			modelData.clear();
			modelIndices.clear();
		}

		//============================================================================
		m_ModelData->m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_ModelData->m_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_ModelData->ModelShader = Shader::Create("assets/shader/StandardModelShader.glsl");

		if (m_Meshes.size() == 1) 
		{
			if (m_Meshes[0].GetModelTexture().size() == 0) 
			{
				m_ModelData->ModelShader->Bind();
				m_ModelData->ModelShader->SetInt("u_Texture", 0);
			}
		}

		//============================================================================
	} 
}
