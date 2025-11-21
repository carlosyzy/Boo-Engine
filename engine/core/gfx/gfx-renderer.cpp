#include "gfx-renderer.h"
#include "gfx-context.h"
#include "gfx-texture.h"
#include "gfx-pass.h"
#include "gfx-pass-struct.h"
#include "gfx-pipeline.h"
#include "gfx-pipeline-struct.h"
#include "gfx-shader.h"
#include "gfx-shader-struct.h"
#include "gfx-queue.h"
#include "gfx-object.h"
#include "gfx-object-struct.h"
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
    this->_initDefaultUIShaders();
    this->_initDefaultUIPipeline();
    this->_initDefaultUIMaskPipeline();
}
/**
 * 创建内置默认的ui pass
 */
void GfxRenderer::_initDefaultUIPasses()
{
    // 创建一个默认的ui pass
    GfxPassStruct uiPassStruct = {};
    uiPassStruct.name = "built-ui";
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

    this->createRenderPass(uiPassStruct.name, uiPassStruct);
}
/**
 * 创建内置默认的ui shader
 */
void GfxRenderer::_initDefaultUIShaders()
{
    std::string shaderVertName = "built-ui.vert";
    GfxShader *shader = new GfxShader(this->_context, shaderVertName);
    shader->createShaderModule(GfxShaderUIVertSPV, GfxShaderUIVertSPVSize);
    this->_shaders[shaderVertName] = shader;

    std::string shaderFragName = "built-ui.frag";
    shader = new GfxShader(this->_context, shaderFragName);
    shader->createShaderModule(GfxShaderUIFragSPV, GfxShaderUIFragSPVSize);
    this->_shaders[shaderFragName] = shader;

    std::string shaderMaskVertName = "built-ui-mask.vert";
    shader = new GfxShader(this->_context, shaderMaskVertName);
    shader->createShaderModule(GfxShaderUIMaskVertSPV, GfxShaderUIMaskVertSPVSzie);
    this->_shaders[shaderMaskVertName] = shader;

    std::string shaderMaskFragName = "built-ui-mask.frag";
    shader = new GfxShader(this->_context, shaderMaskFragName);
    shader->createShaderModule(GfxShaderUIMaskFragSPV, GfxShaderUIMaskFragSPVSzie);
    this->_shaders[shaderMaskFragName] = shader;
}
/**
 * 创建内置默认的ui pipeline
 */
void GfxRenderer::_initDefaultUIPipeline()
{
    // 创建Pass是在摄像机初始化的时候
    // 创建Pipeline应该是渲染物体第一次调用的时候
    GfxPipelineStruct uiPipelineStruct = {};
    uiPipelineStruct.name = "built-ui";
    uiPipelineStruct.vert = "built-ui.vert";
    uiPipelineStruct.frag = "built-ui.frag";
    uiPipelineStruct.pass = "built-ui";
    uiPipelineStruct.depthTest = 0;
    uiPipelineStruct.depthWrite = 0;
    uiPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 启用（用于UI遮罩）
    uiPipelineStruct.stencilTest = 1;
    uiPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Equal;  // 只在模板值相等时绘制
    uiPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;      // 测试失败：保持
    uiPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep; // 深度失败：保持
    uiPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Keep;      // 测试通过：保持（不修改模板值）
    uiPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Equal;
    uiPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Keep;
    // 颜色混合 开启
    uiPipelineStruct.colorBlend = 1;
    uiPipelineStruct.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
    uiPipelineStruct.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipelineStruct.colorBlendOp = GfxPipelineColorBlendOp::Add;
    uiPipelineStruct.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    uiPipelineStruct.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    uiPipelineStruct.alphaBlendOp = GfxPipelineColorBlendOp::Add;
    uiPipelineStruct.colorWriteMask = 4;
    // 多边形模式 填充
    uiPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    uiPipelineStruct.cullMode = GfxPipelineCullMode::Back;
    this->createPipeline(uiPipelineStruct.name, uiPipelineStruct);
}
void GfxRenderer::_initDefaultUIMaskPipeline()
{
    // 模式ui 遮罩 模式为Fill 时 启用cullMode 为Back
    GfxPipelineStruct uiMaskAddPipelineStruct = {};
    uiMaskAddPipelineStruct.name = "built-ui-mask-add";
    uiMaskAddPipelineStruct.vert = "built-ui-mask.vert";
    uiMaskAddPipelineStruct.frag = "built-ui-mask.frag";
    uiMaskAddPipelineStruct.pass = "built-ui";
    uiMaskAddPipelineStruct.depthTest = 0;
    uiMaskAddPipelineStruct.depthWrite = 0;
    uiMaskAddPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 关闭
    uiMaskAddPipelineStruct.stencilTest = 1;
    // ui 遮罩,正面和背面保持一致
    uiMaskAddPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Always;
    uiMaskAddPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Increment_Add;
    uiMaskAddPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Always;
    uiMaskAddPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskAddPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Increment_Add;
    // 颜色混合 关闭（只写Stencil）
    uiMaskAddPipelineStruct.colorBlend = 0;
    // 多边形模式 填充
    uiMaskAddPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 关闭（遮罩是2D平面，不需要剔除）
    uiMaskAddPipelineStruct.cullMode = GfxPipelineCullMode::None;
    this->createPipeline(uiMaskAddPipelineStruct.name, uiMaskAddPipelineStruct);

    // 模式ui 遮罩 模式为Fill 时 启用cullMode 为Back
    GfxPipelineStruct uiMaskSubPipelineStruct = {};
    uiMaskSubPipelineStruct.name = "built-ui-mask-sub";
    uiMaskSubPipelineStruct.vert = "built-ui-mask.vert";
    uiMaskSubPipelineStruct.frag = "built-ui-mask.frag";
    uiMaskSubPipelineStruct.pass = "built-ui";
    uiMaskSubPipelineStruct.depthTest = 0;
    uiMaskSubPipelineStruct.depthWrite = 0;
    uiMaskSubPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
    // 模版测试 关闭
    uiMaskSubPipelineStruct.stencilTest = 1;
    // ui 遮罩,正面和背面保持一致
    uiMaskSubPipelineStruct.stencilFrontCompareOp = GfxPipelineCompareOp::Always;
    uiMaskSubPipelineStruct.stencilFrontFailOp = GfxPipelineStencilOp::Keep;
    uiMaskSubPipelineStruct.stencilFrontDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskSubPipelineStruct.stencilFrontPassOp = GfxPipelineStencilOp::Decrement_Subtract;
    uiMaskSubPipelineStruct.stencilBackCompareOp = GfxPipelineCompareOp::Always;
    uiMaskSubPipelineStruct.stencilBackFailOp = GfxPipelineStencilOp::Keep;
    uiMaskSubPipelineStruct.stencilBackDepthFailOp = GfxPipelineStencilOp::Keep;
    uiMaskSubPipelineStruct.stencilBackPassOp = GfxPipelineStencilOp::Decrement_Subtract;
    // 颜色混合 关闭（只写Stencil）
    uiMaskSubPipelineStruct.colorBlend = 0;
    // 多边形模式 填充
    uiMaskSubPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 关闭（遮罩是2D平面，不需要剔除）
    uiMaskSubPipelineStruct.cullMode = GfxPipelineCullMode::None;
    this->createPipeline(uiMaskSubPipelineStruct.name, uiMaskSubPipelineStruct);
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
    // if (this->_objects.find(id) != this->_objects.end())
    // {
    //     this->_Log("createGfxObject:id already exists");
    //     return;
    // }
    // GfxObject *object = new GfxObject(this->_context);
    // if(this->_pipelines.find("ui-mask.mtl") == this->_pipelines.end()){
    //     this->_Log("createGfxObject:ui-mask pipeline not found,Currently, creation is not supported");
    //     return;
    // }
    // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
    // object->setVertexs(points, colors, normals, uvs, indices);
    // this->_objects[id] = object;
    // this->_queues[passName]->submit(object);

    // if (this->_passes.find(passName) != this->_passes.end())
    // {

    //     object->setPass(this->_passes[passName]);
    // }
    // else
    // {
    //     this->_Log("createGfxObject:renderPassType not found,Currently, creation is not supported");
    // }
    // /*  if (this->_pipelines.find(pipelineType) == this->_pipelines.end())
    //   {
    //       this->_Log("createGfxObject:pipelineType not found,Currently, creation is not supported");
    //       return;
    //   }
    //   else
    //   {
    //       object->setPipeline(this->_pipelines[pipelineType]);
    //   }*/
}
void GfxRenderer::createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        std::cerr << "GfxRenderer: createUIObject:id already exists" << std::endl;
        return;
    }
    GfxObject *object = new GfxObject(GfxObjectType::UI, this->_context);
    object->setVertexs(points, colors, normals, uvs, indices);
    this->_objects[id] = object;
    // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
    // this->_queues["ui-pass"]->submit(object);
}
void GfxRenderer::createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        std::cerr << "GfxRenderer: createUIMaskObject:id already exists" << std::endl;
        return;
    }
    GfxObject *object = new GfxObject(GfxObjectType::UI_MASK, this->_context);
    object->setVertexs(points, colors, normals, uvs, indices);
    this->_objects[id] = object;
    // object->setUIMaskPipeline(this->_pipelines["ui-mask.mtl"]);
    // this->_queues["ui-pass"]->submit(object);
}
void GfxRenderer::setObjectPass(std::string id, std::string pass)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        std::cerr << "GfxRenderer: setObjectPass:object id not found" << std::endl;
        return;
    }
    if (this->_passes.find(pass) == this->_passes.end())
    {
        std::cerr << "GfxRenderer: setObjectPass:pass not found" << std::endl;
        return;
    }
    this->_objects[id]->setPass(this->_passes[pass]);
}

void GfxRenderer::setObjectPipeline(std::string id, std::string pipeline)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        std::cerr << "GfxRenderer: setObjectPipeline:object id not found" << std::endl;
        return;
    }
    if (this->_pipelines.find(pipeline) == this->_pipelines.end())
    {
        std::cerr << "GfxRenderer: setObjectPipeline:pipeline not found" << std::endl;
        return;
    }
    this->_objects[id]->setPipeline(this->_pipelines[pipeline]);
}
/**
 * @brief 设置UI遮罩行为
 *
 * @param id 物体ID
 * @param behavior 行为 0 不遮罩 1 遮罩
 */
void GfxRenderer::setObjectUIMaskBehavior(std::string id, uint32_t behavior)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        std::cerr << "GfxRenderer: setObjectUIMaskBehavior:object id not found" << std::endl;
        return;
    }
    this->_objects[id]->setUIMaskBehavior(behavior);
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

void GfxRenderer::submitObjectRender(std::string id)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        std::cerr << "GfxRenderer submitObjectRender:id not found" << std::endl;
        return;
    }
    GfxObject *object = this->_objects[id];
    GfxPass *pass = object->getPass();
    if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
    {
        std::cerr << "GfxRenderer submitObjectRender:pass not found" << std::endl;
        return;
    }
    this->_queues[pass->name()]->submit(object);
}

void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    for (auto &queue : this->_queues)
    {
        queue.second->render(imageIndex, commandBuffers);
    }
}

void GfxRenderer::_Log(std::string msg)
{
    std::cout << "GfxRenderer:" << msg << std::endl;
}

GfxRenderer::~GfxRenderer()
{
}