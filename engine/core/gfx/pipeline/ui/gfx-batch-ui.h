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

// /**
//  * @brief 渲染批次
//  */

// // 后期想办法走动态的分配，现在先固定大小
// struct StorageBufferObject
// {
//     alignas(16) float modelMat[16];   // 模型矩阵
//     alignas(4) float color[4];        // 颜色
//     alignas(4) uint32_t textureIndex; // 默认纹理索引
// };

class GfxPassUI;
class GfxPipelineUI;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxRendererUI;

class GfxBatchUI
{
private:
    GfxRendererUI *_renderer;
    GfxRenderTexture *_renderTexture;
    GfxMaterial *_material;
    GfxMesh *_mesh;

    /**
     * @brief 绑定渲染管线
     * 渲染第四步
     */
    void _bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxPipelineUI *pipeline);

public:
    GfxBatchUI(GfxRendererUI *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh);
    const GfxMaterial *getMaterial() const { return _material; }
    const GfxMesh *getMesh() const { return _mesh; }
    void addObject();
    void render(VkCommandBuffer &queueCommandBuffer);
    ~GfxBatchUI();
};