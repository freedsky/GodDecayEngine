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
            MeshIndexStep = 0;//������ɺ����Щ����������0
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
            //����
            data.Position.x = CubeVertices[0 + i];
            data.Position.y = CubeVertices[1 + i];
            data.Position.z = CubeVertices[2 + i];
            //����
            data.Normal.x = CubeVertices[3 + i];
            data.Normal.y = CubeVertices[4 + i];
            data.Normal.z = CubeVertices[5 + i];
            //��������
            data.TexCoord.x = CubeVertices[6 + i];
            data.TexCoord.y = CubeVertices[7 + i];
            //id
            data.EntityID = -1;
            m_MeshData.push_back(data);
        }
        //�±�
        for (uint32_t i = 0; i < 36; ++i) 
        {
            m_MeshIndices.push_back(i);
        }
        //����ÿ�������ε�����
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
        //��������
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
        //Բ���±����
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
        //Բ������������
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
        //��ʱ������normal��
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
        //����ģ���ļ�
        Assimp::Importer importer;
        /*
        * aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
        * aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ�����
        * aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż�
        */
        const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //���ģ���ļ��Ƿ���ȷ����
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            GD_ENGINE_ERROR("This Model File Load failed");
            return;
        }
        //��ʼ����ÿ���ڵ��µ�mesh����
        ProcessNode(scene->mRootNode, scene);
    }

    //����ģ���ļ�mesh�Լ��ڵ�Ĵ���
    void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
    {
        //�Ӹ��ڵ㿪ʼѭ�����������Щ�ڵ�
        for (uint32_t i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);//������mesh[���ڶ�mesh��mdoel��˵]
        }
        //��������ӽڵ��ѭ�����������ӽڵ�
        for (uint32_t i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    //���ؽڵ��µ�mesh����
    void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i) 
        {
            MeshProperty data;
            //����
            data.Position.x = mesh->mVertices[i].x;
            data.Position.y = mesh->mVertices[i].y;
            data.Position.z = mesh->mVertices[i].z;
            //����
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
            //����
            if (mesh->mTextureCoords[0]) 
            {
                data.TexCoord.x = mesh->mTextureCoords[0][i].x;
                data.TexCoord.y = mesh->mTextureCoords[0][i].y;
            }
            else
            {
                data.TexCoord = glm::vec2(0.0f, 0.0f);
            }
            //����
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
        //��������
        /*
        * Bug��
        * ��������Mesh��ģ�͵Ķ������������µ�mesh�����ϴ��±�0��ʼ��
        * ���Ի���ʱ�Ż���ʾ����ȫ
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
