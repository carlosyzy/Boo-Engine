#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout (location = 0) in vec4 inPos;

void main() {
    gl_Position = ubo.model * inPos;
}
