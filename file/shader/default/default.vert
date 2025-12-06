#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec2 inUV;


layout(location = 0) out vec2 uv; // 输出UV坐标

void main() {
    gl_Position = inPos;
    //Vulkan 的标准化设备坐标（NDC）： 轴正方向 向下（左上角为 (-1, -1)，右下角为 (1, 1)）。
    //传统数学/OpenGL 坐标系：Y 轴正方向 向上（左下角为 (-1, -1)，右上角为 (1, 1)）。
    gl_Position.y = -gl_Position.y;
    uv = inUV;
}
