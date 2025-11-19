#include "gfx-renderer.h"
#include "gfx-context.h"
#include "gfx-texture.h"
#include "gfx-pass.h"
#include "gfx-pass-struct.h"
#include "gfx-pipeline.h"
#include "gfx-pipeline-struct.h"
#include "gfx-shader.h"
#include "gfx-queue.h"
#include "gfx-object.h"
#include "../math/mat4.h"
#include "gfx-shader-compile.h"

GfxRenderer::GfxRenderer(GfxContext *context)
{
    this->_context = context;
}
void GfxRenderer::init()
{
    std::cout << "GfxRenderer:init" << std::endl;
    GfxShaderCompile::getInstance()->init();
    this->_initDefaultUIPasses();
}
void GfxRenderer::_initDefaultUIPasses()
{
    std::cout << "GfxRenderer:_initDefaultUIPasses" << std::endl;
    // 创建一个默认的ui pass
    GfxPassStruct uiPassStruct = {};
    uiPassStruct.attachmentCount = 2;
    uiPassStruct.canMSAA = false;

    uiPassStruct.colorAttachment.enable = true;
    uiPassStruct.colorAttachment.attachment = 0;
    uiPassStruct.colorAttachment.format = 0; // 保持和swapchain 一致// this->_context->getSwapChainImageFormat();
    uiPassStruct.colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    uiPassStruct.colorAttachment.loadOp = GfxPassAttachmentLoadOp::Clear; // 后续有3d 后 会改为Load
    uiPassStruct.colorAttachment.storeOp = GfxPassAttachmentStoreOp::Store;
    uiPassStruct.colorAttachment.stencilLoadOp = GfxPassAttachmentLoadOp::DontCare;
    uiPassStruct.colorAttachment.stencilStoreOp = GfxPassAttachmentStoreOp::DontCare;
    uiPassStruct.colorAttachment.initialLayout = GfxPassAttachmentLayout::Undefined;
    uiPassStruct.colorAttachment.finalLayout = GfxPassAttachmentLayout::Present;
    uiPassStruct.colorAttachment.refLayout = GfxPassAttachmentLayout::Color;

    uiPassStruct.depthAttachment.enable = true;
    uiPassStruct.depthAttachment.attachment = 1;
    uiPassStruct.depthAttachment.format = 3; // VK_FORMAT_D32_UNORM_S8_UINT;
    uiPassStruct.depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    uiPassStruct.depthAttachment.loadOp = GfxPassAttachmentLoadOp::Clear;
    uiPassStruct.depthAttachment.storeOp = GfxPassAttachmentStoreOp::DontCare;
    uiPassStruct.depthAttachment.stencilLoadOp = GfxPassAttachmentLoadOp::Clear;
    uiPassStruct.depthAttachment.stencilStoreOp = GfxPassAttachmentStoreOp::Store;
    uiPassStruct.depthAttachment.initialLayout = GfxPassAttachmentLayout::Undefined;
    uiPassStruct.depthAttachment.finalLayout = GfxPassAttachmentLayout::Depth;
    uiPassStruct.depthAttachment.refLayout = GfxPassAttachmentLayout::Depth;

    this->createRenderPass("ui", uiPassStruct);
}

void GfxRenderer::createRenderPass(std::string name, GfxPassStruct passStruct)
{
    /*  // 创建渲染通道 */
    if (this->_passes.find(name) == this->_passes.end())
    {
        GfxPass *pass = new GfxPass(name, this->_context);
        pass->create(passStruct);
        this->_passes[name] = pass;
    }
    /*  // 创建对应的渲染队列 */
    if (this->_queues.find(name) == this->_queues.end())
    {
        GfxQueue *queue = new GfxQueue(name, this->_context);
        this->_queues[name] = queue;
        queue->create(this->_passes[name]);
    }
}
void GfxRenderer::createPipeline(std::string passName, std::string pipelineName)
{
    /*  // 创建管线 */
    if (this->_pipelines.find(pipelineName) != this->_pipelines.end())
    {
        this->_Log("createPipeline:name already exists");
        return;
    }
    /*  // 通过name解析出pipeline的关键信息
     // 哈希 有序 */
    std::unordered_map<std::string, std::string> properties;
    /* // 创建字符串流对象，将输入字符串包装成流 */
    std::stringstream ss(pipelineName);
    std::string token;
    while (std::getline(ss, token, '|'))
    {
        /*  // 4. 在token中查找冒号的位置 */
        size_t colon_pos = token.find(':');
        /*  // 5. 如果找到冒号 */
        if (colon_pos != std::string::npos)
        {
            /*  // 6. 提取冒号前的部分作为key */
            std::string key = token.substr(0, colon_pos);
            std::string value = token.substr(colon_pos + 1);
            properties[key] = value;
            std::cout << "createPipeline:key: " << key << " value: " << value << std::endl;
        }
    }
    std::cout << "createPipeline:name: " << pipelineName << std::endl;
    if (properties.find("vert") == properties.end())
    {
        this->_Log("createPipeline:vert or frag not found");
        return;
    }
    if (properties.find("frag") == properties.end())
    {
        this->_Log("createPipeline:frag not found");
        return;
    }
    std::string shaderVert = properties["vert"];
    std::string shaderFrag = properties["frag"];
    // std::cout << "createPipeline:vert: " << shaderVert << std::endl;
    // std::cout << "createPipeline:frag: " << shaderFrag << std::endl;
    /* // 先检测shader */
    if (this->_shaders.find(shaderVert) == this->_shaders.end())
    {
        std::cout << "createPipeline:vert not found:" << shaderVert << std::endl;
        return;
    }

    if (this->_shaders.find(shaderFrag) == this->_shaders.end())
    {
        std::cout << "createPipeline:frag not found:" << shaderFrag << std::endl;
        return;
    }
    /*  // 创建管线 */
    GfxPipeline *pipeline = new GfxPipeline(pipelineName, this->_context);
    pipeline->create(this->_passes[passName], this->_shaders[shaderVert], this->_shaders[shaderFrag], properties);
    this->_pipelines[pipelineName] = pipeline;
}

void GfxRenderer::createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct)
{

    if (this->_shaders.find(pipelineStruct.vert) == this->_shaders.end())
    {
        std::cout << "createPipeline:vert not found:" << pipelineStruct.vert << std::endl;
        return;
    }
    if (this->_shaders.find(pipelineStruct.frag) == this->_shaders.end())
    {
        std::cout << "createPipeline:frag not found:" << pipelineStruct.frag << std::endl;
        return;
    }
    if (this->_passes.find(pipelineStruct.pass) == this->_passes.end())
    {
        std::cout << "createPipeline:pass not found:" << pipelineStruct.pass << std::endl;
        return;
    }
    // // 更具GfxPipelineStruct结构调,定义出pipeline的唯一标识名字
    // std::string pipelineName = pipelineStruct.name;
    // pipelineName += "|Pass:" + pipelineStruct.pass + "|Vert:" + pipelineStruct.vert + "|Frag:" + pipelineStruct.frag;
    // std::map<std::string, int> properties;
    // properties["DepthTest"] = pipelineStruct.depthTest;
    // properties["DepthWrite"] = pipelineStruct.depthWrite;
    // properties["DepthCompareOp"] = static_cast<uint32_t>(pipelineStruct.depthCompareOp);
    // // 模版测试 关闭
    // properties["StencilTest"] = pipelineStruct.stencilTest;
    // if (pipelineStruct.stencilTest != 0)
    // {
    //     properties["StencilFrontCompareOp"] = static_cast<uint32_t>(pipelineStruct.stencilFrontCompareOp);
    //     properties["StencilFrontFailOp"] = static_cast<uint32_t>(pipelineStruct.stencilFrontFailOp);
    //     properties["StencilFrontDepthFailOp"] = static_cast<uint32_t>(pipelineStruct.stencilFrontDepthFailOp);
    //     properties["StencilFrontPassOp"] = static_cast<uint32_t>(pipelineStruct.stencilFrontPassOp);
    //     properties["StencilFrontCompareMask"] = static_cast<uint32_t>(pipelineStruct.stencilFrontCompareMask);
    //     properties["StencilFrontWriteMask"] = static_cast<uint32_t>(pipelineStruct.stencilFrontWriteMask);
    //     properties["StencilFrontReference"] = static_cast<uint32_t>(pipelineStruct.stencilFrontRreference);
    //     properties["StencilBackCompareOp"] = static_cast<uint32_t>(pipelineStruct.stencilBackCompareOp);
    //     properties["StencilBackFailOp"] = static_cast<uint32_t>(pipelineStruct.stencilBackFailOp);
    //     properties["StencilBackDepthFailOp"] = static_cast<uint32_t>(pipelineStruct.stencilBackDepthFailOp);
    //     properties["StencilBackPassOp"] = static_cast<uint32_t>(pipelineStruct.stencilBackPassOp);
    //     properties["StencilBackCompareMask"] = static_cast<uint32_t>(pipelineStruct.stencilBackCompareMask);
    //     properties["StencilBackWriteMask"] = static_cast<uint32_t>(pipelineStruct.stencilBackWriteMask);
    //     properties["StencilBackReference"] = static_cast<uint32_t>(pipelineStruct.stencilBackRreference);
    // }
    // // 颜色混合
    // properties["ColorBlend"] = pipelineStruct.colorBlend;
    // if (pipelineStruct.colorBlend != 0)
    // {
    //     properties["ColorBlendSrcFactor"] = static_cast<uint32_t>(pipelineStruct.srcColorBlendFactor);
    //     properties["ColorBlendDstFactor"] = static_cast<uint32_t>(pipelineStruct.dstColorBlendFactor);
    //     properties["ColorBlendOp"] = static_cast<uint32_t>(pipelineStruct.colorBlendOp);
    //     properties["ColorBlendSrcAlphaFactor"] = static_cast<uint32_t>(pipelineStruct.srcAlphaBlendFactor);
    //     properties["ColorBlendDstAlphaFactor"] = static_cast<uint32_t>(pipelineStruct.dstAlphaBlendFactor);
    //     properties["ColorBlendAlphaOp"] = static_cast<uint32_t>(pipelineStruct.alphaBlendOp);
    //     properties["ColorWriteMask"] = pipelineStruct.colorWriteMask;
    // }
    // // 多边形模式
    // properties["PolygonMode"] = static_cast<uint32_t>(pipelineStruct.polygonMode);
    // // 剔除模式
    // properties["CullMode"] = static_cast<uint32_t>(pipelineStruct.cullMode);

    // // 遍历map,将key和value添加到properties中
    // for (auto &[key, value] : properties)
    // {
    //     pipelineName += "|" + key + ":" + std::to_string(value);
    // }
    // std::cout << "createPipeline:pipelineName: " << pipelineName << std::endl;
    // ui|Vert:resources/shader/ui/ui.vert.spv|Fragresources/shader/ui/ui.frag.spv|ColorBlend:1|ColorBlendAlphaOp:0|ColorBlendDstAlphaFactor:6|ColorBlendDstFactor:6|ColorBlendOp:0|ColorBlendSrcAlphaFactor:0|ColorBlendSrcFactor:4|CullMode:2|DepthCompareOp:7|DepthTest:0|DepthWrite:0|PolygonMode:0|StencilTest:0|colorWriteMask:4
    GfxPipeline *pipeline = new GfxPipeline(pipelineName, this->_context);
    pipeline->create(this->_passes[pipelineStruct.pass], this->_shaders[pipelineStruct.vert], this->_shaders[pipelineStruct.frag], pipelineStruct);
    this->_pipelines[pipelineName] = pipeline;
}

void GfxRenderer::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    if (this->_textures.find(textureUuid) == this->_textures.end())
    {
        GfxTexture *texture = new GfxTexture(this->_context, pixels, width, height, channels);
        this->_textures[textureUuid] = texture;
        // std::cout << "createGfxTexture: " << textureUuid << std::endl;
    }
}

void GfxRenderer::destroyTexture(std::string textureUuid)
{
    if (this->_textures.find(textureUuid) != this->_textures.end())
    {
        delete this->_textures[textureUuid];
        this->_textures.erase(textureUuid);
    }
}
bool GfxRenderer::isExistTexture(std::string textureUuid)
{
    return this->_textures.find(textureUuid) != this->_textures.end();
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
    if (this->_shaders.find(finalCacheKey) != this->_shaders.end())
    {
        std::cout << "Shader already exists: " << finalCacheKey << std::endl;
        return;
    }
    // 创建着色器
    try
    {
        std::vector<uint32_t> spirvCode = GfxShaderCompile::getInstance()->compile(shaderType, finalCacheKey, data, macros);
        GfxShader *shader = new GfxShader(this->_context, finalCacheKey);
        shader->createShaderModule(spirvCode);
        this->_shaders[finalCacheKey] = shader;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to create shader '" << finalCacheKey << "': " << e.what() << std::endl;
        // 可以考虑抛出异常或返回错误码
    }
}

void GfxRenderer::createSpirvShader(const std::string &shaderName, const std::vector<char> &data)
{
    if (this->_shaders.find(shaderName) != this->_shaders.end())
    {
        std::cout << "Shader already exists: " << shaderName << std::endl;
        return;
    }
    GfxShader *shader = new GfxShader(this->_context, shaderName);
    shader->createShaderModule(data);
    this->_shaders[shaderName] = shader;
}

void GfxRenderer::cleanRendererState()
{
    /* // 渲染物体清除 */
    for (auto &[name, object] : this->_objects)
    {
        object->clear();
    }

    /*  // 渲染物体清除 */
    for (auto &[name, queue] : this->_queues)
    {
        queue->clear();
    }

    /*  // 渲染管线清除 */
    for (auto &[name, pipeline] : this->_pipelines)
    {
        pipeline->clear();
    }
    /*  // 渲染pass清除 */
    for (auto &[name, pass] : this->_passes)
    {
        pass->clear();
    }
}
void GfxRenderer::resetRendererState()
{
    for (auto &[name, pass] : this->_passes)
    {
        pass->reset();
    }
    for (auto &[name, pipeline] : this->_pipelines)
    {
        pipeline->reset();
    }
    for (auto &[name, queue] : this->_queues)
    {
        queue->reset();
    }
    for (auto &[name, object] : this->_objects)
    {
        object->reset();
    }
}

void GfxRenderer::createObject(std::string id, std::string passName, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_Log("createGfxObject:id already exists");
        return;
    }
    GfxObject *object = new GfxObject(this->_context);
    if(this->_pipelines.find("ui-mask.mtl") == this->_pipelines.end()){
        this->_Log("createGfxObject:ui-mask pipeline not found,Currently, creation is not supported");
        return;
    }
    object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
    object->setVertexs(points, colors, normals, uvs, indices);
    this->_objects[id] = object;
    this->_queues[passName]->submit(object);

    if (this->_passes.find(passName) != this->_passes.end())
    {

        object->setPass(this->_passes[passName]);
    }
    else
    {
        this->_Log("createGfxObject:renderPassType not found,Currently, creation is not supported");
    }
    /*  if (this->_pipelines.find(pipelineType) == this->_pipelines.end())
      {
          this->_Log("createGfxObject:pipelineType not found,Currently, creation is not supported");
          return;
      }
      else
      {
          object->setPipeline(this->_pipelines[pipelineType]);
      }*/
}
/* // void GfxRenderer::resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType)
// {
//     this->_Log("Currently not supported");
// } */
void GfxRenderer::destroyObject(std::string id)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        std::cout << "destroyGfxObject:id:aaaaaaaaaa" << id << std::endl;
        GfxObject *object = this->_objects[id];
        object->destroy();
        delete object;
        object = nullptr;
        this->_objects.erase(id);
    }
}
void GfxRenderer::setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->setModelMatrix(modelMatrix);
        return;
    }
}
void GfxRenderer::setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->setViewMatrix(viewMatrix);
        return;
    }
}
void GfxRenderer::setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->setProjMatrix(projMatrix);
        return;
    }
}
void GfxRenderer::setObjectTexture(const std::string &id, const std::string &texture)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        if (this->_textures.find(texture) == this->_textures.end())
        {
            this->_Log("setObjectTexture:texture not found");
            return;
        }
        this->_objects[id]->setTexture(this->_textures[texture]);
        return;
    }
}
void GfxRenderer::setObjectColor(std::string id, float r, float g, float b, float a)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->setColor(r, g, b, a);
        return;
    }
}
void GfxRenderer::setObjectPipeline(std::string id, std::string pipeline)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        this->_Log("setObjectPipeline:id not found");
        return;
    }
    if (this->_pipelines.find(pipeline) == this->_pipelines.end())
    {
        /* // this->_Log("setObjectPipeline:pipeline not found");
        // return; */
    }
    this->_objects[id]->setPipeline(this->_pipelines[pipeline]);
    // std::cout << "setObjectPipeline:id:" << id << " pipeline:" << pipeline << std::endl;
    return;
}
void GfxRenderer::addUIObjectMask(std::string id, std::string maskId, std::vector<float> mask)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->addUIMask(maskId, mask);
        return;
    }
}

void GfxRenderer::submitObjectRender(std::string id)
{
    // std::cout << "renderer submit   :" << id << std::endl;
    if (this->_objects.find(id) == this->_objects.end())
    {
        this->_Log("submit:id not found");
        return;
    }
    GfxObject *object = this->_objects[id];
    GfxPass *pass = object->pass();
    if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
    {
        this->_Log("submit:pass not found");
        return;
    }
    this->_queues[pass->name()]->submit(object);
}

void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    if (this->_queues.find("ui") != this->_queues.end())
    {
        this->_queues["ui"]->render(imageIndex, commandBuffers);
    }
}

void GfxRenderer::_Log(std::string msg)
{
    std::cout << "GfxRenderer:" << msg << std::endl;
}

GfxRenderer::~GfxRenderer()
{
}