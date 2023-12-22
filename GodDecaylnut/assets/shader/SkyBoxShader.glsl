#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;
out flat int v_EntityID;

void main()
{
	//翻转Y轴即可
	v_TexCoords = vec3(a_Position.x,-a_Position.y,a_Position.z);
	v_EntityID = a_EntityID;
	vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

in vec3 v_TexCoords;
in flat int v_EntityID;

uniform samplerCube skybox;
uniform vec4 skyColor;

void main()
{
	//先初始纯色天空
	color = texture(skybox, v_TexCoords) * skyColor;
	PixelID = v_EntityID;
}