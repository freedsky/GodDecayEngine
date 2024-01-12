#type vertex
#version 450 core
			
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

//��Դ������
uniform int DirectionNumber;
uniform int PointNumber;
uniform int SpotNumber;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
//�������
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
//������λ����λ�ã������Ƿ���
uniform vec3 direction_rotatiion;//�����ʱ�ò���
uniform vec3 direction_position;
uniform vec4 direction_lightcolor;
uniform vec4 direction_ambient;
uniform vec4 direction_diffuse;
uniform vec4 direction_specular;
uniform float direction_shininess;
uniform float direction_intensity;

//���Դ
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

//�۹�Դ
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
	//�õ���Դ�ķ�������
	vec3 ldir = normalize(-direction_rotatiion);
	//�Ȱ������������
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
	//�õ����ķ�������
	vec3 ldir = normalize(Point[index].Position - v_WorldPos);

	//��������
	vec3 diffColor = Point[index].LightColor.rgb * diffuseColor;
	vec3 specColor = Point[index].LightColor.rgb * specularColor;

	//���ﲻ�����������[�������ɶ�������]
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Point[index].DiffuseColor.rgb * diffColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Point[index].Shininess);
	vec3 specular = Point[index].SpecularColor.rgb * specColor.rgb * spec;

	//�������˥��
	float distance    = length(Point[index].Position - v_WorldPos);
    float attenuation = 1.0f / (Point[index].Constant + Point[index].Linear * distance + Point[index].Quadratic * (distance * distance));    
 
    diffuse  *= attenuation;
    specular *= attenuation;

	return diffuse + specular;
}

vec3 SpotLightColor(vec3 vdir, vec3 ndir, vec3 diffuseColor, vec3 specularColor,int index)
{
	//�õ��۹�Դ�ķ�������
	vec3 ldir = normalize(Spot[index].Position - v_WorldPos);
	//�õ��۹�����䷽λ�н�
	float theta = dot(ldir,normalize(Spot[index].Rotation));
	float epsilon = (Spot[index].CutOff - Spot[index].OuterCutOff);
	float intensity = clamp((theta - Spot[index].OuterCutOff) / epsilon, 0.0, 1.0);
	//ͬ�������㻷����
	//������ͼ
	vec3 diffColor = Spot[index].LightColor.rgb * diffuseColor;
	vec3 specColor = Spot[index].LightColor.rgb * specularColor;

	//�������
	float diff = max(dot(ndir, ldir),0.0f);
	vec3 diffuse = Spot[index].DiffuseColor.rgb * diffColor.rgb * diff;

	vec3 halfVec = normalize(ldir + vdir);
	float spec = pow(max(dot(ndir,halfVec),0.0f),Spot[index].Shininess);
	vec3 specular = Spot[index].SpecularColor.rgb * specColor.rgb * spec;

	//����Ȧ
	diffuse  *= intensity;
    specular *= intensity;

	//�������˥��
	float distance    = length(Spot[index].Position - v_WorldPos);
	float attenuation = 1.0f / (Spot[index].Constant + Spot[index].Linear * distance + Spot[index].Quadratic * (distance * distance));    
 
	diffuse  *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

//----------------

//Shadow����
float ShadowCalculation(vec4 fragPosLightSpace, vec3 vdir, vec3 ndir)
{
	//�õ���Դ�ķ�������
	vec3 ldir = normalize(direction_rotatiion);

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

void main()
{
	//����Buling-Phongģ��
	vec3 viewDir = normalize(u_ViewPosition - v_WorldPos);//ָ��۲���
	vec3 normal = normalize(v_Normal);//��λ�����߷������
	vec3 FinalColor;

	//��Ϊ�����Դ��Shadow����,����Shadow��Ӱ�컷����[��shadowֵΪ1����ô�õ㲻����Ӱ�У���Ϊ0��ζ������Ӱ�У���ȥ����������͸߹ⷴ��]
	float shadow = ShadowCalculation(v_FragPosLightSpace,viewDir,normal);

	//���屾��Ĭ�ϵĵ���ɫ
	vec3 defColor = DefaultColor.rgb;
	//�Ȱ������������
	vec3 diffuseColor = texture(DiffuseTexture,v_TexCoords).rgb * defColor;
	vec3 specularColor = texture(SpecularTexture,v_TexCoords).rgb * defColor;

	//����direction��
	if(DirectionNumber != 0)
	{
		FinalColor = DirectionLightColor(viewDir,normal,diffuseColor,specularColor,shadow);
	}
	if(PointNumber != 0)
	{
		//����number����������Ҫѭ���Ĵ����������н�����ۼ�
		for(int i = 0;i < PointNumber;i++)
		{
			FinalColor += PointLightColor(viewDir,normal,diffuseColor,specularColor,i);
		}
	}
	if(SpotNumber != 0)
	{
		//����number����������Ҫѭ���Ĵ����������н�����ۼ�
		for(int i = 0;i < SpotNumber;i++)
		{
			FinalColor += SpotLightColor(viewDir,normal,diffuseColor,specularColor,i);
		}
	}
	
	//���ʲô��Դ��û�оͷ���ȫ�ھ���
	if(DirectionNumber == 0 && PointNumber == 0 && SpotNumber == 0)
	{
		FinalColor = vec3(0.0,0.0,0.0);
	}

	color = vec4(FinalColor,1.0f);

	PixelID = v_EntityID;
}