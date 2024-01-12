#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;
out flat int v_EntityID;

void main()
{
	//翻转Y轴即可[在渲染天空盒时会被翻转，这里便不再翻转]
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
	//此时传进来的已经转换完成的环境立方体贴图，只需要对其进行半球上的卷积即可
	vec3 normal = normalize(v_TexCoords);

	vec3 irradiance = vec3(0.0);
	//把本地的方向向量坐标转为关于球体的极坐标(单位球坐标)
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);
	//开始离散计算积分[采样固定数量的采样样本去近似(逼近)积分值]
	float sampleDelta = 0.025;//采样频率[决定最终结果的准确度(精度)]
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
	//然后求平均
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	color = vec4(irradiance,1.0f);
	PixelID = v_EntityID;
}