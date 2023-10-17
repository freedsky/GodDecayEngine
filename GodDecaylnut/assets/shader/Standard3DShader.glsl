#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Coords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_uv;
out vec3 v_Normal;

void main()
{
	v_Color = a_Color;
	v_uv = a_Coords;
	v_Normal = normalize(a_Tangent);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_uv;
in vec3 v_Normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;


void main()
{
	color = texture(u_Texture, v_uv) * u_Color;
}