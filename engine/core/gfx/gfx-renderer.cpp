#include "gfx-renderer.h"
#include "gfx-context.h"
#include "gfx-texture.h"
#include "gfx-pass.h"
#include "gfx-pipeline.h"
#include "gfx-shader.h"
#include "gfx-queue.h"
#include "gfx-object.h"
#include "../math/mat4.h"

GfxRenderer::GfxRenderer(GfxContext *context)
{
    this->_context = context;
}
void GfxRenderer::init()
{
    std::cout << "GfxRenderer:init" << std::endl;
}

void GfxRenderer::createRenderPass(std::string name)
{
   /*  // 创建渲染通道 */
    if (this->_passes.find(name) == this->_passes.end())
    {
        GfxPass *pass = new GfxPass(name, this->_context);
        pass->create();
        this->_passes[name] = pass;
    }
   /*  // 创建对应的渲染队列 */
    if (this->_queues.find(name) == this->_queues.end())
    {
        this->_queues[name] = new GfxQueue(this->_context, this->_passes[name]);
    }
}
void GfxRenderer::createPipeline(std::string passName, std::string pipelineName)
{
   /*  // 创建管线 */
    if (this->_pipelines.find(pipelineName) != this->_pipelines.end())
    {
        // this->_Log("createPipeline:name already exists");
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
        }
    }
    if (properties.find("vert") == properties.end())
    {
       /*  // this->_Log("createPipeline:vert or frag not found");/ */
        return;
    }
    if (properties.find("frag") == properties.end())
    {
        /* // this->_Log("createPipeline:frag not found"); */
        return;
    }
    std::string shaderVert = properties["vert"];
    std::string shaderFrag = properties["frag"];
    /* // 先检测shader */
    if (this->_shaders.find(shaderVert) == this->_shaders.end())
    {
       /*  // 顶点着色器 */
        GfxShader *vertexShader = new GfxShader(this->_context, shaderVert);
        this->_shaders[shaderVert] = vertexShader;
    }
    if (this->_shaders.find(shaderFrag) == this->_shaders.end())
    {
        GfxShader *fragmentShader = new GfxShader(this->_context, shaderFrag);
        this->_shaders[shaderFrag] = fragmentShader;
    }
   /*  // 创建管线 */
    GfxPipeline *pipeline = new GfxPipeline(pipelineName, this->_context);
    pipeline->create(this->_passes[passName], this->_shaders[shaderVert], this->_shaders[shaderFrag], properties);
    this->_pipelines[pipelineName] = pipeline;
}

void GfxRenderer::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    if (this->_textures.find(textureUuid) == this->_textures.end())
    {
        GfxTexture *texture = new GfxTexture(this->_context, pixels, width, height, channels);
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

void GfxRenderer::createObject(std::string id, std::string passName, std::string pipelineType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
{
    if (this->_passes.find(passName) == this->_passes.end())
    {
        this->_Log("createGfxObject:renderPassType not found,Currently, creation is not supported");
        return;
    }
    if (this->_pipelines.find(pipelineType) == this->_pipelines.end())
    {
        this->_Log("createGfxObject:pipelineType not found,Currently, creation is not supported");
        return;
    }
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_Log("createGfxObject:id already exists");
        return;
    }
    GfxObject *object = new GfxObject(this->_context);
    object->setPass(this->_passes[passName]);
    object->setVertexs(points, colors, normals, uvs, indices);
    object->setPipeline(this->_pipelines[pipelineType]);
    this->_objects[id] = object;

    this->_queues[passName]->submit(object);
}
/* // void GfxRenderer::resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType)
// {
//     this->_Log("Currently not supported");
// } */
void GfxRenderer::destroyGfxObject(std::string id)
{
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
void GfxRenderer::setObjectTexture(std::string id, std::string texture)
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
void GfxRenderer::setUIObjectMask(std::string id, float x, float y, float width, float height, float angle)
{
    if (this->_objects.find(id) != this->_objects.end())
    {
        this->_objects[id]->setUIMask(x, y, width, height, angle);
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
    return;
}

void GfxRenderer::submit(std::string id)
{
    if (this->_objects.find(id) == this->_objects.end())
    {
        this->_Log("submit:id not found");
        return;
    }
    GfxObject *object = this->_objects[id];
    GfxPass *pass = object->pass();
    if (pass == nullptr || this->_queues.find(pass->name()) == this->_queues.end())
    {
        /* // this->_Log("submit:pass not found"); */
        return;
    }
    this->_queues[pass->name()]->submit(object);
}

void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
  /*   // this->submit("text"); */

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