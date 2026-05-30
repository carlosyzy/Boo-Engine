#include "gfx-builtin-batch.h"
#include "../../gfx.h"
#include "../../gfx-manager.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-mesh.h"
#include "../../base/gfx-material.h"
#include "../../base/gfx-texture.h"
#include "../../base/gfx-render-texture.h"
#include "../../base/gfx-buffer.h"
#include "../../base/gfx-buffer-ubo.h"
#include "../../base/gfx-buffer-instance.h"
#include "../gfx-builtin-renderer.h"
#include "../gfx-builtin-pipeline/gfx-builtin-pipeline.h"

GfxBuiltinBatch::GfxBuiltinBatch()
{
}
void GfxBuiltinBatch::init(GfxBuiltinRenderer *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, const std::array<float, 4> &cameraPosition)
{
    this->_clear();
    this->_renderer = renderer;
    this->_renderTexture = renderTexture;
    this->_material = material;
    this->_mesh = mesh;
    this->_viewMatrix = viewMatrix;
    this->_projMatrix = projMatrix;
    this->_cameraPos = cameraPosition;
}
// 后续带上每个物体独有的数据
void GfxBuiltinBatch::addObject(const std::vector<char> &instanceData)
{
    this->_instanceDatas.insert(this->_instanceDatas.end(), instanceData.begin(), instanceData.end());
    this->_instanceCount++;
}
void GfxBuiltinBatch::render(VkCommandBuffer &queueCommandBuffer)
{
}
void GfxBuiltinBatch::_bindUniformBuffer()
{
}
void GfxBuiltinBatch::_bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline)
{
    vkCmdBindPipeline(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
}
void GfxBuiltinBatch::_setViewportScissor(VkCommandBuffer &queueCommandBuffer)
{
    // 设置视口
    VkViewport _viewport{};
    // 裁剪区域 原点左上角为(0,0)
    VkRect2D _scissor{};
    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float)this->_renderTexture->getWidth();
    _viewport.height = (float)this->_renderTexture->getHeight();
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;
    _scissor.offset = {0, 0};
    _scissor.extent = {(uint32_t)(this->_renderTexture->getWidth()), (uint32_t)(this->_renderTexture->getHeight())};
    vkCmdSetViewport(queueCommandBuffer, 0, 1, &_viewport);
    // 设置裁剪区域
    vkCmdSetScissor(queueCommandBuffer, 0, 1, &_scissor);
}
void GfxBuiltinBatch::_bindDescriptorSets(VkCommandBuffer &queueCommandBuffer, GfxBuiltinPipeline *pipeline, GfxBuffer *ubo)
{
}
void GfxBuiltinBatch::_bindVertexIndicesBuffers(VkCommandBuffer &queueCommandBuffer)
{
    
    GfxBuffer *instanceBuffer = Gfx::_bufferInstance->getBuffer(this->_instanceDatas.size());
    memcpy(instanceBuffer->getMappedData(), this->_instanceDatas.data(), this->_instanceDatas.size());
    VkDeviceSize offsets[2] = {0, 0};
    VkBuffer vertexBuffer = this->_mesh->getVertexBuffer();
    VkBuffer indexBuffer = this->_mesh->getIndexBuffer();
    VkBuffer vertexBuffers[] = {vertexBuffer, instanceBuffer->getBuffer()}; // 作为顶点缓冲绑定
    vkCmdBindVertexBuffers(queueCommandBuffer, 0, 2, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(queueCommandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}
void GfxBuiltinBatch::_drawIndexed(VkCommandBuffer &queueCommandBuffer)
{
    vkCmdDrawIndexed(
        queueCommandBuffer,
        this->_mesh->getIndexCount(), // 只绘制3个索引（第一个三角形）
        this->_instanceCount,         // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
        0,                            // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
        0,                            // 第一个实例的索引 从第 0 个实例开始绘制
        0                             // 实例偏移
    );
    Gfx::_drawCount++;
}
void GfxBuiltinBatch::_clear()
{
    this->_renderer = nullptr;
    this->_renderTexture = nullptr;
    this->_material = nullptr;
    this->_mesh = nullptr;
    this->_instanceDatas.clear();
    this->_instanceCount = 0;
}
void GfxBuiltinBatch::destroy()
{
    this->_clear();
}
GfxBuiltinBatch::~GfxBuiltinBatch()
{
}