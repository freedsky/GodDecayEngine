#type vertex
#version 450 core

//Physical Base Render��Blinn-Phong���Ǽ���ֱ�ӹ�Դ�ģ����г��㷨��ͬ����������һ��

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_Texcoords;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
//��Ϊ�����ͼ���ڹ�ռ����ɵģ������ڵ�ǰ����ռ��У�Ҫ�Ѷ���Ҳͬ���任����ռ��£��������������ʱ�������ֵ��ת�������������£��ٴ洢
uniform mat4 u_LightProjection;

out flat int v_EntityID;
out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_WorldPos;
out vec4 v_FragPosLightSpace;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	v_TexCoords = a_Texcoords;
	v_EntityID = a_EntityID;
	//bulin phong----------------------
	//ת��Ϊ��������
	v_WorldPos = vec3(u_Model * vec4(a_Position,1.0f));
	//�÷��߲����б��Ӱ��
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	//����ת��Ϊ��ռ��µ�����
	v_FragPosLightSpace = u_LightProjection * vec4(v_WorldPos,1.0f);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

in flat int v_EntityID;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_WorldPos;
in vec4 v_FragPosLightSpace;

//��Դ������
uniform int DirectionNumber;
uniform int PointNumber;
uniform int SpotNumber;

//������ʱ����Ҫ������ͼ
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
//�������
uniform sampler2D DepthShadowTexture;
//��ӻ��������������ն�ͼ
uniform samplerCube IrradianceTexture;
//�߹�Ԥ��������
uniform samplerCube MipmapPrefilterTextureCube;
uniform sampler2D BRDFLookUpTableTexture;

uniform vec3 u_ViewPosition;
uniform vec4 DefaultColor;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Ao;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;//mip�ȼ�

//PBR�ڲ��Ĺ���˥���Ǿ���ƽ���ķ��ȣ�������ﲻ��Ҫ˥���Ĳ���[��ȻҲ��������֮ǰ��˥���㷨]
//direction
//������λ����λ�ã������Ƿ���[��Ϊ�ṹ�壬���㴦��]
struct DirectionLight
{
	vec3 Rotatiion;
	vec3 Position;
	vec4 LightColor;
	vec4 Ambient;
	vec4 Diffuse;
	float Intensity;
};
uniform DirectionLight Direction;

//���Դ
struct PointLight
{
	vec3 Position;
	vec4 LightColor;
	vec4 DiffuseColor;
};
uniform PointLight Point[4];

//�۹�Դ
struct SpotLight
{
	vec3 Position;
	vec3 Rotation;
	vec4 LightColor;
	vec4 DiffuseColor;
	float CutOff;
	float OuterCutOff;
};
uniform SpotLight Spot[4];

//����DFG��---------------------------------------------------
//����������F F0Ϊ�����������ǵķ����ʣ���ͬ���ʷ����ʲ�ͬ����Ե��һ��Ϊ0.04
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	//ʹ��Fresnel-Schlick���Ʒ������������������ʡ�
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta,0.0,1.0),5.0);
}
//Ϊ�˺�ֱ�ӹ��յķ������������Ǻ�[��Ϊ������û����Ч�İ�������������ֲڶ�]
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

//���㷨�߷ֲ� D ���԰�������ͱ��淨�ߺʹֳ�����Ϊ����
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

	float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

	return num / denom;
}
//���㼸���ڱ� G ����Ҫ��Ϊ���ս���ʱ���ڱ��Լ�����ʱ���ڱ�
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
//�ѹ�����������Լ���ĳ��������ĵ�� Ȼ���Ǳ���ֳ�����Ϊ����
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

//---------------------------------------------------

vec3 DirectionLightColor(vec3 V, vec3 N, vec3 R, vec3 F0, float roughness, float shadowValue)
{
	vec3 finalDiffuse = vec3(0.0f);
	//L�ǹ�Դ�ķ�����������ͬ���͵Ĺ�Դ���㷽ʽҲ��һ��
	vec3 L = normalize(-Direction.Rotatiion);
	//����۲��߷��������͹�Դ�ķ��������İ���������Դ����жϱ���ķ��߷ֲ�
	vec3 H = normalize(V + L);
	//�þ��������Ź��յ�ǿ��[�����Դû�о��������]
	//���Wi����������ÿ�������յ���õ�Ĺ�������[Ҫ���Թ�������ǿ��]
	vec3 radiance = Direction.LightColor.rgb;
	//����DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//����Cook-Torrance BRDF��
	vec3 nominator = D * G * F;
	//��׼����
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = nominator / denominator;

	//����Ks �Լ� Kd �����ֹ���ֵ
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//��ΪKD���������䲿�֣������᾵�淴����ߣ��������ﻹҪ���Էǽ�����
	Kd *= 1.0f - u_Metallic;

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;
	
	//=====================================================================
	//�������յ�F��
	vec3 ambientF = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	//����KS�Լ�KD�ͬ��������Ҳ��Ϊ�����䲿�ֺͷ���߹ⲿ�֣�����Ҫ�÷������������������������ֵı�ֵ
	vec3 aKs = ambientF;
	vec3 aKd = 1.0 - aKs;
	aKd *= 1.0f - u_Metallic;
	//IBL����������
	vec3 irradiance = texture(IrradianceTexture,vec3(N.x,-N.y,N.z)).rgb;
	vec3 ambientDiffuse = DefaultColor.rgb * Direction.LightColor.rgb * irradiance;

	//����Ԥ�˲��Ļ���������������ͼ
	vec3 prefilteredColor = textureLod(MipmapPrefilterTextureCube, vec3(R.x,-R.y,R.z),  roughness * MAX_REFLECTION_LOD).rgb; 
	//����BRDF�Ļ��ֲ���ͼ
	vec2 envBRDF  = texture(BRDFLookUpTableTexture, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 ambientSpecular = prefilteredColor * (ambientF * envBRDF.x + envBRDF.y);

	//���ѻ����������[��ӹ��գ�ȫ�ֹ���][����Ļ���ǿ����ʼ������ȥ����Ϊ����������ͼ���ƣ����������Բ���д]
	vec3 finalAmbient = (ambientDiffuse * aKd + ambientSpecular) * u_Ao;
	//���ս�����ǻ������ֱ�ӹ���
	vec3 finalColor = finalAmbient + finalDiffuse;//���ܼ򵥵�ֱ����ˣ����ƻ������غ㡣

	return finalColor;
}

//������ӳ�����ڶ����Դ�¾��еģ����������Դ��������
vec3 PointLightColor(vec3 V, vec3 N, vec3 R, vec3 F0, float roughness, int index)
{
	vec3 finalDiffuse = vec3(0.0f);
	//L�ǹ�Դ�ķ�����������ͬ���͵Ĺ�Դ���㷽ʽҲ��һ��
	vec3 L = normalize(Point[index].Position - v_WorldPos);
	//����۲��߷��������͹�Դ�ķ��������İ���������Դ����жϱ���ķ��߷ֲ�
	vec3 H = normalize(V + L);
	//�þ��������Ź��յ�ǿ��
	float distance = length(Point[index].Position - v_WorldPos);
	//����ǿ��������վ����ƽ���ɷ���
	float attenuation = 1.0f / (distance * distance);
	//���Wi����������ÿ�������յ���õ�Ĺ�������
	vec3 radiance = Point[index].LightColor.rgb * attenuation;
	//����DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//����Cook-Torrance BRDF��
	vec3 nominator = D * G * F;
	//��׼����
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	vec3 specular = nominator / denominator;

	//����Ks �Լ� Kd �����ֹ���ֵ
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//��ΪKD���������䲿�֣������᾵�淴����ߣ��������ﻹҪ���Էǽ�����
	Kd *= (1.0f - u_Metallic);

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;

	//���ѻ����������[��ӹ��գ�ȫ�ֹ���][������ǰǿ���ܵ��Ƕ����ԴӰ��]//����������������ܻ�����Ӱ��
	vec3 finalAmbient = vec3(0.0f,0.0f,0.0f); //Direction.Intensity * DefaultColor.rgb * u_Ao;
	//���ս�����ǻ������ֱ�ӹ���
	vec3 finalColor = finalAmbient + finalDiffuse;

	return finalColor;
}

vec3 SpotLightColor(vec3 V, vec3 N, vec3 R, vec3 F0, float roughness, int index)
{
	vec3 finalDiffuse = vec3(0.0f);
	//L�ǹ�Դ�ķ�����������ͬ���͵Ĺ�Դ���㷽ʽҲ��һ��
	vec3 L = normalize(Spot[index].Position - v_WorldPos);
	//�õ��۹�����䷽λ�н�
	float theta = dot(L,normalize(Spot[index].Rotation));
	float epsilon = (Spot[index].CutOff - Spot[index].OuterCutOff);
	float intensity = clamp((theta - Spot[index].OuterCutOff) / epsilon, 0.0, 1.0);
	//����۲��߷��������͹�Դ�ķ��������İ���������Դ����жϱ���ķ��߷ֲ�
	vec3 H = normalize(V + L);
	//�þ��������Ź��յ�ǿ��
	float distance = length(Spot[index].Position - v_WorldPos);
	//����ǿ��������վ����ƽ���ɷ���
	float attenuation = 1.0f / (distance * distance);
	//���Wi����������ÿ�������յ���õ�Ĺ�������
	vec3 radiance = Spot[index].LightColor.rgb * attenuation;
	//����DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//����Cook-Torrance BRDF��
	vec3 nominator = D * G * F;
	//��׼����
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	vec3 specular = nominator / denominator;

	//����Ks �Լ� Kd �����ֹ���ֵ
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//��ΪKD���������䲿�֣������᾵�淴����ߣ��������ﻹҪ���Էǽ�����
	Kd *= (1.0f - u_Metallic);

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;

	//���ѻ����������[��ӹ��գ�ȫ�ֹ���]
	vec3 finalAmbient = vec3(0.0f,0.0f,0.0f);//vec3(0.03) * DefaultColor.rgb * u_Ao;
	//���ս�����ǻ������ֱ�ӹ���
	vec3 finalColor = finalAmbient + (finalDiffuse * intensity);

	return finalColor;
}

//----------------

//Shadow����
float ShadowCalculation(vec4 fragPosLightSpace, vec3 vdir, vec3 ndir)
{
	//�õ���Դ�ķ�������
	vec3 ldir = normalize(Direction.Rotatiion);

	//��Ϊ���������ֵ���ڲü��ռ��µı�׼�豸����ϵ�µ�ֵ
	//�����Ƚ���͸�ӳ���������������û�е���͸��ͶӰ���Ǳ�Ҫ�ġ�
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//�ѵ�ǰֵ�任Ϊ[0,1]
	projCoords = projCoords * 0.5 + 0.5;
	//�����������
	float shadowMapDepth = texture(DepthShadowTexture, projCoords.xy).r;
	//��ǰ���ֵ
	float currentDepth = projCoords.z;
	//�Ե�ǰ������[���]����ƫ�ƣ���ֹ��Ӱʧ֡[���ݹ�������ļнǴ�С������ƫ�ƵĴ�С]
	float bias = max(0.05 * (1.0 - max(dot(ndir, ldir),0.0f)), 0.005);
	//Ȼ����бȽ�
	float shadow = currentDepth - bias > shadowMapDepth ? 1.0 : 0.0;

	//����ǰ�㳬���˹��������׶��Զƽ��ʱ������ͶӰ�����z�������1.0��
	//��������£�GL_CLAMP_TO_BORDER���Ʒ�ʽ��������,���Դ���1ֱ�ӷ���0����
	if(projCoords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

//----------------

//----------------

//��δ������Ӱ���㣬����ʱ��������Ӱ��Ⱦ��Ȼ�����⡣
void main()
{
	//����ͨ��
	vec3 N = v_Normal;
	vec3 V = normalize(u_ViewPosition - v_WorldPos);
	vec3 R = reflect(-V,N);
	vec3 FinalColor = vec3(0.0f);

	//���ݽ�����ȥ��ֵF0
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, DefaultColor.rgb, u_Metallic);

	float shadow = ShadowCalculation(v_FragPosLightSpace,V,N);

	//�Դֳ��Ƚ�������(���ֳ���Ϊ0ʱ��������Ч)
	float roughness = clamp(u_Roughness,0.01f,1.0f);

	if(DirectionNumber != 0)
	{
		FinalColor += DirectionLightColor(V,N,R,F0,roughness,shadow);
	}
	if(PointNumber != 0)
	{
		for(int i = 0;i < PointNumber;i++)
		{
			FinalColor += PointLightColor(V,N,R,F0,roughness,i);
		}
	}
	if(SpotNumber != 0)
	{
			for(int i = 0;i < SpotNumber;i++)
		{
			FinalColor += SpotLightColor(V,N,R,F0,roughness,i);
		}
	}

	//�����ϵ�׼ȷ��Ҫ�������Կռ��м��㣬��Ҫ��ʾ���豸�Ͼ�Ҫ����GAMMУ��
	//ͬʱ���Լ���ֵ�ᳬ��1�����Ҫ��GammУ��֮ǰ�Ȱѽ����LDRӳ��ΪHDR[ɫ��ӳ�䷽��ΪReinhard ����]
	FinalColor = FinalColor / (FinalColor + vec3(1.0));
	FinalColor = pow(FinalColor, vec3(1.0/2.2));

	//��ǰ��δȥ���������ͼ������PBR�����
	color = vec4(FinalColor,1.0f);
	PixelID = v_EntityID;
}