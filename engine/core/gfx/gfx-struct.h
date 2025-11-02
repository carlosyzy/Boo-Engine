#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_FRAMES_IN_FLIGHT = 2;
const VkSampleCountFlagBits MsaaSamples = VK_SAMPLE_COUNT_1_BIT;

struct QueueFamilyIndices
{
	/* // 支持图像绘制的队列 */
	uint32_t graphicsFamily = -1;

	/* // 支持图形呈现的队列 */
	uint32_t presentFamily = -1;

	bool isComplete()
	{
		return (graphicsFamily >= 0) && (presentFamily >= 0);
	}
};
/* // 查询并记录交换链支持的细节 */
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;		/* // 基础表面特性 */
	std::vector<VkSurfaceFormatKHR> formats;	/* // 像素格式、色彩空间 */
	std::vector<VkPresentModeKHR> presentModes; /* // 可用的呈现模式 */
};
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
	float maskRect[4];
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