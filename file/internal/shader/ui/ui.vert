#version 450
#extension GL_ARB_separate_shader_objects : enable
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    float time;
} ubo;
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoord;
layout (location = 2) in mat4 w_matrix;    // 世界矩阵
layout (location = 6) in vec4 in_color;    // 输入颜色

layout (location = 0) out vec2 v_uv;
layout (location = 1) out vec4 color;

void main() {
    //列主序矩阵存储在GPU中，列主序应该用（列向量右乘矩阵,向量写在矩阵右边）
    //计算世界坐标
    vec4 worldPos = w_matrix*vec4(l_position.xyz, 1.0) ;
    // 视图矩阵将相机位置从世界空间转换到观察空间
    vec4 viewPos = ubo.view*worldPos;
    // 转换为NDC坐标
    vec4 clipPos = ubo.proj*viewPos;
    gl_Position = clipPos;  
    v_uv = l_texCoord;
    color = in_color.rgba; // 输出颜色
}