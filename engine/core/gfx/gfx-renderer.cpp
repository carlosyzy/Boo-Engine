#include "gfx-renderer.h"
#include "gfx.h"
#include "gfx-context.h"
#include "pass/gfx-pass.h"
#include "pass/gfx-pass-built-screen.h"
#include "pass/gfx-pass-built-ui.h"
#include "pipeline/gfx-pipeline.h"
#include "pipeline/gfx-pipeline-ui.h"
#include "pipeline/gfx-pipeline-struct.h"
#include "queue/gfx-render-queue.h"

#include "gfx-descriptor.h"


#include "gfx-shader.h"
#include "gfx-shader-struct.h"
#include "gfx-shader-compile.h"
#include "gfx-texture.h"

#include "../math/mat4.h"

GfxRenderer::GfxRenderer()
{
}
void GfxRenderer::init()
{
    std::cout << "GfxRenderer:init" << std::endl;
    // GfxShaderCompile::getInstance()->init();
    // this->_descriptor = new GfxDescriptor();
    this->_initDefaultPasses();
    this->_initDefaultShaders();
    this->_initDefaultPipeline();
    // this->_initDefaultUIMaskPipeline();
}
/**
 * 创建内置默认的ui pass
 */
void GfxRenderer::_initDefaultPasses()
{
    GfxPassBuiltScreen *screenPass = new GfxPassBuiltScreen("pass-built-screen");
    this->_passes["pass-built-screen"] = screenPass;
    GfxPassBuiltUI *uiPass = new GfxPassBuiltUI("pass-built-ui");
    this->_passes["pass-built-ui"] = uiPass;
}
/**
 * 创建内置默认的ui shader
 */
void GfxRenderer::_initDefaultShaders()
{
    std::string shaderVertName = "built-ui.vert";
    GfxShader *shader = new GfxShader(shaderVertName);
    shader->createShaderModule(GfxShaderUIVertSPV, GfxShaderUIVertSPVSize);
    this->_shaders[shaderVertName] = shader;

    std::string shaderFragName = "built-ui.frag";
    shader = new GfxShader(shaderFragName);
    shader->createShaderModule(GfxShaderUIFragSPV, GfxShaderUIFragSPVSize);
    this->_shaders[shaderFragName] = shader;

    // std::string shaderMaskVertName = "built-ui-mask.vert";
    // shader = new GfxShader(shaderMaskVertName);
    // shader->createShaderModule(GfxShaderUIMaskVertSPV, GfxShaderUIMaskVertSPVSzie);
    // this->_shaders[shaderMaskVertName] = shader;

    // std::string shaderMaskFragName = "built-ui-mask.frag";
    // shader = new GfxShader(shaderMaskFragName);
    // shader->createShaderModule(GfxShaderUIMaskFragSPV, GfxShaderUIMaskFragSPVSzie);
    // this->_shaders[shaderMaskFragName] = shader;
}
/**
 * 创建内置默认的ui pipeline
 */
void GfxRenderer::_initDefaultPipeline()
{
    GfxPipelineStruct screenPipelineStruct = {};
    screenPipelineStruct.vert = "built-ui.vert";
    screenPipelineStruct.frag = "built-ui.frag";
    screenPipelineStruct.pass = "pass-built-screen";
    screenPipelineStruct.depthTest = 0;
    screenPipelineStruct.depthWrite = 0;
    screenPipelineStruct.depthCompareOp = GfxPipelineCompareOp::Always;
    // 颜色混合 开启
    screenPipelineStruct.colorBlend = 0;
    screenPipelineStruct.srcColorBlendFactor = GfxPipelineColorBlendFactor::SrcAlpha;
    screenPipelineStruct.dstColorBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    screenPipelineStruct.colorBlendOp = GfxPipelineColorBlendOp::Add;
    screenPipelineStruct.srcAlphaBlendFactor = GfxPipelineColorBlendFactor::One;
    screenPipelineStruct.dstAlphaBlendFactor = GfxPipelineColorBlendFactor::OneMinusSrcAlpha;
    screenPipelineStruct.alphaBlendOp = GfxPipelineColorBlendOp::Add;
    screenPipelineStruct.colorWriteMask = 4;
    // 多边形模式 填充
    screenPipelineStruct.polygonMode = GfxPipelinePolygonMode::Fill;
    // 剔除模式 背面
    screenPipelineStruct.cullMode = GfxPipelineCullMode::Back;
    this->createUIPipeline("pass-built-screen", screenPipelineStruct);

    GfxPipelineStruct uiPipelineStruct = {};
    uiPipelineStruct.vert = "built-ui.vert";
    uiPipelineStruct.frag = "built-ui.frag";
    uiPipelineStruct.pass = "pass-built-ui";
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
    this->createUIPipeline("pass-built-ui", uiPipelineStruct);
}

void GfxRenderer::createPipeline(std::string name, GfxPipelineStruct pipelineStruct)
{

    // if (this->_shaders.find(pipelineStruct.vert) == this->_shaders.end())
    // {
    //     std::cout << "createPipeline:vert not found:" << pipelineStruct.vert << std::endl;
    //     return;
    // }
    // if (this->_shaders.find(pipelineStruct.frag) == this->_shaders.end())
    // {
    //     std::cout << "createPipeline:frag not found:" << pipelineStruct.frag << std::endl;
    //     return;
    // }
    // if (this->_passes.find(pipelineStruct.pass) == this->_passes.end())
    // {
    //     std::cout << "createPipeline:pass not found:" << pipelineStruct.pass << std::endl;
    //     return;
    // }
    // // std::string pipelineName = GfxPipelineTypeNames.at(type) + "-" + pipelineStruct.pass;
    // // 通过type-pipelineStruct,哈希一个唯一的pipelineName-这个后续完善  在材质中就已经准备好了
    // // GfxPipeline *pipeline = new GfxPipeline(pipelineName);
    // GfxPipeline *pipeline = new GfxPipeline(name);
    // pipeline->create(this->_passes[pipelineStruct.pass], this->_shaders[pipelineStruct.vert], this->_shaders[pipelineStruct.frag], pipelineStruct);
    // this->_pipelines[name] = pipeline;
}
void GfxRenderer::createUIPipeline(std::string name, GfxPipelineStruct pipelineStruct)
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
    GfxPipelineUI *pipeline = new GfxPipelineUI(name);
    pipeline->create(this->_passes[pipelineStruct.pass], this->_shaders[pipelineStruct.vert], this->_shaders[pipelineStruct.frag], pipelineStruct);
    this->_pipelines[name] = pipeline;
}

void GfxRenderer::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    if (this->_textures.find(textureUuid) == this->_textures.end())
    {
        GfxTexture *texture = new GfxTexture(pixels, width, height, channels);
        this->_textures[textureUuid] = texture;
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
        GfxShader *shader = new GfxShader(finalCacheKey);
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
    GfxShader *shader = new GfxShader(shaderName);
    shader->createShaderModule(data);
    this->_shaders[shaderName] = shader;
}

void GfxRenderer::initRenderQueue(uint32_t renderId, uint32_t renderType, std::array<float, 16> &viewMat, std::array<float, 16> &projMat)
{
    if (this->_queues.find(renderId) == this->_queues.end())
    {
        GfxRenderQueue *queue = new GfxRenderQueue();
        this->_queues[renderId] = queue;
    }
    this->_queues[renderId]->init(viewMat, projMat);
}
void GfxRenderer::submitRenderObject(uint32_t renderId, GfxMaterial &material, GfxMesh &mesh)
{
    if (this->_queues.find(renderId) == this->_queues.end())
    {
        std::cout << "submitRenderObject:renderId not found:" << renderId << std::endl;
        return;
    }
    this->_queues[renderId]->submitObject(material, mesh);
}

void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    for (auto &queue : this->_queues)
    {
        // queue.second->render(imageIndex, commandBuffers);
    }
}

void GfxRenderer::cleanRendererState()
{
    // 渲染管线清除
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
}

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
//     // GfxPass *pass = object->getPass();
//     // if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
//     // {
//     //     std::cerr << "GfxRenderer submitObjectRender:pass not found" << std::endl;
//     //     return;
//     // }
//     // this->_queues[pass->name()]->submit(object);
// }

GfxRenderer::~GfxRenderer()
{
}

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