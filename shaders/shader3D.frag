#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;

layout(location = 0) out vec4 outColor;

vec3 g_LightDirection = vec3(0.89, 0.45, 0.02);
const float PI = 3.1415926535897932384626433832795;

float observed_area() 
{
    return max(0, dot(-g_LightDirection, fragNormal));
}

vec3 lambert(vec3 kd, vec3 color)
{
    return color * kd / PI;
}

vec3 phong(const vec3 color, float ks, float exp, const vec3 l, const vec3 v, const vec3 n)
{
	const float cosAngle = dot(reflect(l, n), -v);
	return ks * pow(max(0, cosAngle), exp) * color;
}

void main() {
    //outColor = vec4(fragColor, 1.0);
    const float observedArea = observed_area();
    outColor = texture(texSampler, fragTexCoord);
    outColor.w=1;
    //outColor.x *= observedArea;
    //outColor.y *= observedArea;
    //outColor.z *= observedArea;
}