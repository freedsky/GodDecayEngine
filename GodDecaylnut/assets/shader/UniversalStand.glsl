#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_Texcoords;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_TexCoords;
out flat int v_EntityID;

void main()
{
	v_TexCoords = a_Texcoords;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);	
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

uniform sampler2D DefaultTexture;
uniform vec4 DefaultColor;

in vec2 v_TexCoords;
in flat int v_EntityID;

void main()
{
	color = texture(DefaultTexture, v_TexCoords) * DefaultColor;
	PixelID = v_EntityID;
}