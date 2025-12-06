#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv;  // 输入UV坐标
layout(location = 0) out vec4 outColor;

void main() {
    // 使用推送常量块中的颜色
    vec4 color = vec4(0.2,1.0,0.2,1.0);
    outColor = color;
}
