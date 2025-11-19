#version 450
#extension GL_ARB_separate_shader_objects : enable

// 遮罩 fragment shader：只写 Stencil，不写颜色
// 不需要任何输入、输出、uniform 或 push constants

void main() {
    // 什么都不做
    // colorWriteMask = 0 已在管线配置中设置
    // 片段通过后会写入 Stencil 值（由管线状态控制）
}
