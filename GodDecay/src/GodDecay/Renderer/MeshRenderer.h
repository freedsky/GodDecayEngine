#pragma once
#include "RenderCommand.h"
#include "EditorCamera.h"
#include "Camera.h"

#include "Mesh.h"
#include "Matrial.h"

//#include <vector>

/// <summary>
/// MeshRenderer�����Mesh�е����ݽ��ж������Ի��崦��
/// ���ڴ����ΪDraw����׼��
/// 
/// ��ͬʱ����Matrial����Matrial������������Shader��Texture�б���ָ��������Ⱦģʽ
/// 
/// </summary>
namespace GodDecay 
{
	struct MeshRenderData
	{
		//���ڴ�������ݣ�meshdataͨ�����ô��θ�ֵ��matrialֱ�����ڲ�����
		Mesh MeshData;
		Matrial MatrialData;
		//�������Զ���
		Ref<VertexArrayBuffer> MeshVertexArray;
		std::string ShaderName;

		BaseMeshType type;
		std::string path;
	};

	class MeshRenderer
	{
	public:
		//��ʼ��������߲���
		MeshRenderer();
		~MeshRenderer() = default;

		inline Ref<MeshRenderData>& GetMeshRendererData() { return s_Mesh; }

		//mesh���ݼ���,���ﲻ�ڹ��캯����������Ϊ���ڿ���������н������ݵĸ���
		void LoadMesh(Mesh& mesh, int entityId);
		//����
		//ʵʱ���
		void BeginDrawMesh(const Camera& camera, const glm::mat4& transform, BaseMeshType type);
		//�༭���
		void BeginDrawMesh(const EditorCamera& camera, BaseMeshType type, std::string path);
		void DrawMesh(const glm::mat4& transform);
		void EndDrawMesh();
		//���Shader������
		void ChanageShader(std::string ShaderName);
	private:
		Ref<MeshRenderData> s_Mesh;

		//����Uniform
		void LoadUniformPropertices();
	};
}

