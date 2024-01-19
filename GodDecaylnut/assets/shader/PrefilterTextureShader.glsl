#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;
out flat int v_EntityID;

void main()
{
	//��תY�ἴ��[����Ⱦ��պ�ʱ�ᱻ��ת������㲻�ٷ�ת]
	v_TexCoords = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position,1.0f);
	v_EntityID = a_EntityID;
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

in vec3 v_TexCoords;
in flat int v_EntityID;

//���ò�ͬ�Ĵֳ���������mipmap����
uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.14159265359;

//��������Ӧ���ǻ���mipmap�㼶�ʹֳ�����Ӱ���
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

//���ɵͲ�����
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}
//GGX��Ҫ�Բ���
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

void main()
{
	//�ѹ۲��߷������ΪWo[��Ϊ����]
	vec3 N = normalize(v_TexCoords);
	vec3 R = N;
	vec3 V = R;

	//������ƫ�ĵͲ�����ȥ���֣��õ����ٵ������Ľ���ֵ
	const uint SAMPLE_COUNT = 1024u;
	vec3 prefilteredColor = vec3(0.0f);
	float totalWeight = 0.0f;//Ȩ��ֵ

	for(uint i = 0u;i < SAMPLE_COUNT;++i)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		//��ͼ����о��[����]
		if(NdotL > 0.0f)
		{
			//��ֱ�Ӳ���������ͼ�����ǻ��ڻ��ֵ� PDF �ʹֲڶȲ���������ͼ�� mipmap ���Լ���α��
			//��ֹ�������Ĺ������[��Ϊ��ǰһֱ����ԭͼ���в��������ж��ڸ�Ƶϸ�ڹ�����������ֲ���ģ�����߻��־��Ȳ�����ȷ�����]
			//��ˣ��ڽ���mipmap�ֲ����ȥ���ֲ�����õ�����ֵ���Ӿ�׼�Ľ��
			float D = DistributionGGX(N, H, roughness);
			float NdotH = max(dot(N, H), 0.0);
			float HdotV = max(dot(H, V), 0.0);
			float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

			float resolution = 512.0; 
			float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
			//���Ͼ�����ʲô�㼶��ȥ���в������ֵĹ����Լ����

			prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

	prefilteredColor = prefilteredColor / totalWeight;

	color = vec4(prefilteredColor,1.0f);
	PixelID = v_EntityID;
}