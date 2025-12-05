#version 450
#extension GL_ARB_separate_shader_objects : enable

// layout(location = 0) in vec3 fragColor;
// layout(location = 1) in vec3 fragNormal;    // 输入法线
// layout(location = 2) in vec2 fragTexCoord;  // 输入UV坐标
// layout(location = 3) in vec2 viewSize;
// layout(location = 4) in vec2 worldPos;
// layout(binding = 1) uniform sampler2D texSampler; // 对应 binding = 1

layout(location = 0) in vec2 fragTexCoord;  // 输入UV坐标
layout(location = 0) out vec4 outColor;

// // 正确的推送常量块声明
// layout(push_constant) uniform PushConstants {
//     vec4 defaultColor;  // 使用vec4而不是float
// } PushConsts;
void main() {
    // // 使用推送常量块中的颜色
    // vec4 color = PushConsts.defaultColor;
    // vec4 texColor = texture(texSampler, fragTexCoord);
    // outColor = color*texColor;
    outColor = vec4(fragTexCoord, 0.0, 1.0);
}
