#pragma once
#include "RenderCommand.h"
#include "EditorCamera.h"
#include "Camera.h"

#include "Mesh.h"
#include "Matrial.h"

//#include <vector>

/// <summary>
/// MeshRenderer负责对Mesh中的数据进行顶点属性缓冲处理
/// 并在处理后为Draw进行准备
/// 
/// 它同时包含Matrial对象，Matrial对象具体包含了Shader和Texture列表来指定光照渲染模式
/// 
/// </summary>
namespace GodDecay 
{
	struct MeshRenderData
	{
		//用于处理的数据，meshdata通过引用传参赋值，matrial直接在内部创建
		Mesh MeshData;
		Matrial MatrialData;
		//顶点属性对象
		Ref<VertexArrayBuffer> MeshVertexArray;
		std::string ShaderName;

		BaseMeshType type;
		std::string path;
	};

	class MeshRenderer
	{
	public:
		//初始化顶点或者材质
		MeshRenderer();
		~MeshRenderer() = default;

		inline Ref<MeshRenderData>& GetMeshRendererData() { return s_Mesh; }

		//mesh数据加载,这里不在构造函数进行是因为后期可以随意进行进行数据的更改
		void LoadMesh(Mesh& mesh, int entityId);
		//绘制
		//实时相机
		void BeginDrawMesh(const Camera& camera, const glm::mat4& transform, BaseMeshType type);
		//编辑相机
		void BeginDrawMesh(const EditorCamera& camera, BaseMeshType type, std::string path);
		void DrawMesh(const glm::mat4& transform);
		void EndDrawMesh();
		//变更Shader的引用
		void ChanageShader(std::string ShaderName);
	private:
		Ref<MeshRenderData> s_Mesh;

		//加载Uniform
		void LoadUniformPropertices();
	};
}

