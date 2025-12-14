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

class GfxRendererBuiltin;
class GfxPipelineBuiltin;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBatchBuiltin
{
private:
    GfxRendererBuiltin *_renderer;
    GfxRenderTexture *_renderTexture;
    GfxMaterial *_material;
    GfxMesh *_mesh;

    std::vector<float> _instanceDatas;
    int _instanceCount = 0;
    /**
     * @brief 绑定渲染管线
     * 渲染第四步
     */
    void _bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxPipelineBuiltin *pipeline);
    void _bindUIDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxPipelineBuiltin *pipeline, GfxBuffer *ubo);
    void _bind3DDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxPipelineBuiltin *pipeline, GfxBuffer *ubo);

public:
    GfxBatchBuiltin(GfxRendererBuiltin *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh);
    const GfxMaterial *getMaterial() const { return _material; }
    const GfxMesh *getMesh() const { return _mesh; }
    void addObject(std::vector<float> &instanceData);
    void render(VkCommandBuffer &queueCommandBuffer, GfxBuffer *ubo);
    void destroy();
    ~GfxBatchBuiltin();
};