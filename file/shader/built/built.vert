#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    // mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec4 inPos;
// layout (location = 1) in vec2 inTexCoord;
// layout(location = 0) out vec2 fragTexCoord;
// layout(location = 1) out vec3 fragNormal;  // 输出法线
// layout(location = 2) out vec2 fragTexCoord; // 输出UV坐标
// layout(location = 3) out vec2 viewSize;
// layout(location = 4) out vec2 worldPos;

void main() {
   
    //Vulkan 的标准化设备坐标（NDC）： 轴正方向 向下（左上角为 (-1, -1)，右下角为 (1, 1)）。
    //传统数学/OpenGL 坐标系：Y 轴正方向 向上（左下角为 (-1, -1)，右上角为 (1, 1)）。
   
    // //计算世界坐标
    // vec4 worldPosition = ubo.model * vec4(inPos.xy, 0.0, 1.0);
    // // worldPos = worldPosition.xy;
    // //将物体从世界空间 → 观察空间（相机空间）
    // //视图矩阵移动的是整个世界，而不是相机
    // // 视图矩阵将相机位置从世界空间转换到观察空间
    // vec4 viewPos = ubo.view * worldPosition;
    // // 转换为NDC坐标
    // vec4 clipPos = ubo.proj * viewPos;
    gl_Position = vec4(inPos.xyz, 1.0);
    gl_Position.y = -gl_Position.y;
    // fragNormal = inNormal;
    // fragTexCoord = inTexCoord;
    // viewSize=vec2(2.0/ubo.proj[0][0],2.0/ubo.proj[1][1]);
}
