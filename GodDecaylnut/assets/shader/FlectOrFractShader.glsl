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
out vec3 v_Normal;
out vec3 v_WorldPos;

void main()
{
	v_EntityID = a_EntityID;
	//转换为世界坐标
	v_WorldPos = vec3(u_Model * vec4(a_Position,1.0f));
	//让法线不受切变的影响
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

in flat int v_EntityID;
in vec3 v_Normal;
in vec3 v_WorldPos;

//环境光贴图，用于反射和折射[完美]
uniform samplerCube AmbientLightTexture;
uniform int flag;//0 表示全反射模式 1 表示全折射模式
uniform vec3 u_ViewPosition;

void main()
{
	vec3 I = normalize(v_WorldPos - u_ViewPosition);
	vec3 R;
	vec3 mirrorRefect;
	vec3 mirrorRefract;
	if(flag == 0)
	{
		R = reflect(I, normalize(v_Normal));
		//把环境光映射算出来
		mirrorRefect = texture(AmbientLightTexture,vec3(R.x,-R.y,R.z)).rgb;

	}
	else if(flag == 1)
	{
		float ratio = 1.00 / 1.52;//折射指数按照玻璃材质计算
		R = refract(I, normalize(v_Normal), ratio);
		mirrorRefract = texture(AmbientLightTexture, vec3(R.x,-R.y,R.z)).rgb;
	}

	if(flag == 0)
	{
		color = vec4(mirrorRefect,1.0f);
	}
	else if(flag == 1)
	{
		color = vec4(mirrorRefract,1.0f);
	}
	PixelID = v_EntityID;
}