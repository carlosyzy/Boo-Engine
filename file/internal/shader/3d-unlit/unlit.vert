#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    float time;
    vec4 cameraPos;
    vec4 mainLitDir;
    vec4 mainLitColor;
} ubo;
//cpu输入属性
layout (location = 0) in vec3 l_position; // 输入本地坐标
layout (location = 1) in vec3 l_normal; // 输入本地法线
layout (location = 2) in vec2 l_texCoord; // 输入uv坐标
layout (location = 3) in vec2 l_texCoord_1; // 输入uv坐标_1
layout (location = 4) in vec2 l_texCoord_2; // 输入uv坐标_2
layout (location = 5) in vec4 l_color; // 输入颜色
layout (location = 6) in vec4 l_tangent; // 输入切线
layout (location = 7) in mat4 w_matrix; // 输入模型世界矩阵
layout (location = 11) in mat4 w_matrix_it; // 输入模型逆转置矩阵

// 输出至片段着色器的属性
layout (location = 0) out vec4 v_color; // 输出颜色
layout (location = 1) out vec2 v_uv;    // 输出uv坐标
layout (location = 2) out vec2 v_uv_1;  // 输出uv坐标_1
layout (location = 3) out vec2 v_uv_2;  // 输出uv坐标_2
layout (location = 4) out vec3 v_pos;     // 输出世界坐标
layout (location = 5) out vec3 v_normal;  // 输出世界法线
layout (location = 6) out vec3 v_tangent; // 输出切线向量


void main() {
    //列主序矩阵存储在GPU中，列主序应该用（列向量右乘矩阵,向量写在矩阵右边）
    //计算世界坐标
    vec4 worldPosition = w_matrix*vec4(l_position, 1.0) ;
    vec4 worldNormal = normalize(w_matrix_it*vec4(l_normal, 0.0) );
    v_pos = worldPosition.xyz;
    v_normal = worldNormal.xyz;
    // 视图矩阵将相机位置从世界空间转换到观察空间
    vec4 viewPos = ubo.view*worldPosition;
    // 转换为NDC坐标
    vec4 clipPos = ubo.proj*viewPos;


    v_color = l_color.rgba;  // 输出实例颜色
    v_uv = l_texCoord;
    v_uv_1 = l_texCoord_1; 
    v_uv_2 = l_texCoord_2;
    v_tangent = l_tangent.xyz;

    gl_Position = clipPos;
}