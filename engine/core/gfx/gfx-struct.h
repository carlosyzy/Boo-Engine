#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_FRAMES_IN_FLIGHT = 2;
const VkSampleCountFlagBits MsaaSamples = VK_SAMPLE_COUNT_1_BIT;


struct UniformBufferObject
{
	/* // 模型矩阵 */
	float model[16];
	/* // 视图矩阵 */
	float view[16];
	/* // 投影矩阵 */
	float proj[16];
};

// 后修管线中写死
struct PushConstants
{
	float defaultColor[4];
	// float maskRect[4];
};
/* // // 图形对象类型
// enum  GfxRendererPassTypeE
// {
// 	UI="GfxPass:UI",
// 	MODEL="GfxPass:Model",
// };

// enum class GfxObjectTypeE
// {
// 	DEFAULT,
// 	UI,
// 	MODEL,
// };
 */