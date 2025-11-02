#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
class GfxContext;
class GfxObject;
class GfxPass;

class GfxQueue
{
private:
    GfxContext *_context;
    GfxPass *_pass;
    std::vector<GfxObject *> _opaqueQueue;
    std::vector<GfxObject *> _transparentQueue;


   /*  //帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images） */
    std::vector<VkFramebuffer> _swapChainFramebuffers;
    /* //命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。 */
    std::vector<VkCommandBuffer> _commandBuffers;
   /*  //更新帧缓冲区和命令缓冲区 */
    void _createBuffers();
    void _createFramebuffers();
    void _createCommandBuffers();
    void _cleanFramebuffers();
    void _cleanCommandBuffers();
    void _beginBindRenderPass(uint32_t imageIndex);

    void _Log(std::string msg);
public:
    GfxQueue(GfxContext *context,GfxPass *pass);
    /**
     * ui 渲染通道队列顶点输入
     */
    void submit(GfxObject *object);
    void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
    void reset();
    void clear();
    ~GfxQueue();
};