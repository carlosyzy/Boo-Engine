#include "gfx-renderer.h"
#include "gfx.h"
#include "gfx-context.h"
#include "base/gfx-shader.h"
#include "base/gfx-shader-struct.h"
#include "base/gfx-texture.h"
#include "base/gfx-render-pass.h"
#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-buffer-ubo.h"
#include "base/gfx-buffer-instance.h"
#include "base/gfx-render-texture.h"
#include "default/gfx-renderer-default.h"
#include "builtin/gfx-renderer-builtin.h"

#include "../math/mat4.h"

GfxRenderer::GfxRenderer()
{
    this->_defaultRenderer = new GfxRendererDefault("default");
    this->_builtinRenderer = new GfxRendererBuiltin("builtin");
}
void GfxRenderer::init()
{
    Gfx::bufferUBO = new GfxBufferUBO();
    Gfx::bufferInstance = new GfxBufferInstance();
}
void GfxRenderer::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    if (Gfx::textures.find(textureUuid) == Gfx::textures.end())
    {
        std::cout << "Gfx : Renderer :: createTexture:uuid:" << textureUuid << " width:" << width << " height:" << height << " channels:" << channels << std::endl;
        GfxTexture *texture = new GfxTexture(textureUuid, pixels, width, height, channels);
        Gfx::textures[textureUuid] = texture;
    }
}
void GfxRenderer::insertTexture(std::string textureUuid, GfxTexture *texture)
{
    if (Gfx::textures.find(textureUuid) != Gfx::textures.end())
    {
        std::cout << "Gfx : Renderer :: insertTexture:uuid:" << textureUuid << " already exists" << std::endl;
        return;
    }
    Gfx::textures[textureUuid] = texture;
}
void GfxRenderer::destroyTexture(std::string textureUuid)
{
    if (Gfx::textures.find(textureUuid) != Gfx::textures.end())
    {
        delete Gfx::textures[textureUuid];
        Gfx::textures.erase(textureUuid);
    }
}
bool GfxRenderer::isExistTexture(std::string textureUuid)
{
    return Gfx::textures.find(textureUuid) != Gfx::textures.end();
}
GfxTexture *GfxRenderer::getTexture(std::string uuid)
{
    if (Gfx::textures.find(uuid) == Gfx::textures.end())
    {
        std::cout << "Gfx : Renderer :: Texture not found:" << uuid << std::endl;
        return nullptr;
    }
    return Gfx::textures.at(uuid);
}

void GfxRenderer::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros)
{
    // 生成唯一的缓存键：shaderName + 宏定义
    std::stringstream cacheKey;
    cacheKey << shaderName;
    if (!macros.empty())
    {
        cacheKey << "[";
        bool first = true;
        for (const auto &[key, value] : macros)
        {
            if (!first)
            {
                cacheKey << "|";
            }
            cacheKey << key << ":" << value;
            first = false;
        }
        cacheKey << "]";
    }

    std::string finalCacheKey = cacheKey.str();
    //  检查是否已存在
    if (Gfx::shaders.find(finalCacheKey) != Gfx::shaders.end())
    {
        std::cout << "Gfx : Renderer :: Shader already exists: " << finalCacheKey << std::endl;
        return;
    }
    // 创建着色器
    try
    {
        std::vector<uint32_t> spirvCode = this->compileShaderGlslToSpirv(shaderType, finalCacheKey, data, macros);
        GfxShader *shader = new GfxShader(finalCacheKey);
        shader->createShaderModule(spirvCode);
        Gfx::shaders[finalCacheKey] = shader;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create shader '" << finalCacheKey << "': " << e.what() << std::endl;
        // 可以考虑抛出异常或返回错误码
    }
}

void GfxRenderer::createSpirvShader(const std::string &shaderName, const std::vector<char> &data)
{
    if (Gfx::shaders.find(shaderName) != Gfx::shaders.end())
    {
        std::cout << "Gfx : Renderer :: Shader already exists: " << shaderName << std::endl;
        return;
    }
    GfxShader *shader = new GfxShader(shaderName);
    shader->createShaderModule(data);
    Gfx::shaders[shaderName] = shader;
}
std::vector<uint32_t> GfxRenderer::compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, std::string> &macros)
{
    shaderc::Compiler _compiler;
    // 配置编译选项
    shaderc::CompileOptions compileOptions;
    // 设置目标环境
    compileOptions.SetTargetEnvironment(
        shaderc_target_env_vulkan,
        shaderc_env_version_vulkan_1_0);
    // 优化级别
    compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
    // 生成调试信息
    compileOptions.SetGenerateDebugInfo();
    // 添加宏定义
    // 添加通用宏定义
    compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    compileOptions.AddMacroDefinition("VULKAN", "100");
    for (const auto &[key, value] : macros)
    {
        compileOptions.AddMacroDefinition(key, value);
    }
    // compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    // compileOptions.AddMacroDefinition("VULKAN", "100");

    shaderc::SpvCompilationResult result;
    if (type == "Vert")
    {
        result = _compiler.CompileGlslToSpv(
            source.c_str(), shaderc_vertex_shader, cacheKey.c_str(), compileOptions);
    }
    else if (type == "Frag")
    {
        result = _compiler.CompileGlslToSpv(
            source.c_str(), shaderc_fragment_shader, cacheKey.c_str(), compileOptions);
    }
    else
    {
        throw std::runtime_error("Shader type not supported");
    }
    shaderc_compilation_status status = result.GetCompilationStatus();
    if (status != shaderc_compilation_status_success)
    {
        std::string errorMsg = "Shader compilation failed:\n";
        errorMsg += "File: " + cacheKey + "\n";
        errorMsg += "Error: " + result.GetErrorMessage();
        errorMsg += "Status: " + std::to_string(status);

        throw std::runtime_error(errorMsg);
    }
    // 输出警告信息
    if (result.GetNumWarnings() > 0)
    {
        std::cout << "Gfx : Renderer :: Shader compilation warnings for " << cacheKey << ":\n"
                  << result.GetErrorMessage() << std::endl;
    }
    std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    std::cout << "Gfx : Renderer :: Successfully compiled " << cacheKey
              << " (" << spirvCode.size() << " SPIR-V words)" << std::endl;
    return spirvCode;
}
void GfxRenderer::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture)
{
    this->_builtinRenderer->initRenderQueue(renderId, renderTexture);
}
void GfxRenderer::delRenderQueue(std::string renderId)
{
    this->_builtinRenderer->delRenderQueue(renderId);
}
void GfxRenderer::submitRenderMat(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix)
{
    this->_builtinRenderer->submitRenderMat(renderId, viewMatrix, projMatrix);
}
void GfxRenderer::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData)
{
    this->_builtinRenderer->submitRenderObject(renderId, material, mesh, instanceData);
}
void GfxRenderer::frameRendererBefore()
{
    Gfx::bufferUBO->clear();
    Gfx::bufferInstance->clear();
    this->_defaultRenderer->frameRendererBefore();
    this->_builtinRenderer->frameRendererBefore();
}
void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    //先获取上一帧的离屏渲染输出提交到默认队列
    std::vector<std::string> pipelineOutds;
    this->_builtinRenderer->getOffScreenOutds(pipelineOutds);
    this->_defaultRenderer->frameRenderer(imageIndex, commandBuffers, pipelineOutds);
    // this->_pipelineOutds.clear();
    // // 渲染3d队列
    // // 渲染ui队列
    // this->_renderPipelineUI->frameRenderer(imageIndex, commandBuffers, this->_pipelineOutds);

    // // 渲染默认队列屏幕输出
    // for (auto &out : this->_pipelineOutds)
    // {
    //     // std::cout << "Gfx : Renderer :: Rendering builtin pipeline output: " << out << std::endl;
    //     this->_renderPipelineBuiltin->submitRenderObject(out);
    // }
    // // this->_renderPipelineBuiltin->submitRenderObject("default-texture");
    // this->_renderPipelineBuiltin->frameRenderer(imageIndex, commandBuffers);
}
void GfxRenderer::frameRendererAfter()
{
    this->_defaultRenderer->frameRendererAfter();
    this->_builtinRenderer->frameRendererAfter();
}
GfxRenderer::~GfxRenderer()
{
}

// void GfxRenderer::submitRenderObject(uint32_t renderId, GfxMaterial &material, GfxMesh &mesh)
// {
// }

// void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
// {
//     // // 渲染默认队列
//     // // std::array<float, 16> viewMat = {1.0f};
//     // // std::array<float, 16> projMat = {1.0f};
//     // // this->_defaultQueue->init(viewMat, projMat);
//     // // this->_defaultQueue->submitObject(defaultMaterial, defaultMesh);
//     // this->_defaultQueue->render(imageIndex, commandBuffers);

//     // // for (auto &queue : this->_queues)
//     // // {
//     // //     // queue.second->render(imageIndex, commandBuffers);
//     // // }
// }

void GfxRenderer::_cleanRendererState()
{
    this->_defaultRenderer->_cleanRendererState();
}
void GfxRenderer::_resetRendererState()
{
    this->_defaultRenderer->_resetRendererState();
}
// void GfxRenderer::_initDescriptor()
// {
//     this->_initDescriptorPool();
//     this->_initDescriptorSetLayout();
//     this->_initDescriptorSets();
// }
// void GfxRenderer::_initDescriptorPool()
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
// void GfxRenderer::_initDescriptorSetLayout()
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
// void GfxRenderer::_initDescriptorSets()
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
// void GfxRenderer::createObject(std::string id, std::string passName, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
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
// void GfxRenderer::createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer: createUIObject:id already exists" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = new GfxObject(id, GfxObjectType::UI, this->_context);
//     // object->setVertexs(points, colors, normals, uvs, indices);
//     // this->_objects[id] = object;
//     // // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
//     // // this->_queues["ui-pass"]->submit(object);
// }
// void GfxRenderer::createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer: createUIMaskObject:id already exists" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = new GfxObject(id, GfxObjectType::UI_MASK, this->_context);
//     // object->setVertexs(points, colors, normals, uvs, indices);
//     // this->_objects[id] = object;
//     // // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
//     // // this->_queues["ui-pass"]->submit(object);
// }
// void GfxRenderer::setObjectPass(std::string id, std::string pass)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer: setObjectPass:object id not found" << std::endl;
//     //     return;
//     // }
//     // if (this->_passes.find(pass) == this->_passes.end())
//     // {
//     //     std::cerr << "GfxRenderer: setObjectPass:pass not found" << std::endl;
//     //     return;
//     // }
//     // this->_objects[id]->setPass(this->_passes[pass]);
// }

// void GfxRenderer::setObjectPipeline(std::string id, std::string pipeline)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer: setObjectPipeline:object id not found" << std::endl;
//     //     return;
//     // }
//     // if (this->_pipelines.find(pipeline) == this->_pipelines.end())
//     // {
//     //     std::cerr << "GfxRenderer: setObjectPipeline:pipeline not found" << std::endl;
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
// void GfxRenderer::setObjectUIMaskBehavior(std::string id, uint32_t behavior)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer: setObjectUIMaskBehavior:object id not found" << std::endl;
//     //     return;
//     // }
//     // this->_objects[id]->setUIMaskBehavior(behavior);
// }

// void GfxRenderer::setObjectTexture(const std::string &id, const std::string &texture)
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
// void GfxRenderer::setObjectColor(std::string id, float r, float g, float b, float a)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setColor(r, g, b, a);
//     //     return;
//     // }
// }
// void GfxRenderer::setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setModelMatrix(modelMatrix);
//     //     return;
//     // }
// }
// void GfxRenderer::setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setViewMatrix(viewMatrix);
//     //     return;
//     // }
// }
// void GfxRenderer::setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix)
// {
//     // if (this->_objects.find(id) != this->_objects.end())
//     // {
//     //     this->_objects[id]->setProjMatrix(projMatrix);
//     //     return;
//     // }
// }
// void GfxRenderer::destroyObject(std::string id)
// {
//     // this->_clearObjects.push_back(id);
// }
// void GfxRenderer::clearDestroyObjects()
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

// void GfxRenderer::submitObjectRender(std::string id)
// {
//     // if (this->_objects.find(id) == this->_objects.end())
//     // {
//     //     std::cerr << "GfxRenderer submitObjectRender:id not found" << std::endl;
//     //     return;
//     // }
//     // GfxObject *object = this->_objects[id];
//     // GfxRenderPass *pass = object->getPass();
//     // if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
//     // {
//     //     std::cerr << "GfxRenderer submitObjectRender:pass not found" << std::endl;
//     //     return;
//     // }
//     // this->_queues[pass->name()]->submit(object);
// }

// void GfxRenderer::_initDefaultUIMaskPipeline()
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