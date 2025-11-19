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
    // // 与 UI shader 保持一致的变换流程
    // // 世界坐标
    // vec4 worldPosition = ubo.model * vec4(inPos.xy, 0.0, 1.0);
    // // 观察空间
    // vec4 viewPos = ubo.view * worldPosition;
    // // NDC 坐标
    // vec4 clipPos = ubo.proj * viewPos;
    // gl_Position = clipPos;
    // // Y 轴翻转（Vulkan 特性）
    // gl_Position.y = -gl_Position.y;
     gl_Position = inPos*0.3;
    gl_Position.y = -gl_Position.y;
}
