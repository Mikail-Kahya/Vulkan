#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;

layout(location = 0) out vec4 outColor;

vec3 g_LightDirection = vec3(0.89, 0.45, 0.02);
const float g_LightIntensity = 7;
const float PI = 3.1415926535897932384626433832795;
const float g_MinObservedArea = 0.05;

float observed_area() 
{
    return max(g_MinObservedArea, dot(-g_LightDirection, fragNormal));
}

vec4 lambert(float kd, vec4 color)
{
    return color * kd / PI;
}

void main() {
    outColor = texture(texSampler, fragTexCoord);
    float alpha = outColor.w;

    outColor = lambert(g_LightIntensity, outColor) * observed_area();
    outColor.w = alpha;
}