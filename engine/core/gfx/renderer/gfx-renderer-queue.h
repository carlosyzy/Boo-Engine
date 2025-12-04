// #pragma once
// #include <vulkan/vulkan_core.h>
// #include <iostream>
// #include <algorithm>
// #include <fstream>
// #include <vector>
// #include <set>
// #include <map>
// #include <cstdint>
// // #include "gfx-queue-struct.h"

// // class GfxContext;
// // class GfxObject;
// // class GfxPass;
// // class GfxTexture;
// // struct GfxPassStruct;

// /**
//  * ui的统一缓冲区对象，用于存储视图矩阵、投影矩阵, 时间（全局）
//  */
// struct UniformBufferUI
// {
//     // 视图矩阵
//     float view[16];
//     // 投影矩阵
//     float proj[16];
//     // 时间（全局）
//     float time;
// };
// /**
//  * ui的统一缓冲区对象，用于存储视图矩阵、投影矩阵, 时间（全局）
//  */
// struct UniformBuffer3D
// {
//     // 视图矩阵
//     float view[16];
//     // 投影矩阵
//     float proj[16];
//     // 时间（全局）
//     float time;
//     // 平行光
//     // 多个光源
// };

// class GfxQueue
// {
// private:
//     VkBuffer _buffer = VK_NULL_HANDLE;
//     VkDeviceMemory _memory = VK_NULL_HANDLE;
//     void _createBuffer();
//     //     VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
//     //
//     //     VkDeviceMemory _indexMemory = VK_NULL_HANDLE;

//     // std::string _name;
//     // GfxContext *_context;
//     // GfxPass *_pass;
//     // GfxQueueRendererStatus _rendererStatus;

//     // uint32_t _stencilRef = 0;

//     // // 渲染队列 ui统一在transparent队列
//     // // 不透明队列
//     // std::vector<GfxObject *> _queueOpaqueQueue;
//     // // 透明队列
//     // std::vector<GfxObject *> _queueTransparentQueue;

//     // /*  //帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images） */
//     // std::vector<VkFramebuffer> _queueFramebuffers;
//     // /* //命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。 */
//     // std::vector<VkCommandBuffer> _queueCommandBuffers;

//     // GfxTexture *_colorTexture;
//     // GfxTexture *_depthTexture;
//     // void _createTextures();

//     // /*  //更新帧缓冲区和命令缓冲区 */
//     // void _createBuffers();
//     // void _createFramebuffers();
//     // void _createCommandBuffers();

//     // void _clearTextures();
//     // void _cleanFramebuffers();
//     // void _cleanCommandBuffers();

//     // void _beginBindRenderPass(uint32_t imageIndex);
//     // void _renderObject(uint32_t imageIndex,GfxObject *object);

// public:
//     GfxQueue();
//     // void create(GfxPass *pass);
//     // void add(GfxObject *object);
//     // void remove(GfxObject *object);
//     // /**
//     //  * ui 渲染通道队列顶点输入
//     //  */
//     // void submit(GfxObject *object);
//     // void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
//     // void reset();
//     // void clear();
//     ~GfxQueue();
// };