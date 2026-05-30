#pragma once
#include "gfx-builtin-batch.h"

class GfxBuiltinRenderer;
class GfxBuiltinPipeline;
class GfxRenderTexture;
class GfxTexture;
class GfxMaterial;
class GfxMesh;
class GfxBuffer;

class GfxBuiltinBatch3D : public GfxBuiltinBatch
{
private:
    //透明材质
    float _disTransparent = 0.0f;
protected:
    void _setViewportScissor(VkCommandBuffer &queueCommandBuffer) override;
    void _bindUniformBuffer() override;
    void _bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo) override;
    void _bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer) override;
    void _drawIndexed(VkCommandBuffer &queueCommandBuffer) override;
    
public:
    GfxBuiltinBatch3D();
    virtual void addObject(const std::vector<char> &instanceData);
    void render(VkCommandBuffer &queueCommandBuffer) override;
    float getDisTransparent() const;

    ~GfxBuiltinBatch3D();
};
