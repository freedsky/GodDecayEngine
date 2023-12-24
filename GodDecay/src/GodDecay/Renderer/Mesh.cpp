#include "gdpch.h"
#include "Mesh.h"

namespace GodDecay 
{
    //data===========================
    float CubeVertices[] = {
        // positions          // normals        // texture coords
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                       
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                   
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                       
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                       
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                       
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    const uint32_t X_SEGMENTS = 65;
    const uint32_t Y_SEGMENTS = 65;
    const float PI = 3.14159265359f;

    uint32_t MeshIndexCount = 0;
    uint32_t MeshIndexStep = 0;

    Mesh::Mesh()
        : m_MeshType(BaseMeshType::CUBE), m_ModelPath("assets/model/nanosuit/nanosuit.obj")
    {
    }
    //================================
    void Mesh::ClearMeshData()
    {
        if (m_MeshData.size() != 0 || m_MeshIndices.size() != 0) 
        {
            m_MeshData.clear();
            m_MeshIndices.clear();
        }
    }

    void Mesh::Init(BaseMeshType type, std::string path)
    {
        switch (type)
        {
        case GodDecay::BaseMeshType::CUBE:
        {
            LoadCubeMesh(); 
            SetMeshType(type);
            break;
        }    
        case GodDecay::BaseMeshType::CIRLE:
        {
            LoadCircleMesh();
            SetMeshType(type);
            break;
        }
        case GodDecay::BaseMeshType::MODEL:
        {
            if(path.compare(""))
                m_ModelPath = path;
            LoadModelMesh(m_ModelPath);
            SetMeshType(type);
            MeshIndexStep = 0;//加载完成后把这些辅助变量归0
            MeshIndexCount = 0;
            break;
        }
        default:
            GD_ENGINE_WARN("UnKnow This Mesh Type");
            break;
        }
    }

    void Mesh::LoadCubeMesh()
    {
        uint32_t size = sizeof(CubeVertices) / sizeof(float);
        for (uint32_t i = 0; i < size; i += 8)
        {
            MeshProperty data;
            //顶点
            data.Position.x = CubeVertices[0 + i];
            data.Position.y = CubeVertices[1 + i];
            data.Position.z = CubeVertices[2 + i];
            //法线
            data.Normal.x = CubeVertices[3 + i];
            data.Normal.y = CubeVertices[4 + i];
            data.Normal.z = CubeVertices[5 + i];
            //纹理坐标
            data.TexCoord.x = CubeVertices[6 + i];
            data.TexCoord.y = CubeVertices[7 + i];
            //id
            data.EntityID = -1;
            m_MeshData.push_back(data);
        }
        //下标
        for (uint32_t i = 0; i < 36; ++i) 
        {
            m_MeshIndices.push_back(i);
        }
        //计算每个三角形的切线
        for (uint32_t i = 0; i < m_MeshData.size(); i += 3)
        {
            glm::vec3 t = Tangent(m_MeshData[0 + i].Position, m_MeshData[1 + i].Position, m_MeshData[2 + i].Position,
                m_MeshData[0 + i].TexCoord, m_MeshData[1 + i].TexCoord, m_MeshData[2 + i].TexCoord);
            for (uint32_t j = 0; j < 3; ++j) 
            {
                m_MeshData[i + j].Tangent = t;
            }
        }
    }

    void Mesh::LoadCircleMesh()
    {
        //顶点属性
        for (uint32_t x = 0; x <= X_SEGMENTS; ++x)
        {
            for (uint32_t y = 0; y <= Y_SEGMENTS; ++y)
            {
                MeshProperty data;
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                data.Position = glm::vec3(xPos, yPos, zPos);
                data.TexCoord = glm::vec2(xSegment, ySegment);
                data.Normal = glm::vec3(xPos, yPos, zPos);
                data.EntityID = -1;
                m_MeshData.push_back(data);
            }
        }
        //圆的下标存疑
        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow)
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    m_MeshIndices.push_back(y * (X_SEGMENTS + 1) + x);
                    m_MeshIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    m_MeshIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    m_MeshIndices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        //圆的切线向量？
        for (uint32_t i = 0; i < m_MeshData.size(); i += 3)
        {
            glm::vec3 t = Tangent(m_MeshData[0 + i].Position, m_MeshData[1 + i].Position, m_MeshData[2 + i].Position,
                m_MeshData[0 + i].TexCoord, m_MeshData[1 + i].TexCoord, m_MeshData[2 + i].TexCoord);
            for (uint32_t j = 0; j < 3; ++j)
            {
                m_MeshData[i + j].Tangent = t;
            }
        }
    }

    glm::vec3 Mesh::Tangent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
    {
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        //暂时不进行normal化
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        return tangent;
    }

    void Mesh::LoadModelMesh(std::string path)
    {
        if (!path.compare("")) 
        {
            GD_ENGINE_DEBUG("Path Is Empty");
            return;
        }
        //加载模型文件
        Assimp::Importer importer;
        /*
        * aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
        * aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用
        * aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化
        */
        const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //检测模型文件是否被正确加载
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            GD_ENGINE_ERROR("This Model File Load failed");
            return;
        }
        //开始处理每个节点下的mesh数据
        ProcessNode(scene->mRootNode, scene);
    }

    //对于模型文件mesh以及节点的处理
    void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
    {
        //从根节点开始循环处理加载这些节点
        for (uint32_t i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);//处理单个mesh[对于多mesh的mdoel来说]
        }
        //如果它有子节点便循环处理它的子节点
        for (uint32_t i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    //加载节点下的mesh数据
    void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i) 
        {
            MeshProperty data;
            //顶点
            data.Position.x = mesh->mVertices[i].x;
            data.Position.y = mesh->mVertices[i].y;
            data.Position.z = mesh->mVertices[i].z;
            //法线
            if (mesh->HasNormals()) 
            {
                data.Normal.x = mesh->mNormals[i].x;
                data.Normal.y = mesh->mNormals[i].y;
                data.Normal.z = mesh->mNormals[i].z;
            }
            else 
            {
                data.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            //纹理
            if (mesh->mTextureCoords[0]) 
            {
                data.TexCoord.x = mesh->mTextureCoords[0][i].x;
                data.TexCoord.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                data.TexCoord = glm::vec2(0.0f, 0.0f);
            }
            //切线
            if (mesh->HasTangentsAndBitangents()) 
            {
                data.Tangent.x = mesh->mTangents[i].x;
                data.Tangent.y = mesh->mTangents[i].y;
                data.Tangent.x = mesh->mTangents[i].z;
            }
            else
            {
                data.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            //id
            data.EntityID = -1;
            m_MeshData.push_back(data);
        }
        //顶点索引
        /*
        * Bug：
        * 描述：多Mesh的模型的顶点索引会在新的mesh基础上从下标0开始，
        * 所以绘制时才会显示不完全
        * 
        */
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; ++j)
            {
                m_MeshIndices.push_back(face.mIndices[j] + MeshIndexStep);
                MeshIndexCount++;
            }    
        }
        MeshIndexStep = MeshIndexCount;
    }
}
