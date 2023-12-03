#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GodDecay 
{
	//ͨ�õĶ������Խṹ��
	struct MeshProperty
	{
		glm::vec3 Position;//����
		glm::vec3 Normal;//����
		glm::vec3 Tangent;//���ߣ�û�оͼ������
		glm::vec2 TexCoord;//�������꣬û�о�Ĭ��Ϊ0����
		int EntityID;//Ĭ��Ϊ-1���������Ⱦ��ȥ���
	};

	//mesh�����ö��
	enum class BaseMeshType //����class��eunm�Ǵ�������ģ�������ͨ����::�޶���Ҳ���Խ��з���
	{
		CUBE = 0,
		CIRLE,
		MODEL
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh() = default;

		inline BaseMeshType GetMeshType() { return m_MeshType; }
		inline void SetMeshType(BaseMeshType type) { m_MeshType = type; }
		inline std::string GetModelPath() { return m_ModelPath; }
		inline void SetModelPath(std::string path) { m_ModelPath = path; }

		inline std::vector<MeshProperty>& GetMeshData() { return m_MeshData; }
		inline std::vector<uint32_t>& GetMeshIndices() { return m_MeshIndices; }
		//ÿ�����¼������ݶ�Ҫ����ɵ����ݣ����ܼ����µ�����
		void ClearMeshData();
		//�������ڲ��������ⲿ���м���
		void Init(BaseMeshType type, std::string path = "");
	private:
		void LoadCubeMesh();
		void LoadCircleMesh();
		//����ÿ�������ε���������
		glm::vec3 Tangent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);

		void LoadModelMesh(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		//���ǵ�������mesh���ݣ����ٵ�����
		std::vector<MeshProperty> m_MeshData;
		std::vector<uint32_t> m_MeshIndices;

		BaseMeshType m_MeshType;
		std::string m_ModelPath;
	};
}



