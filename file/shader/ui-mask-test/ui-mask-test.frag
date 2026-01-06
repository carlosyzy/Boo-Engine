#version 450
#extension GL_ARB_separate_shader_objects : enable

//输出属性
layout(location = 0) out vec4 outColor;
void main() {
    // 什么都不做
    // colorWriteMask = 0 已在管线配置中设置
    // 片段通过后会写入 Stencil 值（由管线状态控制）
    outColor = vec4(1.0,1.0,1.0,1.0);
}
