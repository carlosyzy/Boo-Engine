#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    float time;
} ubo;

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inModelRow0;
layout (location = 3) in vec4 inModelRow1;
layout (location = 4) in vec4 inModelRow2;
layout (location = 5) in vec4 inModelRow3;
layout (location = 6) in vec4 inInstanceColor;  // 颜色输入
void main() {
    //计算世界坐标
    vec4 worldPosition = vec4(inWorldPos.xy, 0.0, 1.0);
    vec4 viewPos = ubo.view * worldPosition;
    // 转换为NDC坐标
    vec4 clipPos = ubo.proj * viewPos;
    gl_Position = clipPos;
    gl_Position.y = -gl_Position.y;
}
