#pragma once
#include <vulkan/vulkan_core.h>

/**
 * @brief 渲染批次
 */

struct StorageBufferObject
{
    alignas(16) float modelMat[16];          // 模型矩阵
    alignas(4) float color[4];               // 颜色
    alignas(4) uint32_t defaultTextureIndex; // 默认纹理索引
};


class GfxBatch
{
private:
    // 最大对象数量
    static const uint32_t maxObjects = 1024;
    // 当前对象数量
    uint32_t _objectCount = 0;

    // 现有的成员变量...
    VkBuffer _storageBuffers;
    VkDeviceMemory _storageBuffersMemory;
    void *_storageBuffersMapped;
    // 现有的方法...
    void _createStorageBuffers();
    uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void _cleanStorageBuffers();

public:
    GfxBatch();
    void addObject(StorageBufferObject &object);
    ~GfxBatch();
};