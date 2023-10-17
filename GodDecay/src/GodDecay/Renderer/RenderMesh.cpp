#include "gdpch.h"
#include "RenderMesh.h"

namespace GodDecay 
{
    float CubeVertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    uint32_t CubeIndices[] =
    {
        0,1,2,3,4,5,
        6,7,8,9,10,11,
        12,13,14,15,16,17,
        18,19,20,21,22,23,
        24,25,26,27,28,29,
        30,31,32,33,34,35,
    };

	//CubeBuffer=====================================================
	RenderCubeMesh::RenderCubeMesh()
	{
        Init();
	}

	RenderCubeMesh::~RenderCubeMesh()
	{

	}

	void RenderCubeMesh::Init()
	{
        int vertexSize = (uint32_t)(sizeof(CubeVertices) / sizeof(float));
        
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < vertexSize;)  
        {
            //顶点
            glm::vec3 position;
            position.x = CubeVertices[i++];
            position.y = CubeVertices[i++];
            position.z = CubeVertices[i++];
            //法线
            glm::vec3 normal;
            normal.x = CubeVertices[i++];
            normal.y = CubeVertices[i++];
            normal.z = CubeVertices[i++];
            //纹理坐标
            glm::vec2 coords;
            coords.x = CubeVertices[i++];
            coords.y = CubeVertices[i++];
            //颜色
            GetVertexProperties().push_back(VertexProperties(position, normal, color, coords));
        }
        //计算切线
        for (int i = 0; i < GetVertexProperties().size();i+=3) 
        {
            glm::vec3 tangent = GetTangent(GetVertexProperties()[i].Position, GetVertexProperties()[i + 1].Position, GetVertexProperties()[i + 2].Position,
                GetVertexProperties()[i].TexCoord, GetVertexProperties()[i + 1].TexCoord, GetVertexProperties()[i + 2].TexCoord);
            for (int j = 0; j < 3; ++j) 
            {
                GetVertexProperties()[i + j].Tangent = tangent;
            }
        }

        //存储下标索引值
        int indexSize = sizeof(CubeIndices) / sizeof(uint32_t);
        for (int i = 0; i < indexSize; ++i) 
        {
            GetIndices().push_back(CubeIndices[i]);
        }
	}

    glm::vec3 RenderCubeMesh::GetTangent(glm::vec3 position1, glm::vec3 position2, glm::vec3 position3, glm::vec2 texcoord1, glm::vec2 texcoord2, glm::vec2 texcoord3)
    {
        glm::vec3 edge1 = position2 - position1;
        glm::vec3 edge2 = position3 - position1;
        glm::vec2 deltaUV1 = texcoord2 - texcoord1;
        glm::vec2 deltaUV2 = texcoord3 - texcoord1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        return tangent;
    }

    //Cirle=================================================================================================================

    RenderCirleMesh::RenderCirleMesh()
    {
        Init();
    }

    RenderCirleMesh::~RenderCirleMesh()
    {
    }

    void RenderCirleMesh::Init()
    {
        //数据的创建
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        for (uint32_t x = 0; x <= X_SEGMENTS; ++x)
        {
        	for (uint32_t y = 0; y <= Y_SEGMENTS; ++y)
        	{
        		float xSegment = (float)x / (float)X_SEGMENTS;
        		float ySegment = (float)y / (float)Y_SEGMENTS;
        		float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
        		float yPos = std::cos(ySegment * PI);
        		float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

        		positions.push_back(glm::vec3(xPos, yPos, zPos));
        		uv.push_back(glm::vec2(xSegment, ySegment));
        		normals.push_back(glm::vec3(xPos, yPos, zPos));            
        	}
        }
        //数据的加载
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < positions.size(); ++i) 
        {
            GetVertexProperties().push_back(VertexProperties(positions[i], normals[i], color, uv[i]));
        }
        //计算切线
        for (int i = 0; i < GetVertexProperties().size() - 2; ++i)
        {
            glm::vec3 tangent = GetTangent(GetVertexProperties()[i].Position, GetVertexProperties()[i + 1].Position, GetVertexProperties()[i + 2].Position,
                GetVertexProperties()[i].TexCoord, GetVertexProperties()[i + 1].TexCoord, GetVertexProperties()[i + 2].TexCoord);
            GetVertexProperties()[i].Tangent = tangent;
            
        }
        //存储顶点下标
        std::vector<uint32_t> cirleIndices;
        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
        	if (!oddRow)
        	{
        		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        		{
        			cirleIndices.push_back(y * (X_SEGMENTS + 1) + x);
        			cirleIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        		}
        	}
        	else
        	{
        		for (int x = X_SEGMENTS; x >= 0; --x)
        		{
        			cirleIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
        			cirleIndices.push_back(y * (X_SEGMENTS + 1) + x);
        		}
        	}
        	oddRow = !oddRow;
        }
        for (int i = 0; i < cirleIndices.size(); ++i) 
        {
            GetIndices().push_back(cirleIndices[i]);
        }
    }

    glm::vec3 RenderCirleMesh::GetTangent(glm::vec3 position1, glm::vec3 position2, glm::vec3 position3, glm::vec2 texcoord1, glm::vec2 texcoord2, glm::vec2 texcoord3)
    {
        glm::vec3 edge1 = position2 - position1;
        glm::vec3 edge2 = position3 - position1;
        glm::vec2 deltaUV1 = texcoord2 - texcoord1;
        glm::vec2 deltaUV2 = texcoord3 - texcoord1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        return tangent;
    }

}
