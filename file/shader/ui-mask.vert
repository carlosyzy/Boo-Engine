#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;


void main() {
    gl_Position = ubo.model * inPos;
}
