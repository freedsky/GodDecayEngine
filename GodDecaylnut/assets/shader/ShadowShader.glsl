#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_Texcoords;
layout(location = 4) in int a_EntityID;

uniform mat4 u_LightProjection;
uniform mat4 u_LightModel;


//因为shadow不用直接绘制，大概是不需要id进行检查
void main()
{
	gl_Position = u_LightProjection * u_LightModel * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

void main()
{
	
}