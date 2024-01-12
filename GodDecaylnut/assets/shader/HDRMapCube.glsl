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

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

//将柱状图映射到立方体上
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
	//先把本地坐标为对应的柱状坐标，然后再进行采样
	vec2 uv = SampleSphericalMap(normalize(v_TexCoords));
	//对柱状图采样
	vec3 skyColor = texture(equirectangularMap,vec2(uv.x,-uv.y)).rgb;

	color = vec4(skyColor,1.0f);
	PixelID = v_EntityID;
}