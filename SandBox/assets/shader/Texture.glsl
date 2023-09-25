#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_uv;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_uv;

uniform mat4 u_ViewProjection;
uniform mat4 u_TransForm;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	v_uv = a_uv;
	gl_Position = u_ViewProjection * u_TransForm * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_uv;

uniform sampler2D u_Texture;

void main()
{
	float a = texture(u_Texture,v_uv).a * 0.2f;
	color = vec4(texture(u_Texture,v_uv).rgb,a);
}