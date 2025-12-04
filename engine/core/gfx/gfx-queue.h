#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
// #include "gfx-queue-struct.h"

// class GfxContext;
// class GfxObject;
// class GfxPass;
// class GfxTexture;
// struct GfxPassStruct;

class GfxQueue
{
private:
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
    

public:
    GfxQueue();
    // void create(GfxPass *pass);
    // void add(GfxObject *object);
    // void remove(GfxObject *object);
    // /**
    //  * ui 渲染通道队列顶点输入
    //  */
    // void submit(GfxObject *object);
    // void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
    // void reset();
    // void clear();
    ~GfxQueue();
};