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

class GfxRendererDefault;
class GfxPipelineDefault;

class GfxQueueDefault
{
private :
	VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
	void _createVertexBuffers();

    GfxRendererDefault *_renderer;
    // 帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images）
	std::vector<VkFramebuffer> _framebuffers;
	// 命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。
	std::vector<VkCommandBuffer> _commandBuffers;


	void _createBuffers();
	void _createFramebuffers();
	void _createCommandBuffers();
	
	// std::vector<std::string> _renderTextures;

	// // 渲染批次
	// std::vector<GfxBatchBuiltin *> _batches;
	/**
	 * @brief 重置命令缓冲区
	 * 渲染第一步
	 */
	void _resetCommandBuffer(uint32_t imageIndex);
	/**
	 * @brief 开始命令缓冲区
	 * 渲染第二步
	 */
	void _beginCommandBuffer(uint32_t imageIndex);
	/**
	 * @brief 开始渲染传递
	 * 渲染第三步
	 */
	void _beginRenderPass(uint32_t imageIndex);
	/**
	 * @brief 绑定渲染管线
	 * 渲染第四步
	 */
	void _bindPipeline(uint32_t imageIndex, GfxPipelineDefault *pipeline);

	
public:
	GfxQueueDefault(GfxRendererDefault *renderer);
	void init();
	void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers,std::vector<std::string> &pipelineOutds);
	void _clean();
	void _reset();
	~GfxQueueDefault();
};
































//   // void create(GfxRenderPass *pass);
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
// GfxRenderPass *_pass;
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