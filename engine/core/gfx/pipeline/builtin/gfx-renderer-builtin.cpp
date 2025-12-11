#include "gfx-renderer-builtin.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-shader.h"
#include "../../gfx-shader-struct.h"
#include "../../gfx-texture.h"
#include "../../../math/mat4.h"
#include "gfx-pass-builtin.h"
#include "gfx-pipeline-builtin.h"
#include "gfx-queue-builtin.h"

GfxRendererBuiltin::GfxRendererBuiltin(std::string name)
{
    this->_samplerCount = 1;
    this->_maxObjectCount = 10;
}
void GfxRendererBuiltin::init()
{
    std::cout << "Gfx : Renderer  Built :: init" << std::endl;
    // this->_textTexture = new TextureAsset("default-texture");
    // // this->_textTexture->create("F:/worksapces/Boo-Engine/x64/Debug/res/private/ic-2d.png");
    // this->_textTexture->create("/Users/yangzongyuan/personal/project/Boo-Engine/build/res/private/ic-2d.png");

    this->_initDefaultDescriptor();
    this->_initDefaultRenderPasse();
    this->_initDefaultShader();
    this->_initDefaultPipeline();
    this->_initDefaultRenderQueue();
}
void GfxRendererBuiltin::_initDefaultDescriptor()
{
    std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
    uint32_t swapChainImageCount = swapChainImageViews.size();
    std::vector<VkDescriptorPoolSize> poolSizes(1);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // 总采样器数量 = 每个集的采样器数 × 集的数量
    poolSizes[0].descriptorCount = swapChainImageCount * (this->_maxObjectCount + 3) * this->_samplerCount; // 单个描述符集3
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = swapChainImageCount * (this->_maxObjectCount + 3); // 描述符集的最大数量
    if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor pool failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor pool success " << std::endl;

    std::vector<VkDescriptorSetLayoutBinding> bindings;
    // 采样器绑定
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = this->_samplerCount;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings.push_back(samplerLayoutBinding);

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(Gfx::context->getVkDevice(),
                                    &layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor set layout failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor set layout success " << std::endl;

    for (uint32_t i = 0; i < this->_maxObjectCount; i++)
    {
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
        uint32_t swapChainImageCount = swapChainImageViews.size();
        std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = this->_descriptorPool;
        allocInfo.descriptorSetCount = swapChainImageCount;
        allocInfo.pSetLayouts = layouts.data();
        descriptorSets.resize(swapChainImageCount);
        if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        {
            std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
            return;
        }
        GfxRenderxDescriptorSets renderxDescriptorSets = {descriptorSets, false};
        this->_descriptorSets.push_back(renderxDescriptorSets);
    }
}

/**
 * @brief 创建内置默认的ui pass
 */
void GfxRendererBuiltin::_initDefaultRenderPasse()
{
    this->_pass = new GfxPassBuiltin("built");
}
GfxPassBuiltin *GfxRendererBuiltin::getPass()
{
    return this->_pass;
}
/**
 * 创建内置默认的ui shader
 */
void GfxRendererBuiltin::_initDefaultShader()
{
    std::string shaderVertName = "built.vert";
    GfxShader *shader = new GfxShader(shaderVertName);
    shader->createShaderModule(GfxShaderBuiltVertSPV, GfxShaderBuiltVertSPVSize);
    Gfx::shaders[shaderVertName] = shader;

    std::string shaderFragName = "built.frag";
    shader = new GfxShader(shaderFragName);
    shader->createShaderModule(GfxShaderBuiltFragSPV, GfxShaderBuiltFragSPVSize);
    Gfx::shaders[shaderFragName] = shader;
}
/**
 * 创建内置默认的ui pipeline
 */
void GfxRendererBuiltin::_initDefaultPipeline()
{
    GfxPipelineStruct pipelineStruct = {};
    pipelineStruct.pass = "built";
    pipelineStruct.vert = "built.vert";
    pipelineStruct.frag = "built.frag";
    // 多边形模式 填充
    pipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    pipelineStruct.cullMode = GfxPipelineCullMode::Back;
    pipelineStruct.depthTest = 0;
    pipelineStruct.depthWrite = 0;
    pipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
    pipelineStruct.stencilTest = 0;
    // 颜色混合 开启
    pipelineStruct.colorBlend = 1;
    pipelineStruct.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
    pipelineStruct.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    pipelineStruct.colorBlendOp = GfxPipelineColorBlendOp::Add;
    pipelineStruct.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    pipelineStruct.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    pipelineStruct.alphaBlendOp = GfxPipelineColorBlendOp::Add;
    pipelineStruct.colorWriteMask = 4;
    // 推送常量 开启
    pipelineStruct.pushConstant = 0;
    pipelineStruct.pushConstantSize = 0;

    this->createPipeline(pipelineStruct.generateKey(), pipelineStruct);
}
void GfxRendererBuiltin::_initDefaultRenderQueue()
{
    this->_queue = new GfxQueueBuiltin(this);
    this->_queue->init();
}

void GfxRendererBuiltin::createPipeline(std::string name, GfxPipelineStruct pipelineStruct)
{
    if (Gfx::shaders.find(pipelineStruct.vert) == Gfx::shaders.end())
    {
        std::cout << "createPipeline:vert not found:" << pipelineStruct.vert << std::endl;
        return;
    }
    if (Gfx::shaders.find(pipelineStruct.frag) == Gfx::shaders.end())
    {
        std::cout << "createPipeline:frag not found:" << pipelineStruct.frag << std::endl;
        return;
    }
    if (this->_pass == nullptr)
    {
        std::cout << "createPipeline:pass not found:" << pipelineStruct.pass << std::endl;
        return;
    }
    this->_pipeline = new GfxPipelineBuiltin(name);
    this->_pipeline->create(this->_pass, Gfx::shaders[pipelineStruct.vert], Gfx::shaders[pipelineStruct.frag], this->_descriptorSetLayout, pipelineStruct);
}

GfxPipelineBuiltin *GfxRendererBuiltin::getPipeline()
{
    if (this->_pipeline == nullptr)
    {
        std::cout << "getPipeline:not found:" << std::endl;
        return nullptr;
    }
    return this->_pipeline;
}
std::vector<VkDescriptorSet> GfxRendererBuiltin::getDescriptorSets()
{
    for (auto &renderxDescriptorSets : this->_descriptorSets)
    {
        if (!renderxDescriptorSets.isUsed)
        {
            renderxDescriptorSets.isUsed = true;
            return renderxDescriptorSets.descriptorSets;
        }
    }
    return {VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE};
}

void GfxRendererBuiltin::submitRenderObject(const std::string textureUuid)
{
    if (this->_currentObjectCount >= this->_maxObjectCount)
    {
        std::cout << "submitRenderObject:max object count reached:" << this->_maxObjectCount << std::endl;
        return;
    }
    this->_currentObjectCount++;
    this->_queue->submitObject(textureUuid);
}

void GfxRendererBuiltin::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    this->_queue->render(imageIndex, commandBuffers);
    // 渲染完成后，重置描述符集状态
    for (auto &renderxDescriptorSets : this->_descriptorSets)
    {
        renderxDescriptorSets.isUsed = false;
    }
    this->_currentObjectCount = 0;
}

void GfxRendererBuiltin::cleanRendererState()
{
    // this->_defaultQueue->_clear();
    // // 渲染队列清除
    // for (auto &[name, queue] : this->_queues)
    // {
    //     queue->_clear();
    // }
    // // 渲染管线清除
    // for (auto &[name, pipeline] : this->_pipelines)
    // {
    //     pipeline->_clear();
    // }
    // /*  // 渲染pass清除 */
    // for (auto &[name, pass] : this->_passes)
    // {
    //     pass->_clear();
    // }
}
void GfxRendererBuiltin::resetRendererState()
{
    // for (auto &[name, pass] : this->_passes)
    // {
    //     pass->_reset();
    // }
    // for (auto &[name, pipeline] : this->_pipelines)
    // {
    //     pipeline->_reset();
    // }
    // this->_defaultQueue->_reset();
    // // 渲染队列重置
    // for (auto &[name, queue] : this->_queues)
    // {
    //     queue->_reset();
    // }
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
//     // GfxPass *pass = object->getPass();
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