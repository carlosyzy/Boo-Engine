#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    float time;
} ubo;

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inModelRow0;
layout (location = 3) in vec4 inModelRow1;
layout (location = 4) in vec4 inModelRow2;
layout (location = 5) in vec4 inModelRow3;
layout (location = 6) in vec4 inInstanceColor;  // 颜色输入

// layout (location = 0) out vec2 uv;
// layout (location = 1) out vec4 color;
// 遮罩 shader 不需要任何输出（只写 Stencil）
void main() {
    // //Vulkan 的标准化设备坐标（NDC）： 轴正方向 向下（左上角为 (-1, -1)，右下角为 (1, 1)）。
    // //传统数学/OpenGL 坐标系：Y 轴正方向 向上（左下角为 (-1, -1)，右上角为 (1, 1)）。
    // //计算世界坐标
    // vec4 worldPosition = ubo.model * vec4(inPos.xy, 0.0, 1.0);
    // vec2 worldPos = worldPosition.xy;
    // //将物体从世界空间 → 观察空间（相机空间）
    // //视图矩阵移动的是整个世界，而不是相机
    // // 视图矩阵将相机位置从世界空间转换到观察空间
    // vec4 viewPos = ubo.view * worldPosition;
    // // 转换为NDC坐标
    // vec4 clipPos = ubo.proj * viewPos;
    // gl_Position = clipPos;
    // gl_Position.y = -gl_Position.y;
    // mat4 modelMatrix = mat4(
    //     inModelRow0,
    //     inModelRow1,
    //     inModelRow2,
    //     inModelRow3
    // );
    //Vulkan 的标准化设备坐标（NDC）： 轴正方向 向下（左上角为 (-1, -1)，右下角为 (1, 1)）。
    //传统数学/OpenGL 坐标系：Y 轴正方向 向上（左下角为 (-1, -1)，右上角为 (1, 1)）。
    //计算世界坐标
    vec4 worldPosition = vec4(inWorldPos.xy, 0.0, 1.0);
    // worldPos = worldPosition.xy;
    //将物体从世界空间 → 观察空间（相机空间）
    //视图矩阵移动的是整个世界，而不是相机
    // 视图矩阵将相机位置从世界空间转换到观察空间
    vec4 viewPos = ubo.view * worldPosition;
    // 转换为NDC坐标
    vec4 clipPos = ubo.proj * viewPos;
    gl_Position = clipPos;
    gl_Position.y = -gl_Position.y;

    // uv = inTexCoord;
}
