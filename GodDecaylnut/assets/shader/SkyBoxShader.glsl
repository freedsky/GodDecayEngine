#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;
out flat int v_EntityID;

void main()
{
	//��תY�ἴ��
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
//���ΪHDRͼ��Ҫ�������٤��У��
uniform bool IsHDR;

void main()
{
	//�ȳ�ʼ��ɫ���
	vec3 TColor = texture(skybox, v_TexCoords).rgb * skyColor.rgb;

	//ŵΪHDRҪ�������HDRת��
	if(IsHDR)
	{
		TColor = TColor / (TColor + vec3(1.0));
		TColor = pow(TColor, vec3(1.0/2.2)); 
	}

	color = vec4(TColor,1.0f);

	PixelID = v_EntityID;
}