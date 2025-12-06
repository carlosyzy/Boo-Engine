#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "../gfx-struct.h"

/**
 * @brief 渲染批次
 */

// 后期想办法走动态的分配，现在先固定大小
struct StorageBufferObject
{
    alignas(16) float modelMat[16];   // 模型矩阵
    alignas(4) float color[4];        // 颜色
    alignas(4) uint32_t textureIndex; // 默认纹理索引
};

class GfxPass;
class GfxPipeline;

class GfxRenderBatch
{
private:
    // 最大对象数量
    static const uint32_t maxObjects = 1024;
    // 当前对象数量
    uint32_t _objectCount = 0;
    const GfxMaterial &_material;
    const GfxMesh &_mesh;

    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
    int _indexSize;
    void _createVertexBuffers();

    // 帧缓冲区:它连接了渲染通道（Render Pass） 和交换链图像（Swap Chain Images）
    std::vector<VkFramebuffer> _framebuffers;
    // 命令缓冲区是用于记录和执行 GPU 命令的内存块。在 Vulkan 中，几乎所有渲染操作都需要通过命令缓冲区来执行。
    std::vector<VkCommandBuffer> _commandBuffers;
    void _createBuffers();
    void _createFramebuffers();
    void _createCommandBuffers();

    void _beginBindRenderPass(uint32_t imageIndex);
    // uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
    GfxRenderBatch(const GfxMaterial &material, const GfxMesh &mesh);
    const GfxMaterial &material() const { return _material; }
    const GfxMesh &mesh() const { return _mesh; }
    void addObject();
    void render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers);
    ~GfxRenderBatch();
};