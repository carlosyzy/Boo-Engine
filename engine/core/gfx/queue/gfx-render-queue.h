#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <array>
#include <cstdint>
#include "../gfx-struct.h"

/**
 * 渲染队列-一个摄像机一个队列
 * 同一个摄像机的渲染对象，会被放到同一个渲染队列中
 * 按照渲染状态被添加到不同的渲染批次中
 */

struct UniformBufferObject
{
	// 视图矩阵
	alignas(16) float viewMat[16];
	// 投影矩阵
	alignas(16) float projMat[16];
	// 时间（全局）
	alignas(4) float time;
};

class GfxRenderBatch;

class GfxRenderQueue
{
protected:
	std::array<float, 16> _viewMat;
	std::array<float, 16> _projMat;
	// 渲染批次
	std::vector<GfxRenderBatch*> _batches;


	GfxRenderBatch *_testBatch = nullptr;
public:
	GfxRenderQueue();
	void init(const std::array<float, 16>& viewMat,const std::array<float, 16>& projMat);
	void submitObject(const GfxMaterial& material, const GfxMesh& mesh);
	void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
	~GfxRenderQueue();
};

//   // void create(GfxPass *pass);
//     // void add(GfxObject *object);
//     // void remove(GfxObject *object);
//     // /**
//     //  * ui 渲染通道队列顶点输入
//     //  */
//     // void submit(GfxObject *object);
//     // void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
//     // void reset();
//     // void clear();
//      /**
//      * @brief 准备渲染队列
//      *  1. 创建uniform buffer
//      *  2. 创建command buffer
//      */

// VkBuffer _buffer = VK_NULL_HANDLE;
// VkDeviceMemory _memory = VK_NULL_HANDLE;
// void _createBuffer();
//     VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
//
//     VkDeviceMemory _indexMemory = VK_NULL_HANDLE;

// std::string _name;
// GfxContext *_context;
// GfxPass *_pass;
// GfxQueueRendererStatus _rendererStatus;

// uint32_t _stencilRef = 0;

// // 渲染队列 ui统一在transparent队列
// // 不透明队列
// std::vector<GfxObject *> _queueOpaqueQueue;
// // 透明队列
// std::vector<GfxObject *> _queueTransparentQueue;

// /*  //帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images） */
// std::vector<VkFramebuffer> _queueFramebuffers;
// /* //命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。 */
// std::vector<VkCommandBuffer> _queueCommandBuffers;

// GfxTexture *_colorTexture;
// GfxTexture *_depthTexture;
// void _createTextures();

// /*  //更新帧缓冲区和命令缓冲区 */
// void _createBuffers();
// void _createFramebuffers();
// void _createCommandBuffers();

// void _clearTextures();
// void _cleanFramebuffers();
// void _cleanCommandBuffers();

// void _beginBindRenderPass(uint32_t imageIndex);
// void _renderObject(uint32_t imageIndex,GfxObject *object);