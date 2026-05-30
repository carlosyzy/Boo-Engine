#version 450
#extension GL_ARB_separate_shader_objects : enable

//采样器
layout(binding = 1) uniform sampler2D mainTexture; // 主纹理
layout(binding = 2) uniform sampler2D texSampler1; // 预留采样器1
layout(binding = 3) uniform sampler2D texSampler2; // 预留采样器2   
layout(binding = 4) uniform sampler2D texSampler3; // 预留采样器3
layout(binding = 5) uniform sampler2D texSampler4; // 预留采样器4
layout(binding = 6) uniform sampler2D texSampler5; // 预留采样器5
layout(binding = 7) uniform sampler2D texSampler6; // 预留采样器6       
layout(binding = 8) uniform sampler2D texSampler7; // 预留采样器7
layout(binding = 9) uniform sampler2D texSampler8; // 预留采样器8
layout(binding = 10) uniform sampler2D texSampler9; // 预留采样器9  

//顶点输出属性
layout(location = 0) in vec4 v_color;  // 输入颜色
layout(location = 1) in vec2 v_uv;  // 输入UV坐标
layout(location = 2) in vec2 v_uv_1;    // 输入UV坐标_1
layout(location = 3) in vec2 v_uv_2;    // 输入UV坐标_2
layout(location = 4) in vec3 v_pos;     // 输入世界坐标
layout(location = 5) in vec3 v_normal;  // 输入世界法线
layout(location = 6) in vec3 v_tangent;  // 输入切线向量


//输出属性
layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor = vec4(1.0, 1.0, 1.0, 1.0);
    baseColor *= texture(mainTexture, v_uv);
    outColor = baseColor;
}