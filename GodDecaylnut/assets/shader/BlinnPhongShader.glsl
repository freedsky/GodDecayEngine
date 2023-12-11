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
	//ת��Ϊ��������
	v_WorldPos = vec3(u_Model * vec4(a_Position,1.0f));
	//�÷��߲����б��Ӱ��
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform vec4 DefaultColor;//��bulin Phong�б�������ʹ��

in flat int v_EntityID;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_WorldPos;
uniform vec3 u_ViewPosition;

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

vec3 DirectionLightColor(vec3 ldir, vec3 vdir, vec3 ndir)
{
	//�Ȱ������������
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
//----------------




void main()
{
	//����Buling-Phongģ��
	//�õ���Դ�ķ�������[������������Դ���Ͳ�һ��]�͹۲��߷�������
	vec3 lightDir = normalize(direction_position);//�ݶ��������ַ�ʽ
	vec3 viewDir = normalize(u_ViewPosition - v_WorldPos);//ָ��۲���
	vec3 normal = normalize(v_Normal);//��λ�����߷������
	//����direction��
	vec3 directionColor = DirectionLightColor(lightDir,viewDir,normal);


	color = vec4(directionColor,1.0f);
	PixelID = v_EntityID;
}