#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Coords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 o_nor;

void main()
{
	o_nor = a_Normal;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec3 o_nor;

void main()
{
	color = vec4(o_nor * 0.5f + 0.5f,1.0f) * u_Color;
}