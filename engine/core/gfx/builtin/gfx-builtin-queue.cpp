#include "gfx-builtin-queue.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-manager.h"
#include "../base/gfx-render-pass.h"
#include "../base/gfx-render-texture.h"
#include "../base/gfx-texture.h"
#include "../base/gfx-material.h"
#include "../base/gfx-mesh.h"
#include "../base/gfx-buffer.h"
#include "../base/gfx-buffer-ubo.h"
#include "gfx-builtin-render-pass.h"
#include "gfx-builtin-pipeline/gfx-builtin-pipeline.h"
#include "gfx-builtin-batch/gfx-builtin-batch.h"
#include "gfx-builtin-batch/gfx-builtin-batch-ui.h"
#include "gfx-builtin-batch/gfx-builtin-batch-3d.h"
#include "gfx-builtin-renderer.h"
#include "../../../log.h"

GfxBuiltinQueue::GfxBuiltinQueue(std::string renderId, uint32_t width, uint32_t height, GfxBuiltinRenderer *renderer) : _renderer(nullptr),
                                                                                                                        _renderTexture(nullptr)
{
    this->_rendererId = renderId;
    this->_renderer = renderer;
    this->_renderTexture = new GfxRenderTexture(renderId, width, height);
    this->_renderTexture->bindRenderPass(this->_renderer->getRenderPass());
    this->_uiBatches.reserve(500);            // 500个ui批次，足够多，避免频繁创建ui批次
    this->_3dOpaqueBatches.reserve(500);      // 500个3d不透明批次，足够多，避免频繁创建3d不透明批次
    this->_3dOpaqueMeshes.reserve(500);       // 500个3d不透明网格，足够多，避免频繁创建3d不透明网格
    this->_3dOpaqueMaterials.reserve(500);    // 500个3d不透明材质，足够多，避免频繁创建3d不透明材质
    this->_3dTransparentBatches.reserve(500); // 500个3d透明批次，足够多，避免频繁创建3d透明批次
}
void GfxBuiltinQueue::init()
{
}
void GfxBuiltinQueue::submitData(const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition)
{
    this->_viewMatrix = viewMatrix;
    this->_projMatrix = projMatrix;
    this->_isOnScreen = isOnScreen;
    this->_cameraPosition = cameraPosition;
}
bool GfxBuiltinQueue::getIsOnScreen()
{
    return this->_isOnScreen;
}
int GfxBuiltinQueue::getPriority() const
{
    return this->_priority;
}
void GfxBuiltinQueue::setPriority(int priority)
{
    this->_priority = priority;
}
void GfxBuiltinQueue::resize(uint32_t width, uint32_t height)
{
    this->_renderTexture->resize(width, height);
}
void GfxBuiltinQueue::submitObject(GfxMaterial *material, GfxMesh *mesh)
{
    if (material == nullptr || mesh == nullptr)
    {
        LOGE("[Gfx : QueueBuiltin] :: submitObject: material or mesh is nullptr");
        return;
    }
    if (!mesh->isVisible())
    {
        return;
    }
    GfxRendererState &pipelineState = material->getRendererState();
    if (pipelineState.layer == GfxRendererLayer::_UI)
    {
        this->_submitObjectUI(material, mesh);
    }
    else if (pipelineState.layer == GfxRendererLayer::_3D)
    {
        if (pipelineState.type == GfxRendererType::_Opaque)
        {
            this->_submitObject3DOpaque(material, mesh);
        }
        else if (pipelineState.type == GfxRendererType::_Transparent)
        {
            this->_submitObject3DTransparent(material, mesh);
        }
    }
}
void GfxBuiltinQueue::_submitObjectUI(GfxMaterial *material, GfxMesh *mesh)
{
    if (this->_uiBatches.size() <= 0)
    {
        // 创建新的ui批次
        GfxBuiltinBatchUI *uiBatch = new GfxBuiltinBatchUI();
        uiBatch->init(this->_renderer, this->_renderTexture, material, mesh, this->_viewMatrix, this->_projMatrix, this->_cameraPosition);
        this->_uiBatches.push_back(uiBatch);
    }
    else
    {
        GfxBuiltinBatchUI *uiBatch = this->_uiBatches.back();
        if (!material->equals(uiBatch->getMaterial()) || !mesh->equals(uiBatch->getMesh()))
        {
            // 创建新的ui批次
            uiBatch = new GfxBuiltinBatchUI();
            uiBatch->init(this->_renderer, this->_renderTexture, material, mesh, this->_viewMatrix, this->_projMatrix, this->_cameraPosition);
            this->_uiBatches.push_back(uiBatch);
        }
    }
    GfxBuiltinBatchUI *batch = this->_uiBatches.back();
    batch->addObject(material->getInstanceData());
}
void GfxBuiltinQueue::_submitObject3DOpaque(GfxMaterial *material, GfxMesh *mesh)
{

    std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    GfxMaterial *_b_mtl = nullptr;
    for (size_t i = 0; i < this->_3dOpaqueMaterials.size(); i++)
    {
        if (material->equals(this->_3dOpaqueMaterials[i]))
        {
            _b_mtl = this->_3dOpaqueMaterials[i];
            break;
        }
    }
    GfxMesh *_b_mesh = nullptr;
    for (size_t i = 0; i < this->_3dOpaqueMeshes.size(); i++)
    {
        if (mesh->equals(this->_3dOpaqueMeshes[i]))
        {
            _b_mesh = this->_3dOpaqueMeshes[i];
            break;
        }
    }
    std::string key = "";
    if (_b_mtl != nullptr && _b_mesh != nullptr)
    {
        key = _b_mtl->getRendererStateKey() + "_" + _b_mesh->getUuid();
    }
    else
    {
        key = material->getRendererStateKey() + "_" + mesh->getUuid();
    }
    GfxBuiltinBatch3D *batch = nullptr;
    if (this->_3dOpaqueBatches.find(key) == this->_3dOpaqueBatches.end())
    {
        // 没有，创建新的
        batch = new GfxBuiltinBatch3D();
        batch->init(this->_renderer, this->_renderTexture, material, mesh, this->_viewMatrix, this->_projMatrix, this->_cameraPosition);
        this->_3dOpaqueBatches[key] = batch;
        this->_3dOpaqueMaterials.push_back(material);
        this->_3dOpaqueMeshes.push_back(mesh);
    }
    else
    {
        // 找到，添加对象
        batch = this->_3dOpaqueBatches[key];
    }
    batch->addObject(material->getInstanceData());
}
void GfxBuiltinQueue::_submitObject3DTransparent(GfxMaterial *material, GfxMesh *mesh)
{
    GfxBuiltinBatch3D *batch = new GfxBuiltinBatch3D();
    batch->init(this->_renderer, this->_renderTexture, material, mesh, this->_viewMatrix, this->_projMatrix, this->_cameraPosition);
    batch->addObject(material->getInstanceData());
    this->_3dTransparentBatches.push_back(batch);
}
void GfxBuiltinQueue::render(std::vector<VkCommandBuffer> &commandBuffers)
{
    this->_resetCommandBuffer();
    this->_beginCommandBuffer();
    this->_beginRenderPass();
    // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    // std::cout << "render frame batch: " << this->_batchIndex << std::endl;
    // 3d不透明批次
    for (auto &[key, batch] : this->_3dOpaqueBatches)
    {
        batch->render(this->_renderTexture->getCommandBuffer());
    }
    // 3d透明批次
    std::sort(this->_3dTransparentBatches.begin(), this->_3dTransparentBatches.end(), [](GfxBuiltinBatch3D *a, GfxBuiltinBatch3D *b)
              { return a->getDisTransparent() > b->getDisTransparent(); });
    for (size_t i = 0; i < this->_3dTransparentBatches.size(); i++)
    {
        GfxBuiltinBatch3D *batch = this->_3dTransparentBatches[i];
        batch->render(this->_renderTexture->getCommandBuffer());
    }
    // ui批次
    for (size_t i = 0; i < this->_uiBatches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_uiBatches[i];
        batch->render(this->_renderTexture->getCommandBuffer());
    }
    // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    // LOGI("render frame batch duration: %f", frameDuration);

    // 渲染结束
    vkCmdEndRenderPass(this->_renderTexture->getCommandBuffer());
    // 结束渲染pass
    if (vkEndCommandBuffer(this->_renderTexture->getCommandBuffer()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
    commandBuffers.push_back(this->_renderTexture->getCommandBuffer());

    // // 清空批次
    // std::chrono::high_resolution_clock::time_point clearStart = std::chrono::high_resolution_clock::now();
    // 3d不透明批次
    // for (size_t i = 0; i < this->_3dOpaqueBatches.size(); i++)
    // {
    //     GfxBuiltinBatch *batch = this->_3dOpaqueBatches[i];
    //     batch->destroy();
    //     delete batch;
    //     batch = nullptr;
    // }
    for (auto &[key, batch] : this->_3dOpaqueBatches)
    {
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    // 遍历透明
    for (auto *batch : this->_3dTransparentBatches)
    {
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    // ui批次
    for (size_t i = 0; i < this->_uiBatches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_uiBatches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_uiBatches.clear();
    this->_3dOpaqueBatches.clear();
    this->_3dTransparentBatches.clear();
    this->_3dOpaqueMaterials.clear();
    this->_3dOpaqueMeshes.clear();
    // std::chrono::high_resolution_clock::time_point clearEnd = std::chrono::high_resolution_clock::now();
    // float clearDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(clearEnd - clearStart).count() / 1000000.0f;
    // LOGI("render frame batch clear duration: %f", clearDuration);
}
void GfxBuiltinQueue::_resetCommandBuffer()
{
    vkResetCommandBuffer(this->_renderTexture->getCommandBuffer(), 0);
}
void GfxBuiltinQueue::_beginCommandBuffer()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(this->_renderTexture->getCommandBuffer(), &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}
void GfxBuiltinQueue::_beginRenderPass()
{
    GfxBuiltinRenderPass *pass = this->_renderer->getRenderPass();
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.framebuffer = this->_renderTexture->getFramebuffer();
    renderPassInfo.renderArea.extent = {this->_renderTexture->getWidth(), this->_renderTexture->getHeight()};
    renderPassInfo.renderPass = pass->getVKRenderPass();
    renderPassInfo.renderArea.offset = {0, 0};

    std::array<VkClearValue, 2> clearValues = {}; /*  // 至少4个，因为最高索引是3 */
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};
    clearValues[1].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(this->_renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
GfxRenderTexture *GfxBuiltinQueue::getRenderTexture()
{
    return this->_renderTexture;
}

void GfxBuiltinQueue::_clean()
{
}
void GfxBuiltinQueue::_reset()
{
}
void GfxBuiltinQueue::destroy()
{
    for (auto &[key, batch] : this->_3dOpaqueBatches)
    {
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_3dOpaqueBatches.clear();
    this->_3dOpaqueMaterials.clear();
    this->_3dOpaqueMeshes.clear();
    // 透明
    for (auto *batch : this->_3dTransparentBatches)
    {
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_3dTransparentBatches.clear();
    // ui批次
    for (size_t i = 0; i < this->_uiBatches.size(); i++)
    {
        GfxBuiltinBatch *batch = this->_uiBatches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_uiBatches.clear();
    // 销毁rt
    if (this->_renderTexture != nullptr)
    {
        this->_renderTexture->destroy();
        delete this->_renderTexture;
    }
    this->_renderTexture = nullptr;
    this->_renderer = nullptr;
}

GfxBuiltinQueue::~GfxBuiltinQueue()
{
}
