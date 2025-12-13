//#pragma once
//#include <vulkan/vulkan_core.h>
//#include <iostream>
//#include <algorithm>
//#include <fstream>
//#include <vector>
//#include <set>
//#include <map>
//#include <cstdint>
//#include <array>
//#include "../../gfx-struct.h"
//
///**
// * @brief 渲染批次
// */
//
//// 后期想办法走动态的分配，现在先固定大小
//struct StorageBufferObject
//{
//    alignas(16) float modelMat[16];   // 模型矩阵
//    alignas(4) float color[4];        // 颜色
//    alignas(4) uint32_t textureIndex; // 默认纹理索引
//};
//
//class GfxPass;
//class GfxPipelineBuiltin;
//class GfxRendererBuiltin;
//
//class GfxBatchBuiltin
//{
//private:
//    // 最大对象数量
//    static const uint32_t maxObjects = 10;
//    // 当前对象数量
//    uint32_t _objectCount = 0;
//    GfxMaterial _material;
//    GfxMesh _mesh;
//    // void _beginBindRenderPass(GfxRenderTexture *renderTexture);
//    // uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//
//public:
//    GfxBatchBuiltin(const GfxMaterial &material, const GfxMesh &mesh);
//    GfxMaterial &material() { return _material; }
//    GfxMesh &mesh() { return _mesh; }
//    void addObject();
//    void render(GfxPipelineBuiltin *pipeline, VkCommandBuffer &queueCommandBuffer, VkDescriptorSet &descriptorSet);
//    void clear();
//    ~GfxBatchBuiltin();
//};