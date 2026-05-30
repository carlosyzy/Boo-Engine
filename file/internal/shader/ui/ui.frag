#version 450
#extension GL_ARB_separate_shader_objects : enable

//采样器
layout(binding = 1) uniform sampler2D mainTexture;
//输入属性
layout(location = 0) in vec2 v_uv;  // 输入UV坐标
layout(location = 1) in vec4 color;  // 输入颜色

//输出属性
layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor=vec4(1.0,1.0,1.0,1.0);
    baseColor *= color; // 合并颜色
    baseColor *= texture(mainTexture, v_uv); // 合并颜色
    outColor = baseColor;
}