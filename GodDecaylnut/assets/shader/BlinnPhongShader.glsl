#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_Texcoords;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out flat int v_EntityID;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_WorldPos;

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

uniform vec4 DefaultColor;//在bulin Phong中保留但不使用

in flat int v_EntityID;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_WorldPos;
uniform vec3 u_ViewPosition;

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


vec3 DirectionLightColor(vec3 vdir, vec3 ndir)
{
	//得到光源的方向向量
	vec3 ldir = normalize(direction_rotatiion);
	//先把纹理采样出来
	vec3 diffuseColor = direction_lightcolor.rgb * texture(DiffuseTexture,v_TexCoords).rgb;
	vec3 specularColor = direction_lightcolor.rgb * texture(SpecularTexture,v_TexCoords).rgb;

	vec3 ambient = direction_ambient.rgb * diffuseColor.rgb * direction_intensity;

	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = direction_diffuse.rgb * diffuseColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),direction_shininess);
	vec3 specular = direction_specular.rgb * specularColor.rgb * spec;

	return (ambient + diffuse + specular);

}

vec3 PointLightColor(vec3 vdir, vec3 ndir,int index)
{
	//得到点光的方向向量
	vec3 ldir = normalize(Point[index].Position - v_WorldPos);

	//采样纹理
	vec3 diffuseColor = Point[index].LightColor.rgb * texture(DiffuseTexture,v_TexCoords).rgb;
	vec3 specularColor = Point[index].LightColor.rgb * texture(SpecularTexture,v_TexCoords).rgb;

	//这里不做环境光计算[环境光由定向光决定]
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Point[index].DiffuseColor.rgb * diffuseColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Point[index].Shininess);
	vec3 specular = Point[index].SpecularColor.rgb * specularColor.rgb * spec;

	//计算光照衰减
	float distance    = length(Point[index].Position - v_WorldPos);
    float attenuation = 1.0f / (Point[index].Constant + Point[index].Linear * distance + Point[index].Quadratic * (distance * distance));    
 
    diffuse  *= attenuation;
    specular *= attenuation;

	return diffuse + specular;
}

vec3 SpotLightColor(vec3 vdir, vec3 ndir,int index)
{
	//得到聚光源的方向向量
	vec3 ldir = normalize(Spot[index].Position - v_WorldPos);
	//得到聚光的照射方位夹角
	float theta = dot(ldir,normalize(Spot[index].Rotation));
	float epsilon = (Spot[index].CutOff - Spot[index].OuterCutOff);
	float intensity = clamp((theta - Spot[index].OuterCutOff) / epsilon, 0.0, 1.0);
	//同样不计算环境光
	//采样贴图
	vec3 diffuseColor = Spot[index].LightColor.rgb * texture(DiffuseTexture,v_TexCoords).rgb;
	vec3 specularColor = Spot[index].LightColor.rgb * texture(SpecularTexture,v_TexCoords).rgb;

	//计算光照
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Spot[index].DiffuseColor.rgb * diffuseColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Spot[index].Shininess);
	vec3 specular = Spot[index].SpecularColor.rgb * specularColor.rgb * spec;

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

void main()
{
	//计算Buling-Phong模型
	vec3 viewDir = normalize(u_ViewPosition - v_WorldPos);//指向观察者
	vec3 normal = normalize(v_Normal);//单位化法线方便计算
	vec3 FinalColor;
	//计算direction光
	if(DirectionNumber != 0)
	{
		FinalColor = DirectionLightColor(viewDir,normal);
	}
	if(PointNumber != 0)
	{
		//根据number的数量决定要循环的次数，并进行结果的累计
		for(int i = 0;i < PointNumber;i++)
		{
			FinalColor += PointLightColor(viewDir,normal,i);
		}
	}
	if(SpotNumber != 0)
	{
		//根据number的数量决定要循环的次数，并进行结果的累计
		for(int i = 0;i < SpotNumber;i++)
		{
			FinalColor += SpotLightColor(viewDir,normal,i);
		}
	}
	
	//如果什么光源都没有就返回全黑就行
	if(DirectionNumber == 0 && PointNumber == 0 && SpotNumber == 0)
	{
		FinalColor = vec3(0.0,0.0,0.0);
	}	
	
	//看来关于环境光部分只有单独写个Shader来应对[应该是受限于采样方式的影响]

	color = vec4(FinalColor,1.0f);
	PixelID = v_EntityID;
}