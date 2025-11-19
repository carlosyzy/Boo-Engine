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

// 遮罩 shader 不需要任何输出（只写 Stencil）

void main() {
    // 与 UI shader 保持一致的变换（使用相同的缩放比例）
    gl_Position = inPos * 0.3;
    gl_Position.y = -gl_Position.y;
}
