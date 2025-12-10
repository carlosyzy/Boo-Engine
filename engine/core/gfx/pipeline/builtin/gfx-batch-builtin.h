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
#include "../../gfx-struct.h"

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
class GfxPipelineBuiltin;
class GfxRendererBuiltin;

class GfxBatchBuiltin
{
private:
    // 最大对象数量
    static const uint32_t maxObjects = 1024;
    // 当前对象数量
    uint32_t _objectCount = 0;
    GfxMaterial _material;
    GfxMesh _mesh;

    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
    int _indexSize;
    void _createVertexBuffers();

    // void _beginBindRenderPass(GfxRenderTexture *renderTexture);
    // uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
    GfxBatchBuiltin(GfxMaterial material, GfxMesh mesh);
    GfxMaterial &material() { return _material; }
    GfxMesh &mesh() { return _mesh; }
    void addObject();
    void render(GfxPipelineBuiltin *pipeline, VkCommandBuffer &queueCommandBuffer, VkDescriptorSet &descriptorSet);
    ~GfxBatchBuiltin();
};