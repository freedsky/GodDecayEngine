#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "VertexArrayBuffer.h"

#include "Texture.h"

/// <summary>
/// ģ�͵����ݼ���
/// 
/// ���ڶ�mesh�����������Ҫ�������չ֧�֣�������ʱ�ȸ��ö�mesh��������أ�ֻ���ǵ�mesh���������
/// </summary>

namespace GodDecay
{
	struct RendererModelStorage 
	{
		std::vector<Ref<VertexArrayBuffer>> ModelVertexArray;
		Ref<Shader> ModelShader;
		Ref<Texture2D> m_WhiteTexture;
	};

	class Model
	{
	public:
		Model() = default;
		Model(const std::string& path);
		~Model() = default;

		const Ref<RendererModelStorage> GetModelData() const { return m_ModelData; }
	private:
		std::vector<Mesh> m_Meshes;
		Ref<RendererModelStorage> m_ModelData;
		std::string directory;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		void BindModelData();
	};
}



