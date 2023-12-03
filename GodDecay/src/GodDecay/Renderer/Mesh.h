#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GodDecay 
{
	//通用的顶点属性结构体
	struct MeshProperty
	{
		glm::vec3 Position;//顶点
		glm::vec3 Normal;//法线
		glm::vec3 Tangent;//切线，没有就计算出来
		glm::vec2 TexCoord;//纹理坐标，没有就默认为0就行
		int EntityID;//默认为-1，在组件渲染是去添加
	};

	//mesh种类的枚举
	enum class BaseMeshType //加上class的eunm是带作用域的，不加则不通过加::限定符也可以进行访问
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
		//每次重新加载数据都要清除旧的数据，才能加载新的数据
		void ClearMeshData();
		//数据在内部或者在外部进行加载
		void Init(BaseMeshType type, std::string path = "");
	private:
		void LoadCubeMesh();
		void LoadCircleMesh();
		//计算每个三角形的切线向量
		glm::vec3 Tangent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);

		void LoadModelMesh(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		//考虑到大量的mesh数据，开辟到堆中
		std::vector<MeshProperty> m_MeshData;
		std::vector<uint32_t> m_MeshIndices;

		BaseMeshType m_MeshType;
		std::string m_ModelPath;
	};
}



