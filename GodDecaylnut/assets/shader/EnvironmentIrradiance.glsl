#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;
out flat int v_EntityID;

void main()
{
	//��תY�ἴ��[����Ⱦ��պ�ʱ�ᱻ��ת������㲻�ٷ�ת]
	v_TexCoords = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position,1.0f);
	v_EntityID = a_EntityID;
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int PixelID;

in vec3 v_TexCoords;
in flat int v_EntityID;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
	//��ʱ���������Ѿ�ת����ɵĻ�����������ͼ��ֻ��Ҫ������а����ϵľ������
	vec3 normal = normalize(v_TexCoords);

	vec3 irradiance = vec3(0.0);
	//�ѱ��صķ�����������תΪ��������ļ�����(��λ������)
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);
	//��ʼ��ɢ�������[�����̶������Ĳ�������ȥ����(�ƽ�)����ֵ]
	float sampleDelta = 0.025;//����Ƶ��[�������ս����׼ȷ��(����)]
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 
			irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
        	nrSamples++;
		}
	}
	//Ȼ����ƽ��
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	color = vec4(irradiance,1.0f);
	PixelID = v_EntityID;
}