#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include "../gfx-struct.h"

/**
 * @brief 渲染批次
 */

//后期想办法走动态的分配，现在先固定大小
struct StorageBufferObject
{
    alignas(16) float modelMat[16];          // 模型矩阵
    alignas(4) float color[4];               // 颜色
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
    GfxMaterial &_material;
    GfxMesh &_mesh;


    // 现有的成员变量...
    std::vector<VkBuffer> _storageBuffers;
    std::vector<VkDeviceMemory> _storageBuffersMemory;
    std::vector<void *> _storageBuffersMapped;
    // 现有的方法...
    void _createStorageBuffers();
    uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
    GfxRenderBatch(GfxMaterial &material, GfxMesh &mesh);
    const GfxMaterial &material() const { return _material; }
    const GfxMesh &mesh() const { return _mesh; }
    void addObject();
    
    ~GfxRenderBatch();
};