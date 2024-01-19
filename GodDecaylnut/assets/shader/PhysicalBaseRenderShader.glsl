#type vertex
#version 450 core

//Physical Base Render和Blinn-Phong都是计算直接光源的，所有除算法不同，其他基本一样

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_Texcoords;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
//因为深度贴图是在光空间生成的，所有在当前相机空间中，要把顶点也同样变换到光空间下，或者在深度生成时，把深度值先转换到世界坐标下，再存储
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
	//转换为世界坐标
	v_WorldPos = vec3(u_Model * vec4(a_Position,1.0f));
	//让法线不受切变的影响
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	//顶点转换为光空间下的坐标
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

//光源计数器
uniform int DirectionNumber;
uniform int PointNumber;
uniform int SpotNumber;

//这里暂时不需要采样贴图
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
//深度纹理
uniform sampler2D DepthShadowTexture;
//添加环境光的漫反射辐照度图
uniform samplerCube IrradianceTexture;
//高光预计算纹理
uniform samplerCube MipmapPrefilterTextureCube;
uniform sampler2D BRDFLookUpTableTexture;

uniform vec3 u_ViewPosition;
uniform vec4 DefaultColor;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Ao;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;//mip等级

//PBR内部的光照衰减是距离平方的反比，因此这里不需要衰减的参数[当然也可以沿用之前的衰减算法]
//direction
//定向光的位置是位置，方向是方向[变为结构体，方便处理]
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

//点光源
struct PointLight
{
	vec3 Position;
	vec4 LightColor;
	vec4 DiffuseColor;
};
uniform PointLight Point[4];

//聚光源
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

//计算DFG项---------------------------------------------------
//菲涅尔方程F F0为物体材质入射角的反射率，不同物质反射率不同。绝缘体一般为0.04
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	//使用Fresnel-Schlick近似法来计算菲涅尔项【反射率】
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta,0.0,1.0),5.0);
}
//为了和直接光照的菲涅尔计算相吻合[因为环境光没有有效的半向量来计算其粗糙度]
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

//计算法线分布 D 它以半程向量和表面法线和粗超度作为输入
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
//计算几何遮蔽 G 它主要分为光照进入时的遮蔽以及出射时的遮蔽
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
//把光的入射向量以及光的出射向量的点乘 然后是表面粗超度作为输入
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
	//L是光源的反向向量，不同类型的光源计算方式也不一样
	vec3 L = normalize(-Direction.Rotatiion);
	//计算观察者方向向量和光源的方向向量的半程向量，以此来判断表面的法线分布
	vec3 H = normalize(V + L);
	//该距离来缩放光照的强度[定向光源没有距离的缩放]
	//光从Wi方向进入表面每个点最终到达该点的光照能量[要乘以光照整体强度]
	vec3 radiance = Direction.LightColor.rgb;
	//计算DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//计算Cook-Torrance BRDF项
	vec3 nominator = D * G * F;
	//标准化项
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = nominator / denominator;

	//计算Ks 以及 Kd 各部分贡献值
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//因为KD属于漫反射部分，它不会镜面反射光线，所以这里还要乘以非金属量
	Kd *= 1.0f - u_Metallic;

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;
	
	//=====================================================================
	//环境光照的F项
	vec3 ambientF = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	//计算KS以及KD项，同样环境光也分为漫反射部分和反射高光部分，所以要用菲涅尔方程来计算这两个部分的比值
	vec3 aKs = ambientF;
	vec3 aKd = 1.0 - aKs;
	aKd *= 1.0f - u_Metallic;
	//IBL的漫反射项
	vec3 irradiance = texture(IrradianceTexture,vec3(N.x,-N.y,N.z)).rgb;
	vec3 ambientDiffuse = DefaultColor.rgb * Direction.LightColor.rgb * irradiance;

	//计算预滤波的环境光照立方体贴图
	vec3 prefilteredColor = textureLod(MipmapPrefilterTextureCube, vec3(R.x,-R.y,R.z),  roughness * MAX_REFLECTION_LOD).rgb; 
	//采样BRDF的积分查找图
	vec2 envBRDF  = texture(BRDFLookUpTableTexture, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 ambientSpecular = prefilteredColor * (ambientF * envBRDF.x + envBRDF.y);

	//最后把环境光项加上[间接光照，全局光照][这里的环境强度起始可以舍去，因为环境光由贴图控制，因此这里可以不用写]
	vec3 finalAmbient = (ambientDiffuse * aKd + ambientSpecular) * u_Ao;
	//最终结果就是环境光加直接光照
	vec3 finalColor = finalAmbient + finalDiffuse;//不能简单的直接相乘，这破坏能量守恒。

	return finalColor;
}

//环境光映射是在定向光源下具有的，因此其他光源不做计算
vec3 PointLightColor(vec3 V, vec3 N, vec3 R, vec3 F0, float roughness, int index)
{
	vec3 finalDiffuse = vec3(0.0f);
	//L是光源的反向向量，不同类型的光源计算方式也不一样
	vec3 L = normalize(Point[index].Position - v_WorldPos);
	//计算观察者方向向量和光源的方向向量的半程向量，以此来判断表面的法线分布
	vec3 H = normalize(V + L);
	//该距离来缩放光照的强度
	float distance = length(Point[index].Position - v_WorldPos);
	//光照强度是与光照距离的平方成反比
	float attenuation = 1.0f / (distance * distance);
	//光从Wi方向进入表面每个点最终到达该点的光照能量
	vec3 radiance = Point[index].LightColor.rgb * attenuation;
	//计算DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//计算Cook-Torrance BRDF项
	vec3 nominator = D * G * F;
	//标准化项
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	vec3 specular = nominator / denominator;

	//计算Ks 以及 Kd 各部分贡献值
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//因为KD属于漫反射部分，它不会镜面反射光线，所以这里还要乘以非金属量
	Kd *= (1.0f - u_Metallic);

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;

	//最后把环境光项加上[间接光照，全局光照][环境光前强度受的是定向光源影响]//除定向光外其他不受环境光影响
	vec3 finalAmbient = vec3(0.0f,0.0f,0.0f); //Direction.Intensity * DefaultColor.rgb * u_Ao;
	//最终结果就是环境光加直接光照
	vec3 finalColor = finalAmbient + finalDiffuse;

	return finalColor;
}

vec3 SpotLightColor(vec3 V, vec3 N, vec3 R, vec3 F0, float roughness, int index)
{
	vec3 finalDiffuse = vec3(0.0f);
	//L是光源的反向向量，不同类型的光源计算方式也不一样
	vec3 L = normalize(Spot[index].Position - v_WorldPos);
	//得到聚光的照射方位夹角
	float theta = dot(L,normalize(Spot[index].Rotation));
	float epsilon = (Spot[index].CutOff - Spot[index].OuterCutOff);
	float intensity = clamp((theta - Spot[index].OuterCutOff) / epsilon, 0.0, 1.0);
	//计算观察者方向向量和光源的方向向量的半程向量，以此来判断表面的法线分布
	vec3 H = normalize(V + L);
	//该距离来缩放光照的强度
	float distance = length(Spot[index].Position - v_WorldPos);
	//光照强度是与光照距离的平方成反比
	float attenuation = 1.0f / (distance * distance);
	//光从Wi方向进入表面每个点最终到达该点的光照能量
	vec3 radiance = Spot[index].LightColor.rgb * attenuation;
	//计算DFG
	vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);
	float D = DistributionGGX(N, H, roughness); 
	float G = GeometrySmith(N, V, L, roughness);

	//计算Cook-Torrance BRDF项
	vec3 nominator = D * G * F;
	//标准化项
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	vec3 specular = nominator / denominator;

	//计算Ks 以及 Kd 各部分贡献值
	vec3 Ks = F;
	vec3 Kd = vec3(1.0f) - Ks;
	//因为KD属于漫反射部分，它不会镜面反射光线，所以这里还要乘以非金属量
	Kd *= (1.0f - u_Metallic);

	float NdotL = max(dot(N,L),0.0);
	finalDiffuse = (Kd * DefaultColor.rgb / PI + specular) * radiance * NdotL;

	//最后把环境光项加上[间接光照，全局光照]
	vec3 finalAmbient = vec3(0.0f,0.0f,0.0f);//vec3(0.03) * DefaultColor.rgb * u_Ao;
	//最终结果就是环境光加直接光照
	vec3 finalColor = finalAmbient + (finalDiffuse * intensity);

	return finalColor;
}

//----------------

//Shadow计算
float ShadowCalculation(vec4 fragPosLightSpace, vec3 vdir, vec3 ndir)
{
	//得到光源的方向向量
	vec3 ldir = normalize(Direction.Rotatiion);

	//因为真正的深度值是在裁剪空间下的标准设备坐标系下的值
	//所以先进行透视除法【这在正交下没有但在透视投影下是必要的】
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//把当前值变换为[0,1]
	projCoords = projCoords * 0.5 + 0.5;
	//采样深度纹理
	float shadowMapDepth = texture(DepthShadowTexture, projCoords.xy).r;
	//当前深度值
	float currentDepth = projCoords.z;
	//对当前采样点[深度]进行偏移，防止阴影失帧[根据光与物体的夹角大小来决定偏移的大小]
	float bias = max(0.05 * (1.0 - max(dot(ndir, ldir),0.0f)), 0.005);
	//然后进行比较
	float shadow = currentDepth - bias > shadowMapDepth ? 1.0 : 0.0;

	//当当前点超出了光的正交视锥的远平面时，它的投影坐标的z坐标大于1.0，
	//这种情况下，GL_CLAMP_TO_BORDER环绕方式不起作用,所以大于1直接返回0即可
	if(projCoords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

//----------------

//----------------

//还未进行阴影计算，但暂时不急，阴影渲染任然有问题。
void main()
{
	//计算通量
	vec3 N = v_Normal;
	vec3 V = normalize(u_ViewPosition - v_WorldPos);
	vec3 R = reflect(-V,N);
	vec3 FinalColor = vec3(0.0f);

	//根据金属度去赋值F0
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, DefaultColor.rgb, u_Metallic);

	float shadow = ShadowCalculation(v_FragPosLightSpace,V,N);

	//对粗超度进行限制(当粗超度为0时计算结果无效)
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

	//物理上的准确性要求在线性空间中计算，但要显示在设备上就要进行GAMM校正
	//同时线性计算值会超过1，因此要在Gamm校正之前先把结果从LDR映射为HDR[色调映射方法为Reinhard 操作]
	FinalColor = FinalColor / (FinalColor + vec3(1.0));
	FinalColor = pow(FinalColor, vec3(1.0/2.2));

	//当前并未去计算带有贴图的物理PBR结果。
	color = vec4(FinalColor,1.0f);
	PixelID = v_EntityID;
}