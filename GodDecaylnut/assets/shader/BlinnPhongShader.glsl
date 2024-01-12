#type vertex
#version 450 core
			
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

//光源计数器
uniform int DirectionNumber;
uniform int PointNumber;
uniform int SpotNumber;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
//深度纹理
uniform sampler2D DepthShadowTexture;

uniform vec4 DefaultColor;

uniform vec3 u_ViewPosition;

in flat int v_EntityID;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_WorldPos;
in vec4 v_FragPosLightSpace;

//bulin phong

//direction
//定向光的位置是位置，方向是方向
uniform vec3 direction_rotatiion;//这个暂时用不到
uniform vec3 direction_position;
uniform vec4 direction_lightcolor;
uniform vec4 direction_ambient;
uniform vec4 direction_diffuse;
uniform vec4 direction_specular;
uniform float direction_shininess;
uniform float direction_intensity;

//点光源
struct PointLight
{
	vec3 Position;
	vec4 LightColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float Shininess;
	float Constant;
	float Linear;
	float Quadratic;
};
uniform PointLight Point[4];

//聚光源
struct SpotLight
{
	vec3 Position;
	vec3 Rotation;
	vec4 LightColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float Shininess;
	float Constant;
	float Linear;
	float Quadratic;
	float CutOff;
	float OuterCutOff;
};
uniform SpotLight Spot[4];


vec3 DirectionLightColor(vec3 vdir, vec3 ndir, vec3 diffuseColor, vec3 specularColor, float shadowValue)
{
	//得到光源的方向向量
	vec3 ldir = normalize(-direction_rotatiion);
	//先把纹理采样出来
	vec3 diffColor = direction_lightcolor.rgb * diffuseColor;
	vec3 specColor = direction_lightcolor.rgb * specularColor;

	vec3 ambient = direction_ambient.rgb * diffColor.rgb * direction_intensity;

	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = direction_diffuse.rgb * diffColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),direction_shininess);
	vec3 specular = direction_specular.rgb * specColor.rgb * spec;

	return (ambient + (1.0 - shadowValue) * (diffuse + specular));

}

vec3 PointLightColor(vec3 vdir, vec3 ndir, vec3 diffuseColor, vec3 specularColor,int index)
{
	//得到点光的方向向量
	vec3 ldir = normalize(Point[index].Position - v_WorldPos);

	//采样纹理
	vec3 diffColor = Point[index].LightColor.rgb * diffuseColor;
	vec3 specColor = Point[index].LightColor.rgb * specularColor;

	//这里不做环境光计算[环境光由定向光决定]
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Point[index].DiffuseColor.rgb * diffColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Point[index].Shininess);
	vec3 specular = Point[index].SpecularColor.rgb * specColor.rgb * spec;

	//计算光照衰减
	float distance    = length(Point[index].Position - v_WorldPos);
    float attenuation = 1.0f / (Point[index].Constant + Point[index].Linear * distance + Point[index].Quadratic * (distance * distance));    
 
    diffuse  *= attenuation;
    specular *= attenuation;

	return diffuse + specular;
}

vec3 SpotLightColor(vec3 vdir, vec3 ndir, vec3 diffuseColor, vec3 specularColor,int index)
{
	//得到聚光源的方向向量
	vec3 ldir = normalize(Spot[index].Position - v_WorldPos);
	//得到聚光的照射方位夹角
	float theta = dot(ldir,normalize(Spot[index].Rotation));
	float epsilon = (Spot[index].CutOff - Spot[index].OuterCutOff);
	float intensity = clamp((theta - Spot[index].OuterCutOff) / epsilon, 0.0, 1.0);
	//同样不计算环境光
	//采样贴图
	vec3 diffColor = Spot[index].LightColor.rgb * diffuseColor;
	vec3 specColor = Spot[index].LightColor.rgb * specularColor;

	//计算光照
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Spot[index].DiffuseColor.rgb * diffColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Spot[index].Shininess);
	vec3 specular = Spot[index].SpecularColor.rgb * specColor.rgb * spec;

	//内外圈
	diffuse  *= intensity;
    specular *= intensity;

	//计算光照衰减
	float distance    = length(Spot[index].Position - v_WorldPos);
	float attenuation = 1.0f / (Spot[index].Constant + Spot[index].Linear * distance + Spot[index].Quadratic * (distance * distance));    
 
	diffuse  *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

//----------------

//Shadow计算
float ShadowCalculation(vec4 fragPosLightSpace, vec3 vdir, vec3 ndir)
{
	//得到光源的方向向量
	vec3 ldir = normalize(direction_rotatiion);

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

void main()
{
	//计算Buling-Phong模型
	vec3 viewDir = normalize(u_ViewPosition - v_WorldPos);//指向观察者
	vec3 normal = normalize(v_Normal);//单位化法线方便计算
	vec3 FinalColor;

	//此为定向光源的Shadow计算,并且Shadow不影响环境光[当shadow值为1，那么该点不在阴影中，而为0意味着在阴影中，不去计算漫反射和高光反射]
	float shadow = ShadowCalculation(v_FragPosLightSpace,viewDir,normal);

	//物体本身默认的叠加色
	vec3 defColor = DefaultColor.rgb;
	//先把纹理采样出来
	vec3 diffuseColor = texture(DiffuseTexture,v_TexCoords).rgb * defColor;
	vec3 specularColor = texture(SpecularTexture,v_TexCoords).rgb * defColor;

	//计算direction光
	if(DirectionNumber != 0)
	{
		FinalColor = DirectionLightColor(viewDir,normal,diffuseColor,specularColor,shadow);
	}
	if(PointNumber != 0)
	{
		//根据number的数量决定要循环的次数，并进行结果的累计
		for(int i = 0;i < PointNumber;i++)
		{
			FinalColor += PointLightColor(viewDir,normal,diffuseColor,specularColor,i);
		}
	}
	if(SpotNumber != 0)
	{
		//根据number的数量决定要循环的次数，并进行结果的累计
		for(int i = 0;i < SpotNumber;i++)
		{
			FinalColor += SpotLightColor(viewDir,normal,diffuseColor,specularColor,i);
		}
	}
	
	//如果什么光源都没有就返回全黑就行
	if(DirectionNumber == 0 && PointNumber == 0 && SpotNumber == 0)
	{
		FinalColor = vec3(0.0,0.0,0.0);
	}

	color = vec4(FinalColor,1.0f);

	PixelID = v_EntityID;
}