#include "gfx-builtin-renderer.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../base/gfx-shader.h"
#include "../base/gfx-texture.h"
#include "../base/gfx-render-texture.h"
#include "../base/gfx-mesh.h"
#include "../base/gfx-material.h"
#include "../base/gfx-buffer-ubo.h"
#include "../base/gfx-buffer-instance.h"
#include "gfx-builtin-pipeline/gfx-builtin-pipeline.h"
#include "gfx-builtin-pipeline/gfx-builtin-pipeline-ui.h"
#include "gfx-builtin-pipeline/gfx-builtin-pipeline-3d.h"
#include "gfx-builtin-render-pass.h"
#include "gfx-builtin-queue.h"

uint32_t GfxBuiltinRenderer::StencilRef = 0;

GfxBuiltinRenderer::GfxBuiltinRenderer(std::string name)
{
    this->_name = name;
}
void GfxBuiltinRenderer::init()
{
    LOGI("[Gfx : RendererBuiltin] :: init %s", this->_name.c_str());
    this->_initDescriptorSetLayout();
    this->_initDescriptorSets();
    this->_initDefaultRenderPass();
}
void GfxBuiltinRenderer::_initDescriptorSetLayout()
{
    this->_initUIDescriptorSetLayout();
    this->_init3DDescriptorSetLayout();
}
void GfxBuiltinRenderer::_initUIDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    // 采样器绑定
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // ubo绑定
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    bindings.push_back(uboLayoutBinding);

    // 采样器绑定
    // 4个采样器绑定（binding 1-4）
    for (uint32_t i = 0; i < 4; i++)
    {
        VkDescriptorSetLayoutBinding samplerBinding{};
        samplerBinding.binding = i + 1; // binding 1, 2, 3, 4
        samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding.descriptorCount = 1;
        samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings.push_back(samplerBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(Gfx::_context->getVkDevice(),
                                    &layoutInfo, nullptr, &this->_uiDescriptorSetLayout) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor set layout failed ");
        return;
    }
    LOGI("[Gfx : RendererBuiltin] :: create descriptor set layout success ");
}
void GfxBuiltinRenderer::_init3DDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    // 采样器绑定
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // ubo绑定
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    bindings.push_back(uboLayoutBinding);

    // 采样器绑定
    // 10个采样器绑定（binding 1-10）
    for (uint32_t i = 0; i < 10; i++)
    {
        VkDescriptorSetLayoutBinding samplerBinding{};
        samplerBinding.binding = i + 1; // binding 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
        samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding.descriptorCount = 1;
        samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings.push_back(samplerBinding);
    }
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(Gfx::_context->getVkDevice(),
                                    &layoutInfo, nullptr, &this->_3dDescriptorSetLayout) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor set layout failed ");
        return;
    }
    LOGI("[Gfx : RendererBuiltin] :: create descriptor set layout success ");
}
void GfxBuiltinRenderer::_initDescriptorSets()
{
    this->_initUIDescriptorSets();
    this->_init3DDescriptorSets();
}
void GfxBuiltinRenderer::_initUIDescriptorSets()
{

    int maxObjectCount = 2048;
    int samplerCount = 4;

    std::vector<VkDescriptorPoolSize> poolSizes(2);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = (maxObjectCount + 3); // 支持更多

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // 总采样器数量 = 每个集的采样器数 × 集的数量
    poolSizes[1].descriptorCount = (maxObjectCount + 3) * samplerCount; // 单个描述符集3

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = (maxObjectCount + 3); // 描述符集的最大数量
    if (vkCreateDescriptorPool(Gfx::_context->getVkDevice(), &poolInfo, nullptr, &this->_uiDescriptorPool) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor pool failed ");
        return;
    }
    // LOGI("[Gfx : RendererBuiltin] :: create descriptor pool success ");
}
void GfxBuiltinRenderer::_init3DDescriptorSets()
{
    int maxObjectCount = 20480;
    int samplerCount = 10;

    std::vector<VkDescriptorPoolSize> poolSizes(2);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = (maxObjectCount + 3); // 支持更多

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // 总采样器数量 = 每个集的采样器数 × 集的数量
    poolSizes[1].descriptorCount = (maxObjectCount + 3) * samplerCount; // 单个描述符集3

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = (maxObjectCount + 3); // 描述符集的最大数量
    if (vkCreateDescriptorPool(Gfx::_context->getVkDevice(), &poolInfo, nullptr, &this->_3dDescriptorPool) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor pool failed ");
        return;
    }
    // LOGI("[Gfx : RendererBuiltin] :: create descriptor pool success ");
}
VkDescriptorSet GfxBuiltinRenderer::getUIDescriptorSet()
{
    for (auto &descriptorSet : this->_gfxUIDescriptorSets)
    {
        if (!descriptorSet.isOccupied)
        {
            descriptorSet.isOccupied = true;
            return descriptorSet.descriptorSet;
        }
    }
    if (this->_gfxUIDescriptorSets.size() >= 2000)
    {
        return this->_gfxUIDescriptorSets.back().descriptorSet;
    }

    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    std::vector<VkDescriptorSetLayout> layouts(1, this->_uiDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; // VK_STRUCTURE_TYPE_DESCRIPT1OR_SET_ALLOCATE_INFO
    allocInfo.descriptorPool = this->_uiDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts.data();
    if (vkAllocateDescriptorSets(Gfx::_context->getVkDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor sets failed ");
    }
    this->_gfxUIDescriptorSets.push_back({descriptorSet, true});
    return descriptorSet;
}
VkDescriptorSet GfxBuiltinRenderer::get3DDescriptorSet()
{
    for (auto &descriptorSet : this->_gfx3dDescriptorSets)
    {
        if (!descriptorSet.isOccupied)
        {
            descriptorSet.isOccupied = true;
            return descriptorSet.descriptorSet;
        }
    }
    if (this->_gfx3dDescriptorSets.size() >= 20000)
    {
        return this->_gfx3dDescriptorSets.back().descriptorSet;
    }
    // 3D描述符集
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    std::vector<VkDescriptorSetLayout> layouts(1, this->_3dDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; // VK_STRUCTURE_TYPE_DESCRIPT1OR_SET_ALLOCATE_INFO
    allocInfo.descriptorPool = this->_3dDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts.data();
    if (vkAllocateDescriptorSets(Gfx::_context->getVkDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS)
    {
        LOGI("[Gfx : RendererBuiltin] :: create descriptor sets failed ");
    }
    this->_gfx3dDescriptorSets.push_back({descriptorSet, true});
    return descriptorSet;
}

/**
 * @brief 创建内置默认的ui pass
 */
void GfxBuiltinRenderer::_initDefaultRenderPass()
{
    this->_pass = new GfxBuiltinRenderPass("builtin");
}
GfxBuiltinRenderPass *GfxBuiltinRenderer::getRenderPass()
{
    return this->_pass;
}

void GfxBuiltinRenderer::createPipeline(std::string name, GfxRendererState rendererState)
{
    if (this->_pipelines.find(name) != this->_pipelines.end())
    {
        return;
    }
    if (Gfx::_shaders.find(rendererState.vert) == Gfx::_shaders.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: createPipeline:vert not found:%s", rendererState.vert.c_str());
        return;
    }
    if (Gfx::_shaders.find(rendererState.frag) == Gfx::_shaders.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: createPipeline:frag not found:%s", rendererState.frag.c_str());
        return;
    }
    if (this->_pass == nullptr)
    {
        LOGI("[Gfx : RendererBuiltin] :: createPipeline:pass not found:");
        return;
    }
    if (rendererState.layer == GfxRendererLayer::_UI)
    {
        GfxBuiltinPipelineUI *pipeline = new GfxBuiltinPipelineUI(name);
        pipeline->create(this->_pass, Gfx::_shaders[rendererState.vert], Gfx::_shaders[rendererState.frag], this->_uiDescriptorSetLayout, rendererState);
        this->_pipelines[name] = pipeline;
    }
    else if (rendererState.layer == GfxRendererLayer::_3D)
    {
        GfxBuiltinPipeline3D *pipeline = new GfxBuiltinPipeline3D(name);
        pipeline->create(this->_pass, Gfx::_shaders[rendererState.vert], Gfx::_shaders[rendererState.frag], this->_3dDescriptorSetLayout, rendererState);
        this->_pipelines[name] = pipeline;
    }
}

GfxBuiltinPipeline *GfxBuiltinRenderer::getPipeline(const GfxRendererState &pipelineState)
{
    std::string name = pipelineState.generateKey();
    this->createPipeline(name, pipelineState);
    if (this->_pipelines.find(name) != this->_pipelines.end())
    {
        GfxBuiltinPipeline *pipeline = this->_pipelines[name];
        return pipeline;
    }
    // LOGI("[Gfx : RendererBuiltin] :: getPipeline:not found:%s", name.c_str());
    return nullptr;
}
void GfxBuiltinRenderer::createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height)
{
    if (this->_renderQueues.find(renderId) != this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: createRenderQueue:renderId already exists:%s", renderId.c_str());
        return;
    }
    GfxBuiltinQueue *queue = new GfxBuiltinQueue(renderId, width, height, this);
    queue->init();
    queue->setPriority(priority);
    this->_renderQueues[renderId] = queue;
}
void GfxBuiltinRenderer::setRenderQueuePriority(std::string renderId, int priority)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: setRenderQueuePriority:renderId not found:%s", renderId.c_str());
        return;
    }
    this->_renderQueues[renderId]->setPriority(priority);
}
GfxRenderTexture *GfxBuiltinRenderer::getRenderQueueRT(std::string renderId)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: getRenderQueueRT:renderId not found:%s", renderId.c_str());
        return nullptr;
    }
    return this->_renderQueues[renderId]->getRenderTexture();
}
void GfxBuiltinRenderer::resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: resizeRenderQueue:renderId not found:%s", renderId.c_str());
        return;
    }
    this->_renderQueues[renderId]->resize(width, height);
}
void GfxBuiltinRenderer::delRenderQueue(std::string renderId)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: delRenderQueue:renderId not found:%s", renderId.c_str());
        return;
    }
    this->_destroyQueueCaches.push_back(this->_renderQueues[renderId]);
    this->_renderQueues.erase(renderId);
}
void GfxBuiltinRenderer::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: submitRenderMat:renderId not found:%s", renderId.c_str());
        return;
    }
    this->_renderQueues[renderId]->submitData(viewMatrix, projMatrix, isOnScreen, cameraPosition);
}
void GfxBuiltinRenderer::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        LOGI("[Gfx : RendererBuiltin] :: submitRenderObject:renderId not found:%s", renderId.c_str());
        return;
    }
    if (material == nullptr)
    {
        LOGI("[Gfx : RendererBuiltin] :: submitRenderObject:material is null:%s", renderId.c_str());
        return;
    }
    if (mesh == nullptr)
    {
        LOGI("[Gfx : RendererBuiltin] :: submitRenderObject:mesh is null:%s", renderId.c_str());
        return;
    }
    this->_renderQueues[renderId]->submitObject(material, mesh);
}
void GfxBuiltinRenderer::getOffScreenOutds(std::vector<std::string> &pipelineOutds)
{
    // 将_renderQueues按priority排序
    std::vector<std::string> renderQueueIds;
    for (auto &renderQueue : this->_renderQueues)
    {
        renderQueueIds.push_back(renderQueue.first);
    }
    std::sort(renderQueueIds.begin(), renderQueueIds.end(), [this](const std::string &a, const std::string &b)
              { return this->_renderQueues[a]->getPriority() < this->_renderQueues[b]->getPriority(); });
    // 遍历渲染队列，将isOnScreen为true的渲染队列添加到pipelineOutds中
    for (auto &renderQueueId : renderQueueIds)
    {
        GfxBuiltinQueue *renderQueue = this->_renderQueues[renderQueueId];
        if (!renderQueue->getIsOnScreen())
        {
            continue;
        }
        GfxRenderTexture *renderTexture = renderQueue->getRenderTexture();
        if (renderTexture == nullptr)
        {
            continue;
        }
        const std::string &renderTargetUid = renderTexture->getColorTextureUuid();
        if (renderTargetUid.empty())
        {
            continue;
        }
        pipelineOutds.push_back(renderTargetUid);
    }
}

void GfxBuiltinRenderer::frameRendererBefore()
{
    for (auto &descriptorSet : this->_gfxUIDescriptorSets)
    {
        descriptorSet.isOccupied = false;
    }
    for (auto &descriptorSet : this->_gfx3dDescriptorSets)
    {
        descriptorSet.isOccupied = false;
    }
    // 销毁渲染队列缓存
    for (auto &renderQueue : this->_destroyQueueCaches)
    {
        renderQueue->destroy();
        delete renderQueue;
        renderQueue = nullptr;
    }
    this->_destroyQueueCaches.clear();
    GfxBuiltinRenderer::StencilRef = 0;
}
void GfxBuiltinRenderer::frameRenderer(std::vector<VkCommandBuffer> &commandBuffers)
{
    for (auto &renderQueue : this->_renderQueues)
    {
        GfxBuiltinQueue *queue = renderQueue.second;
        if (queue != nullptr)
        {
            renderQueue.second->render(commandBuffers);
        }
    }
}
void GfxBuiltinRenderer::frameRendererAfter()
{
    GfxBuiltinRenderer::StencilRef = 0;
}

void GfxBuiltinRenderer::_cleanRendererState()
{
}
void GfxBuiltinRenderer::_resetRendererState()
{
}
GfxBuiltinRenderer::~GfxBuiltinRenderer()
{
}