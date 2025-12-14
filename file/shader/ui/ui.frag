#version 450
#extension GL_ARB_separate_shader_objects : enable

//采样器
layout(binding = 1) uniform sampler2D texDefault;
layout(binding = 2) uniform sampler2D texSampler1;
layout(binding = 3) uniform sampler2D texSampler2;
layout(binding = 4) uniform sampler2D texSampler3;
//输入属性
layout(location = 0) in vec2 uv;  // 输入UV坐标
layout(location = 1) in vec4 color;  // 输入颜色

//输出属性
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0,1.0,1.0,1.0);
    outColor *= color ;
    outColor *= texture(texDefault, uv);
    // outColor = color*texColor;
    // outColor = vec4(0.5, 0.5, 0.5, 1.0);
}
