#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;  // 输出法线
layout(location = 2) out vec2 fragTexCoord; // 输出UV坐标
layout(location = 3) out vec2 viewSize;
layout(location = 4) out vec2 worldPos;

void main() {
    // 简化版本：直接使用输入位置
    gl_Position = inPos * 0.9;
    gl_Position.y = -gl_Position.y;

    // 必须初始化所有输出变量，否则fragment shader会读到未定义数据
    fragColor = inColor.rgb;
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
    viewSize = vec2(1.0, 1.0);
    worldPos = inPos.xy;
}
