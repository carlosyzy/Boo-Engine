#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texSampler; // 对应 binding = 1
layout(location = 0) in vec2 uv;  // 输入UV坐标
layout(location = 0) out vec4 outColor;

void main() {
    // 使用推送常量块中的颜色
    vec4 color = vec4(1.0,1.0,1.0,1.0);
    color *= texture(texSampler, uv);
    outColor = color;
}
