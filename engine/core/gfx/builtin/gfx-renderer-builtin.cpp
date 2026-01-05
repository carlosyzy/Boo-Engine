#include "gfx-renderer-builtin.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../base/gfx-shader.h"
#include "../base/gfx-shader-struct.h"
#include "../base/gfx-texture.h"
#include "../base/gfx-render-texture.h"
#include "../base/gfx-mesh.h"
#include "../base/gfx-material.h"
#include "../base/gfx-buffer-ubo.h"
#include "../base/gfx-buffer-instance.h"
#include "../../math/mat4.h"
#include "gfx-render-pass-builtin.h"
#include "gfx-pipeline-builtin.h"
#include "gfx-queue-builtin.h"

uint32_t GfxRendererBuiltin::StencilRef = 0;

GfxRendererBuiltin::GfxRendererBuiltin(std::string name)
{
    this->_name = name;
}
void GfxRendererBuiltin::init()
{
    std::cout << "[Gfx : GfxRendererBuiltin] :: init " << this->_name << std::endl;
    this->_initDescriptorSetLayout();
    this->_initDescriptorSets();
    this->_initDefaultRenderPass();
    this->_initDefaultShader();
    this->_initDefaultPipeline();
}
void GfxRendererBuiltin::_initDescriptorSetLayout()
{
    this->_initUIDescriptorSetLayout();
    this->_init3DDescriptorSetLayout();
}
void GfxRendererBuiltin::_initUIDescriptorSetLayout()
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
    if (vkCreateDescriptorSetLayout(Gfx::context->getVkDevice(),
                                    &layoutInfo, nullptr, &this->_uiDescriptorSetLayout) != VK_SUCCESS)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor set layout failed " << std::endl;
        return;
    }
    std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor set layout success " << std::endl;
}
void GfxRendererBuiltin::_init3DDescriptorSetLayout()
{
}
void GfxRendererBuiltin::_initDescriptorSets()
{
    this->_initUIDescriptorSets();
    this->_init3DDescriptorSets();
}
void GfxRendererBuiltin::_initUIDescriptorSets()
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
    if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_uiDescriptorPool) != VK_SUCCESS)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor pool failed " << std::endl;
        return;
    }
    std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor pool success " << std::endl;
}
void GfxRendererBuiltin::_init3DDescriptorSets()
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
    if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_uiDescriptorPool) != VK_SUCCESS)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor pool failed " << std::endl;
        return;
    }
    std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor pool success " << std::endl;
}

/**
 * @brief 创建内置默认的ui pass
 */
void GfxRendererBuiltin::_initDefaultRenderPass()
{
    this->_pass = new GfxRenderPassBuiltin("builtin");
}
GfxRenderPassBuiltin *GfxRendererBuiltin::getRenderPass()
{
    return this->_pass;
}
/**
 * 创建内置默认的ui shader
 */
void GfxRendererBuiltin::_initDefaultShader()
{
    std::string shaderVertName = "builtin-ui.vert";
    GfxShader *shader = new GfxShader(shaderVertName);
    shader->createShaderModule(GfxShaderUIVertSPV, GfxShaderUIVertSPVSize);
    Gfx::shaders[shaderVertName] = shader;

    std::string shaderFragName = "builtin-ui.frag";
    shader = new GfxShader(shaderFragName);
    shader->createShaderModule(GfxShaderUIFragSPV, GfxShaderUIFragSPVSize);
    Gfx::shaders[shaderFragName] = shader;

    // 内置默认ui 遮罩 顶点着色器
    std::string shaderMaskVertName = "builtin-ui-mask.vert";
    shader = new GfxShader(shaderMaskVertName);
    shader->createShaderModule(GfxShaderUIMaskVertSPV, GfxShaderUIMaskVertSPVSzie);
    Gfx::shaders[shaderMaskVertName] = shader;

    // 内置默认ui 遮罩 片元着色器
    std::string shaderMaskFragName = "builtin-ui-mask.frag";
    shader = new GfxShader(shaderMaskFragName);
    shader->createShaderModule(GfxShaderUIMaskFragSPV, GfxShaderUIMaskFragSPVSzie);
    Gfx::shaders[shaderMaskFragName] = shader;
}
/**
 * 创建内置默认的ui pipeline
 */
void GfxRendererBuiltin::_initDefaultPipeline()
{
    this->_initDefaultUIPipeline();
    this->_initDefaultUIMaskAddPipeline();
    this->_initDefaultUIMaskSubPipeline();
}
void GfxRendererBuiltin::_initDefaultUIPipeline()
{
    GfxPipelineStruct uiPipeline = {};
    uiPipeline.render = uint32_t(GfxPipelineRender::_UI);
    uiPipeline.vert = "builtin-ui.vert";
    uiPipeline.frag = "builtin-ui.frag";
    // 多边形模式 填充
    uiPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiPipeline.cullMode = GfxPipelineCullMode::None;
    // 深度测试 开启
    uiPipeline.depthTest = 0;
    // 深度写入 开启
    uiPipeline.depthWrite = 0;
    // 深度比较操作 小于等于
    uiPipeline.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 启用（用于UI遮罩）
    uiPipeline.stencilTest = 1;
    uiPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
    uiPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
    uiPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
    uiPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
    uiPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
    uiPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiPipeline.stencilBackPassOp = GfxPipelineStencilOp::Keep;
    // 颜色混合 开启
    uiPipeline.colorBlend = 1;
    uiPipeline.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
    uiPipeline.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipeline.colorBlendOp = GfxPipelineColorBlendOp::Add;
    uiPipeline.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    uiPipeline.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipeline.alphaBlendOp = GfxPipelineColorBlendOp::Add;
    // 推送常量 开启
    uiPipeline.pushConstant = 1;
    uiPipeline.pushConstantSize = 0;
    this->createPipeline(uiPipeline.generateKey(), uiPipeline);
}

void GfxRendererBuiltin::_initDefaultUIMaskAddPipeline()
{
    GfxPipelineStruct uiMaskAddPipeline = {};
    uiMaskAddPipeline.render = uint32_t(GfxPipelineRender::_UI);
    uiMaskAddPipeline.vert = "builtin-ui-mask.vert";
    uiMaskAddPipeline.frag = "builtin-ui-mask.frag";
    // 多边形模式 填充
    uiMaskAddPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiMaskAddPipeline.cullMode = GfxPipelineCullMode::None;
    // 深度测试 开启
    uiMaskAddPipeline.depthTest = 0;
    // 深度写入 开启
    uiMaskAddPipeline.depthWrite = 0;
    // 深度比较操作 小于等于
    uiMaskAddPipeline.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 启用（用于UI遮罩）
    uiMaskAddPipeline.stencilTest = 1;
    uiMaskAddPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Always;     // 总是通过测试
    uiMaskAddPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;          // 测试失败：保持
    uiMaskAddPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;     // 深度失败：保持
    uiMaskAddPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add; // 测试通过：替换模板值为1（表示遮罩区域）
    uiMaskAddPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Always;
    uiMaskAddPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipeline.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add; // 测试通过：替换模板值为1（表示遮罩区域）

    uiMaskAddPipeline.colorBlend = 0;
    
    // 推送常量 开启
    uiMaskAddPipeline.pushConstant = 1;
    uiMaskAddPipeline.pushConstantSize = 0;
    this->createPipeline(uiMaskAddPipeline.generateKey(), uiMaskAddPipeline);
}
void GfxRendererBuiltin::_initDefaultUIMaskSubPipeline()
{
    GfxPipelineStruct uiMaskAddPipeline = {};
    uiMaskAddPipeline.render = uint32_t(GfxPipelineRender::_UI);
    uiMaskAddPipeline.vert = "builtin-ui-mask.vert";
    uiMaskAddPipeline.frag = "builtin-ui-mask.frag";
    // 多边形模式 填充
    uiMaskAddPipeline.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiMaskAddPipeline.cullMode = GfxPipelineCullMode::None;
    // 深度测试 开启
    uiMaskAddPipeline.depthTest = 0;
    // 深度写入 开启
    uiMaskAddPipeline.depthWrite = 0;
    // 深度比较操作 小于等于
    uiMaskAddPipeline.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 启用（用于UI遮罩）
    uiMaskAddPipeline.stencilTest = 1;
    uiMaskAddPipeline.stencilFrontCompareOp = GfxPipelineCompareOp::Always;          // 总是通过测试
    uiMaskAddPipeline.stencilFrontFailOp = GfxPipelineStencilOp::Keep;               // 测试失败：保持
    uiMaskAddPipeline.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;          // 深度失败：保持
    uiMaskAddPipeline.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract; // 测试通过：替换模板值为1（表示遮罩区域）
    uiMaskAddPipeline.stencilBackCompareOp = GfxPipelineCompareOp::Always;
    uiMaskAddPipeline.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipeline.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipeline.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract; // 测试通过：替换模板值为1（表示遮罩区域）

    uiMaskAddPipeline.colorBlend = 0;

    // 推送常量 开启
    uiMaskAddPipeline.pushConstant = 1;
    uiMaskAddPipeline.pushConstantSize = 0;
    this->createPipeline(uiMaskAddPipeline.generateKey(), uiMaskAddPipeline);
}

void GfxRendererBuiltin::createPipeline(std::string name, GfxPipelineStruct pipelineStruct)
{
    if (Gfx::shaders.find(pipelineStruct.vert) == Gfx::shaders.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: createPipeline:vert not found:" << pipelineStruct.vert << std::endl;
        return;
    }
    if (Gfx::shaders.find(pipelineStruct.frag) == Gfx::shaders.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: createPipeline:frag not found:" << pipelineStruct.frag << std::endl;
        return;
    }
    if (this->_pass == nullptr)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: createPipeline:pass not found:" << std::endl;
        return;
    }
    if (this->_pipelines.find(name) != this->_pipelines.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: createPipeline:name already exists:" << name << std::endl;
        return;
    }
    GfxPipelineBuiltin *pipeline = new GfxPipelineBuiltin(name);
    if (pipelineStruct.render == uint32_t(GfxPipelineRender::_UI))
    {
        pipeline->create(this->_pass, Gfx::shaders[pipelineStruct.vert], Gfx::shaders[pipelineStruct.frag], this->_uiDescriptorSetLayout, pipelineStruct);
        this->_pipelines[name] = pipeline;
        std::cout << "[Gfx : GfxRendererBuiltin] :: createPipeline:ui pipeline created:" << name << std::endl;
    }
    else if (pipelineStruct.render == uint32_t(GfxPipelineRender::_3D))
    {
        pipeline->create(this->_pass, Gfx::shaders[pipelineStruct.vert], Gfx::shaders[pipelineStruct.frag], this->_3DDescriptorSetLayout, pipelineStruct);
        this->_pipelines[name] = pipeline;
    }
}

GfxPipelineBuiltin *GfxRendererBuiltin::getPipeline(std::string name)
{
    if (this->_pipelines.find(name) == this->_pipelines.end())
    {
        std::cout << "getPipeline:not found:" << name << std::endl;
        return nullptr;
    }
    return this->_pipelines[name];
}
VkDescriptorSet GfxRendererBuiltin::getUIDescriptorSet()
{
    // std::cout << "GfxRendererBuiltin::getUIDescriptorSet:" << this->_gfxUIDescriptorSets.size() << std::endl;
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
    if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: create descriptor sets failed " << std::endl;
    }
    this->_gfxUIDescriptorSets.push_back({descriptorSet, true});
    return descriptorSet;
}
void GfxRendererBuiltin::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture)
{
    if (this->_renderQueues.find(renderId) != this->_renderQueues.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: initRenderQueue:renderId already exists:" << renderId << std::endl;
        return;
    }
    // 绑定渲染pass
    renderTexture->bindRenderPass(this->_pass);
    // 创建渲染队列
    GfxQueueBuiltin *queue = new GfxQueueBuiltin(this, renderTexture);
    queue->init();
    this->_renderQueues[renderId] = queue;
}
void GfxRendererBuiltin::delRenderQueue(std::string renderId)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: delRenderQueue:renderId not found:" << renderId << std::endl;
        return;
    }
    this->_renderQueues[renderId]->destroy();
    // 销毁渲染队列
    delete this->_renderQueues[renderId];
    this->_renderQueues.erase(renderId);
}
void GfxRendererBuiltin::submitRenderMat(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: submitRenderMat:renderId not found:" << renderId << std::endl;
        return;
    }
    this->_renderQueues[renderId]->submitMat(viewMatrix, projMatrix);
}
void GfxRendererBuiltin::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData)
{
    if (this->_renderQueues.find(renderId) == this->_renderQueues.end())
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: submitRenderObject:renderId not found:" << renderId << std::endl;
        return;
    }
    if (material == nullptr)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: submitRenderObject:material is null:" << renderId << std::endl;
        return;
    }
    if (mesh == nullptr)
    {
        std::cout << "[Gfx : GfxRendererBuiltin] :: submitRenderObject:mesh is null:" << renderId << std::endl;
        return;
    }
    this->_renderQueues[renderId]->submitObject(material, mesh, instanceData);
}
void GfxRendererBuiltin::getOffScreenOutds(std::vector<std::string> &pipelineOutds)
{
    for (auto &renderQueue : this->_renderQueues)
    {
        GfxRenderTexture *renderTexture = renderQueue.second->getRenderTexture();
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

void GfxRendererBuiltin::frameRendererBefore()
{
    for (auto &descriptorSet : this->_gfxUIDescriptorSets)
    {
        descriptorSet.isOccupied = false;
    }
    for (auto &descriptorSet : this->_gfx3DDescriptorSets)
    {
        descriptorSet.isOccupied = false;
    }
}
void GfxRendererBuiltin::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    for (auto &renderQueue : this->_renderQueues)
    {
        renderQueue.second->render(commandBuffers);
    }
}
void GfxRendererBuiltin::frameRendererAfter()
{
}

void GfxRendererBuiltin::_cleanRendererState()
{
    // // 清除渲染队列相关的状态
    // this->_queue->_clean();
    // //  渲染管线清除
    // this->_pipeline->_clear();
    // // 渲染pass清除
    // this->_pass->_clear();
    // // 描述符池清除
    // if (this->_descriptorPool != VK_NULL_HANDLE)
    // {
    //     vkDestroyDescriptorPool(Gfx::context->getVkDevice(), this->_descriptorPool, nullptr);
    //     this->_descriptorPool = nullptr;
    // }
    // this->_descriptorSets.clear();
}
void GfxRendererBuiltin::_resetRendererState()
{
    // this->_initDefaultDescriptor();
    // this->_pass->_reset();
    // this->_pipeline->_reset();
    // this->_queue->_reset();
}
// void GfxRendererBuilt::_initDescriptor()
// {
//     this->_initDescriptorPool();
//     this->_initDescriptorSetLayout();
//     this->_initDescriptorSets();
// }
// void GfxRendererBuilt::_initDescriptorPool()
// {
//     // std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();

//     // std::vector<VkDescriptorPoolSize> poolSizes(3);
//     // // 1. UBO数量计算
//     // // 每个帧：2个UBO（3D UBO + UI UBO）
//     // // 可能需要额外：阴影UBO、后处理UBO
//     // poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     // poolSizes[0].descriptorCount = 30 * swapChainImageViews.size(); // 支持更多
//     // // 2. 存储缓冲区数量计算
//     // // 每个帧可能需要的存储缓冲区：
//     // // - 模型数据buffer（所有物体的变换）
//     // // - 材质数据buffer
//     // // - 灯光数据buffer
//     // // - 实例数据buffer
//     // poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
//     // poolSizes[1].descriptorCount = 300 * swapChainImageViews.size(); // 支持更多
//     // // 3. Bindless纹理：关键是这个要足够大！
//     // poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     // // ⚠️ 临时修复：增加容量（但这不是最佳方案，见下方建议）
//     // poolSizes[2].descriptorCount = 500 * swapChainImageViews.size();

//     // // 4. maxSets计算：需要多少个描述符集？
//     // // 每个帧需要：
//     // // - 3D渲染描述符集
//     // // - UI渲染描述符集
//     // // - 可能还有：阴影Pass描述符集、后处理描述符集
//     // // int descriptorSetsPerFrame = 400;          // 3D + UI（最基本）
//     // // uint32_t maxSets = descriptorSetsPerFrame; // 6个描述符集

//     // VkDescriptorPoolCreateInfo poolInfo{};
//     // poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//     // // ✓ 关键：添加 UPDATE_AFTER_BIND 标志（Bindless 必需）
//     // poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
//     // poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//     // poolInfo.pPoolSizes = poolSizes.data();
//     // poolInfo.maxSets = 10 * swapChainImageViews.size(); // 支持更多描述符集

//     // if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
//     // {
//     //     throw std::runtime_error("failed to create descriptor pool!");
//     // }

//     // std::cout << "Gfx : Renderer ::   Created descriptor pool with UPDATE_AFTER_BIND support" << std::endl;
//     // std::cout << "Gfx : Renderer ::   UBO descriptors: " << poolSizes[0].descriptorCount << std::endl;
//     // std::cout << "Gfx : Renderer ::   Storage Buffer descriptors: " << poolSizes[1].descriptorCount << std::endl;
//     // std::cout << "Gfx : Renderer ::   Texture descriptors: " << poolSizes[2].descriptorCount << std::endl;
// }
// void GfxRendererBuilt::_initDescriptorSetLayout()
// {
//     // // 动态 UBO 研究
//     // std::vector<VkDescriptorSetLayoutBinding> bindings;
//     // // ubo缓冲区绑定  -全局统一属性
//     // VkDescriptorSetLayoutBinding uboLayoutBinding{};
//     // uboLayoutBinding.binding = 0;
//     // uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     // uboLayoutBinding.descriptorCount = 1;
//     // uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//     // bindings.push_back(uboLayoutBinding);
//     // // 使用存储缓冲区绑定 - 物体单独属性
//     // VkDescriptorSetLayoutBinding ssboBinding = {};
//     // ssboBinding.binding = 1; // 对应shader中的 binding = 0
//     // ssboBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
//     // ssboBinding.descriptorCount = 1;
//     // ssboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |   // 顶点着色器可以访问
//     //                          VK_SHADER_STAGE_FRAGMENT_BIT | // 片段着色器可以访问
//     //                          VK_SHADER_STAGE_COMPUTE_BIT;   // 计算着色器可以访问
//     // bindings.push_back(ssboBinding);
//     // // 绑定纹理数组采样器
//     // VkDescriptorSetLayoutBinding textureArrayBinding{};
//     // textureArrayBinding.binding = 2;
//     // textureArrayBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     // textureArrayBinding.descriptorCount = 500; // 你的池子大小
//     // textureArrayBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//     // bindings.push_back(textureArrayBinding);

//     // // ==================== 启用Bindless扩展 ====================
//     // // 需要这些扩展标志
//     // std::vector<VkDescriptorBindingFlags> bindingFlags(bindings.size(), 0);
//     // bindingFlags[0] = 0;
//     // bindingFlags[1] = 0;
//     // // 为纹理数组启用部分绑定和更新后绑定
//     // bindingFlags[2] = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT |
//     //                   VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT |
//     //                   VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT; // 新增

//     // // VkDescriptorSetLayoutBindingFlagsCreateInfoEXT bindingFlagsInfo;
//     // // bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
//     // // bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
//     // // bindingFlagsInfo.pBindingFlags = bindingFlags.data();
//     // VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{};
//     // bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
//     // bindingFlagsInfo.pNext = nullptr; // 如果还有其他扩展链，需要正确设置
//     // bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
//     // bindingFlagsInfo.pBindingFlags = bindingFlags.data();

//     // VkDescriptorSetLayoutCreateInfo layoutInfo{};
//     // layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//     // layoutInfo.pNext = &bindingFlagsInfo;                                              // 链接扩展结构
//     // layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT; // 注意：这里使用 EXT 后缀
//     // layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//     // layoutInfo.pBindings = bindings.data();

//     // std::cout << "Gfx : Renderer ::   Texture Array Descriptors: " << textureArrayBinding.descriptorCount << std::endl;
//     // std::cout << "Gfx : Renderer ::   Binding Flags: " << bindingFlags[2] << std::endl;
//     // std::cout << "Gfx : Renderer ::   getVkDevice: " << Gfx::context->getVkDevice() << std::endl;

//     // if (vkCreateDescriptorSetLayout(Gfx::context->getVkDevice(),
//     //                                 &layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
//     // {
//     //     std::cout << "Gfx : Renderer :: create descriptor set layout failed " << std::endl;
//     //     return;
//     // }
//     // std::cout << "Gfx : Renderer :: create descriptor set layout success " << std::endl;
// }
// void GfxRendererBuilt::_initDescriptorSets()
// {
//     // // 每个帧需要一个描述符集
//     // std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();

//     // std::vector<VkDescriptorSetLayout> layouts(swapChainImageViews.size(), this->_descriptorSetLayout);

//     // // 关键：设置可变描述符数量信息
//     // VkDescriptorSetVariableDescriptorCountAllocateInfoEXT countInfo = {};
//     // countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
//     // countInfo.descriptorSetCount = swapChainImageViews.size();
//     // // 每个描述符集实际使用的纹理数量（可以动态设置）
//     // std::vector<uint32_t> counts(swapChainImageViews.size(), 500); // 每个集最多500个纹理
//     // countInfo.pDescriptorCounts = counts.data();

//     // VkDescriptorSetAllocateInfo allocInfo{};
//     // allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     // allocInfo.pNext = &countInfo; // 链接可变数量信息
//     // allocInfo.descriptorPool = this->_descriptorPool;
//     // allocInfo.descriptorSetCount = swapChainImageViews.size();
//     // allocInfo.pSetLayouts = layouts.data();
//     // this->_descriptorSets.resize(swapChainImageViews.size());
//     // if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
//     // {
//     //     throw std::runtime_error("Failed to allocate descriptor sets!");
//     // }
//     // std::cout << "Gfx : Renderer :: create descriptor sets success..." << std::endl;
// }
// void GfxRendererBuilt::createObject(std::string id, std::string passName, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_Log("createGfxObject:id already exists");
//     //     return;
//     // }
//     // GfxObject *object = new GfxObject(this->_context);
//     // if(this->_pipelines.find("ui-mask.mtl") == this->_pipelines.end()){
//     //     this->_Log("createGfxObject:ui-mask pipeline not found,Currently, creation is not supported");
//     //     return;
//     // }
//     // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
//     // object->setVertexs(points, colors, normals, uvs, indices);
//     // this->_objects[id] = object;
//     // this->_queues[passName]->submit(object);

//     // if (this->_passes.find(passName) != this->_passes.end())
//     // {

//     //     object->setPass(this->_passes[passName]);
//     // }
//     // else
//     // {
//     //     this->_Log("createGfxObject:renderPassType not found,Currently, creation is not supported");
//     // }
//     // /*  if (this->_pipelines.find(pipelineType) == this->_pipelines.end())
//     //   {
//     //       this->_Log("createGfxObject:pipelineType not found,Currently, creation is not supported");
//     //       return;
//     //   }
//     //   else
//     //   {
//     //       object->setPipeline(this->_pipelines[pipelineType]);
//     //   }*/
// }
// void GfxRendererBuilt::createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: createUIObject:id already exists" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = new GfxObject(id, GfxObjectType::UI, this->_context);
//     // object->setVertexs(points, colors, normals, uvs, indices);
//     // this->_objects[id] = object;
//     // // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
//     // // this->_queues["ui-pass"]->submit(object);
// }
// void GfxRendererBuilt::createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: createUIMaskObject:id already exists" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = new GfxObject(id, GfxObjectType::UI_MASK, this->_context);
//     // object->setVertexs(points, colors, normals, uvs, indices);
//     // this->_objects[id] = object;
//     // // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
//     // // this->_queues["ui-pass"]->submit(object);
// }
// void GfxRendererBuilt::setObjectPass(std::string id, std::string pass)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: setObjectPass:object id not found" << std::endl;
//     //     return;
//     // }
//     // if (this->_passes.find(pass) == this->_passes.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: setObjectPass:pass not found" << std::endl;
//     //     return;
//     // }
//     // this->_objects[id]->setPass(this->_passes[pass]);
// }

// void GfxRendererBuilt::setObjectPipeline(std::string id, std::string pipeline)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: setObjectPipeline:object id not found" << std::endl;
//     //     return;
//     // }
//     // if (this->_pipelines.find(pipeline) == this->_pipelines.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: setObjectPipeline:pipeline not found" << std::endl;
//     //     return;
//     // }
//     // this->_objects[id]->setPipeline(this->_pipelines[pipeline]);
// }
// /**
//  * @brief 设置UI遮罩行为
//  *
//  * @param id 物体ID
//  * @param behavior 行为 0 不遮罩 1 遮罩
//  */
// void GfxRendererBuilt::setObjectUIMaskBehavior(std::string id, uint32_t behavior)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt: setObjectUIMaskBehavior:object id not found" << std::endl;
//     //     return;
//     // }
//     // this->_objects[id]->setUIMaskBehavior(behavior);
// }

// void GfxRendererBuilt::setObjectTexture(const std::string &id, const std::string &texture)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     if (this->_textures.find(texture) == this->_textures.end())
//     //     {
//     //         this->_Log("setObjectTexture:texture not found");
//     //         return;
//     //     }
//     //     this->_objects[id]->setTexture(this->_textures[texture]);
//     //     return;
//     // }
// }
// void GfxRendererBuilt::setObjectColor(std::string id, float r, float g, float b, float a)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setColor(r, g, b, a);
//     //     return;
//     // }
// }
// void GfxRendererBuilt::setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setModelMatrix(modelMatrix);
//     //     return;
//     // }
// }
// void GfxRendererBuilt::setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setViewMatrix(viewMatrix);
//     //     return;
//     // }
// }
// void GfxRendererBuilt::setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setProjMatrix(projMatrix);
//     //     return;
//     // }
// }
// void GfxRendererBuilt::destroyObject(std::string id)
// {
//     // this->_clearObjects.push_back(id);
// }
// void GfxRendererBuilt::clearDestroyObjects()
// {
//     // for (auto &id : this->_clearObjects)
//     // {
//     //     if (this->_objects.find(id) != this->_objects.end())
//     //     {
//     //         GfxObject *object = this->_objects[id];
//     //         object->destroy();
//     //         delete object;
//     //         object = nullptr;
//     //         this->_objects.erase(id);
//     //     }
//     // }
//     // this->_clearObjects.clear();
// }

// void GfxRendererBuilt::submitObjectRender(std::string id)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt submitObjectRender:id not found" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = this->_objects[id];
//     // GfxRenderPass *pass = object->getPass();
//     // if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
//     // {
//     //     std::cerr << "GfxRendererBuilt submitObjectRender:pass not found" << std::endl;
//     //     return;
//     // }
//     // this->_queues[pass->name()]->submit(object);
// }

GfxRendererBuiltin::~GfxRendererBuiltin()
{
}

// void GfxRendererBuiltin::_initDefaultUIMaskPipeline()
// {
//     // // 模式ui 遮罩 模式为Fill 时 启用cullMode 为Back
//     // GfxPipelineStruct uiMaskAddPipelineStruct = {};
//     // uiMaskAddPipelineStruct.name = "built-ui-mask-add";
//     // uiMaskAddPipelineStruct.vert = "built-ui-mask.vert";
//     // uiMaskAddPipelineStruct.frag = "built-ui-mask.frag";
//     // uiMaskAddPipelineStruct.pass = "built-ui";
//     // uiMaskAddPipelineStruct.depthTest = 0;
//     // uiMaskAddPipelineStruct.depthWrite = 0;
//     // uiMaskAddPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
//     // // 模版测试 关闭
//     // uiMaskAddPipelineStruct.stencilTest = 1;
//     // // ui 遮罩,正面和背面保持一致
//     // uiMaskAddPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Always;
//     // uiMaskAddPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskAddPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskAddPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;
//     // uiMaskAddPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Always;
//     // uiMaskAddPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskAddPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskAddPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
//     // // 颜色混合 关闭（只写Stencil）
//     // uiMaskAddPipelineStruct.colorBlend = 0;
//     // // 多边形模式 填充
//     // uiMaskAddPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
//     // // 剔除模式 关闭（遮罩是2D平面，不需要剔除）
//     // uiMaskAddPipelineStruct.cullMode = GfxPipelineCullMode::None;
//     // this->createPipeline(uiMaskAddPipelineStruct.name, uiMaskAddPipelineStruct);

//     // // 模式ui 遮罩 模式为Fill 时 启用cullMode 为Back
//     // GfxPipelineStruct uiMaskSubPipelineStruct = {};
//     // uiMaskSubPipelineStruct.name = "built-ui-mask-sub";
//     // uiMaskSubPipelineStruct.vert = "built-ui-mask.vert";
//     // uiMaskSubPipelineStruct.frag = "built-ui-mask.frag";
//     // uiMaskSubPipelineStruct.pass = "built-ui";
//     // uiMaskSubPipelineStruct.depthTest = 0;
//     // uiMaskSubPipelineStruct.depthWrite = 0;
//     // uiMaskSubPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
//     // // 模版测试 关闭
//     // uiMaskSubPipelineStruct.stencilTest = 1;
//     // // ui 遮罩,正面和背面保持一致
//     // uiMaskSubPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Always;
//     // uiMaskSubPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskSubPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskSubPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     // uiMaskSubPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Always;
//     // uiMaskSubPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskSubPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
//     // uiMaskSubPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract;
//     // // 颜色混合 关闭（只写Stencil）
//     // uiMaskSubPipelineStruct.colorBlend = 0;
//     // // 多边形模式 填充
//     // uiMaskSubPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
//     // // 剔除模式 关闭（遮罩是2D平面，不需要剔除）
//     // uiMaskSubPipelineStruct.cullMode = GfxPipelineCullMode::None;
//     // this->createPipeline(uiMaskSubPipelineStruct.name, uiMaskSubPipelineStruct);
// }