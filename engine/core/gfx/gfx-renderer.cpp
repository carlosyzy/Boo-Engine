#include "gfx-renderer.h"
#include "gfx.h"
#include "gfx-context.h"
#include "base/gfx-shader.h"
#include "base/gfx-texture.h"
#include "base/gfx-render-pass.h"
#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-buffer-ubo.h"
#include "base/gfx-buffer-instance.h"
#include "base/gfx-render-texture.h"
#include "default/gfx-default-renderer.h"
#include "builtin/gfx-builtin-renderer.h"

#include "../math/mat4.h"
#include "../../log.h"

GfxRenderer::GfxRenderer()
{
    this->_defaultRenderer = new GfxDefaultRenderer("default");
    this->_builtinRenderer = new GfxBuiltinRenderer("builtin");
}
void GfxRenderer::init()
{
    Gfx::_bufferUBO = new GfxBufferUBO();
    Gfx::_bufferInstance = new GfxBufferInstance();
    this->_defaultRenderer->init();
    this->_builtinRenderer->init();
}
void GfxRenderer::createPipeline(std::string name, GfxRendererState rendererState)
{
    this->_builtinRenderer->createPipeline(name, rendererState);
}

GfxTexture *GfxRenderer::createTexture(std::string uuid, uint32_t width, uint32_t height, uint32_t channels, int textureType,const std::vector<uint8_t> *pixels, GfxTextureFormat format)
{
    if (Gfx::_textures.find(uuid) == Gfx::_textures.end())
    {
        VkFormat _format = VK_FORMAT_R8G8B8A8_UNORM;
        if (format == GfxTextureFormat::R8G8B8A8_SRGB)
        {
            _format = VK_FORMAT_R8G8B8A8_SRGB;
        }
        else if (format == GfxTextureFormat::R8G8B8A8_UNORM)
        {
            _format = VK_FORMAT_R8G8B8A8_UNORM;
        }
        else if (format == GfxTextureFormat::R8_UNORM)
        {
            _format = VK_FORMAT_R8_UNORM;
        }
        GfxTexture *texture = new GfxTexture(uuid, width, height, channels, textureType, _format);
        texture->create(pixels);
        Gfx::_textures[uuid] = texture;
    }
    try
    {
        return Gfx::_textures.at(uuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createTexture:uuid:%s not found", uuid.c_str());
        return nullptr;
    }
}
void GfxRenderer::insertTexture(GfxTexture *texture)
{
    if (Gfx::_textures.find(texture->getUuid()) != Gfx::_textures.end())
    {
        LOGI("[Gfx : Renderer] :: insertTexture:uuid:%s already exists", texture->getUuid().c_str());
        return;
    }
    Gfx::_textures[texture->getUuid()] = texture;
}
void GfxRenderer::destroyTexture(GfxTexture *texture)
{
    if (texture == nullptr)
    {
        return;
    }
    if (Gfx::_textures.find(texture->getUuid()) != Gfx::_textures.end())
    {
        // 加入销毁队列
        this->_destroyTextureCaches.push_back(texture);
        Gfx::_textures.erase(texture->getUuid());
    }
}

GfxShader *GfxRenderer::createSpirvShader(const std::string &uuid, const std::vector<uint32_t> &data)
{
    if (Gfx::_shaders.find(uuid) == Gfx::_shaders.end())
    {
        GfxShader *shader = new GfxShader(uuid);
        shader->createShaderModule(data);
        Gfx::_shaders[uuid] = shader;
    }
    try
    {
        return Gfx::_shaders.at(uuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createSpirvShader:uuid:%s not found", uuid.c_str());
        return nullptr;
    }
}
GfxShader *GfxRenderer::createGlslShader(const std::string &uuid, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros)
{
    // 生成唯一的缓存键：shaderName + 宏定义
    std::stringstream newUuid;
    newUuid << uuid;
    if (!macros.empty())
    {
        newUuid << "[";
        bool first = true;
        for (const auto &[key, value] : macros)
        {
            if (!first)
            {
                newUuid << "|";
            }
            newUuid << key << ":" << value;
            first = false;
        }
        newUuid << "]";
    }

    //  检查是否已存在
    if (Gfx::_shaders.find(newUuid.str()) == Gfx::_shaders.end())
    {
        std::vector<uint32_t> spirvCode = this->_compileShaderGlslToSpirv(shaderType, newUuid.str(), data, macros);
        if (spirvCode.empty())
        {
            return nullptr;
        }
        GfxShader *shader = new GfxShader(newUuid.str());
        shader->createShaderModule(spirvCode);
        Gfx::_shaders[newUuid.str()] = shader;
    }
    try
    {
        return Gfx::_shaders.at(newUuid.str());
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createGlslShader:uuid:%s not found", newUuid.str().c_str());
        return nullptr;
    }
}

void GfxRenderer::destroyShader(GfxShader *shader)
{
    if (shader == nullptr)
    {
        return;
    }
    if (Gfx::_shaders.find(shader->getUuid()) != Gfx::_shaders.end())
    {
        // 加入销毁队列
        this->_destroyShaderCaches.push_back(shader);
        Gfx::_shaders.erase(shader->getUuid());
    }
}
// // glslang 编译器编译GLSL着色器为SPIR-V代码
// std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, int> &macros)
// {
//     auto startTime = std::chrono::high_resolution_clock::now();

//     // 1. 确定着色器阶段
//     EShLanguage stage;
//     if (type == "Vert")
//         stage = EShLangVertex;
//     else if (type == "Frag")
//         stage = EShLangFragment;
//     else
//     {
//         LOGE("[Gfx : Renderer] :: Unsupported shader type: %s", type.c_str());
//         return {};
//     }

//     // 2. 预处理源码：提取 #version 并注入宏（无论 macros 是否为空都处理）
//     std::string processedSource;
//     {
//         std::string versionLine = "#version 450\n";
//         std::string restSource = source;

//         // 用简单字符串查找代替 regex
//         size_t versionPos = source.find("#version");
//         if (versionPos != std::string::npos)
//         {
//             size_t lineEnd = source.find('\n', versionPos);
//             if (lineEnd != std::string::npos)
//             {
//                 // 处理 \r\n
//                 size_t lineEndLen = (lineEnd > 0 && source[lineEnd - 1] == '\r') ? 2 : 1;
//                 versionLine = source.substr(versionPos, lineEnd - versionPos + 1);
//                 restSource = source.substr(lineEnd + 1);
//             }
//         }

//         std::stringstream ss;
//         ss << versionLine;
//         ss << "#define VULKAN 100\n";
//         for (const auto &[key, value] : macros)
//             ss << "#define " << key << " " << value << "\n";
//         ss << restSource;
//         processedSource = ss.str();
//     }

//     // 3. 创建着色器对象
//     glslang::TShader shader(stage);
//     const char *src = processedSource.c_str();
//     const char *name = cacheKey.c_str();
//     shader.setStrings(&src, 1);
//     // shader.setStringNames(&name, 1); // 设置文件名，报错时显示正确路径
//     shader.setEntryPoint("main");
//     shader.setSourceEntryPoint("main");

//     // 4. 设置目标环境
//     shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
//     shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
//     shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

//     // 5. 编译消息（去掉 EShMsgReadHlsl，避免影响 GLSL 编译行为）
//     EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

//     // 6. 解析（第一个参数必须是资源限制，不是字符串）
//     if (!shader.parse(GetDefaultResources(), 100, false, messages))
//     {
//         LOGE("[Gfx : Renderer] :: Shader compile failed: %s\n%s\n%s",
//              cacheKey.c_str(),
//              shader.getInfoLog(),
//              shader.getInfoDebugLog());
//         return {};
//     }

//     // 7. 链接
//     glslang::TProgram program;
//     program.addShader(&shader);
//     if (!program.link(messages))
//     {
//         LOGE("[Gfx : Renderer] :: Shader link failed: %s\n%s",
//              cacheKey.c_str(),
//              program.getInfoLog());
//         return {};
//     }

//     // 8. 生成 SPIR-V
//     const glslang::TIntermediate *intermediate = program.getIntermediate(stage);
//     if (!intermediate)
//     {
//         LOGE("[Gfx : Renderer] :: Failed to get intermediate: %s", cacheKey.c_str());
//         return {};
//     }

//     glslang::SpvOptions spvOptions{};
// #ifdef NDEBUG
//     spvOptions.generateDebugInfo = false;
//     spvOptions.disableOptimizer = false;
//     spvOptions.optimizeSize = false;
// #else
//     spvOptions.generateDebugInfo = true; // Debug 模式保留调试信息
//     spvOptions.disableOptimizer = true;
// #endif

//     std::vector<uint32_t> spirvCode;
//     glslang::GlslangToSpv(*intermediate, spirvCode, &spvOptions);

//     if (spirvCode.empty())
//     {
//         LOGE("[Gfx : Renderer] :: GlslangToSpv produced empty output: %s", cacheKey.c_str());
//         return {};
//     }

//     // 9. 输出警告
//     const char *infoLog = shader.getInfoLog();
//     if (infoLog && strlen(infoLog) > 0)
//         LOGW("[Gfx : Renderer] :: Shader warnings [%s]:\n%s", cacheKey.c_str(), infoLog);

//     // 10. 耗时
//     auto ms = std::chrono::duration<double, std::milli>(
//                   std::chrono::high_resolution_clock::now() - startTime)
//                   .count();
//     LOGI("[Gfx : Renderer] :: Compiled %s (%zu words) %.2fms", cacheKey.c_str(), spirvCode.size(), ms);

//     return spirvCode;
// }

// shaderc 编译器编译GLSL着色器为SPIR-V代码
std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, int> &macros)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    // 1. 确定阶段
    shaderc_shader_kind kind;
    if (type == "Vert")
        kind = shaderc_vertex_shader;
    else if (type == "Frag")
        kind = shaderc_fragment_shader;
    else if (type == "Comp")
        kind = shaderc_compute_shader;
    else
    {
        LOGE("[Gfx : Renderer] :: Unsupported shader type: %s", type.c_str());
        return {};
    }

    // 2. 编译选项
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
    options.AddMacroDefinition("GL_SPIRV", "1");
    options.AddMacroDefinition("VULKAN", "100");
    for (const auto &[key, value] : macros)
        options.AddMacroDefinition(key, std::to_string(value));

#ifdef NDEBUG
    options.SetOptimizationLevel(shaderc_optimization_level_performance);
#else
    options.SetGenerateDebugInfo();
    options.SetOptimizationLevel(shaderc_optimization_level_zero);
#endif

    // 3. 编译
    shaderc::Compiler compiler;
    auto result = compiler.CompileGlslToSpv(source, kind, cacheKey.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOGE("[Gfx : Renderer] :: Shader compile failed: %s\nStatus: %d\n%s",
             cacheKey.c_str(),
             result.GetCompilationStatus(),
             result.GetErrorMessage().c_str());
        return {};
    }

    if (result.GetNumWarnings() > 0)
        LOGW("[Gfx : Renderer] :: Shader warnings [%s]:\n%s",
             cacheKey.c_str(), result.GetErrorMessage().c_str());

    auto ms = std::chrono::duration<double, std::milli>(
                  std::chrono::high_resolution_clock::now() - startTime)
                  .count();
    LOGI("[Gfx : Renderer] :: Compiled %s (%zu words) %.2fms",
         cacheKey.c_str(),
         std::distance(result.cbegin(), result.cend()),
         ms);

    return {result.cbegin(), result.cend()};
}

GfxMesh *GfxRenderer::createMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    if (Gfx::_meshes.find(meshUuid) == Gfx::_meshes.end())
    {
        GfxMesh *mesh = new GfxMesh(meshUuid);
        mesh->createMesh(meshType, vertices, indices);
        Gfx::_meshes[meshUuid] = mesh;
    }
    try
    {
        return Gfx::_meshes.at(meshUuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createMesh:uuid:%s not found", meshUuid.c_str());
        return nullptr;
    }
}
GfxMesh *GfxRenderer::createUIMesh(std::string meshUuid, int meshMode, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    if (Gfx::_meshes.find(meshUuid) == Gfx::_meshes.end())
    {
        GfxMesh *mesh = new GfxMesh(meshUuid);
        mesh->createUIMesh(meshMode, vertices, indices);
        Gfx::_meshes[meshUuid] = mesh;
    }
    try
    {
        return Gfx::_meshes.at(meshUuid);
    }
    catch (const std::out_of_range &e)
    {
        LOGE("[Gfx : Renderer] :: createUIMesh:uuid:%s not found", meshUuid.c_str());
        return nullptr;
    }
}

void GfxRenderer::destroyMesh(GfxMesh *mesh)
{
    if (mesh == nullptr)
    {
        return;
    }
    if (Gfx::_meshes.find(mesh->getUuid()) != Gfx::_meshes.end())
    {
        // 加入销毁队列
        this->_destroyMeshCaches.push_back(mesh);
        Gfx::_meshes.erase(mesh->getUuid());
    }
}
void GfxRenderer::createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height)
{
    this->_builtinRenderer->createRenderQueue(renderId, priority, width, height);
}
void GfxRenderer::setRenderQueuePriority(std::string renderId, int priority)
{
    this->_builtinRenderer->setRenderQueuePriority(renderId, priority);
}
GfxRenderTexture *GfxRenderer::getRenderQueueRT(std::string renderId)
{
    return this->_builtinRenderer->getRenderQueueRT(renderId);
}
void GfxRenderer::resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height)
{
    this->_builtinRenderer->resizeRenderQueue(renderId, width, height);
}
void GfxRenderer::delRenderQueue(std::string renderId)
{
    this->_builtinRenderer->delRenderQueue(renderId);
}
void GfxRenderer::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition)
{
    this->_builtinRenderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen, cameraPosition);
}
void GfxRenderer::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh)
{
    this->_builtinRenderer->submitRenderObject(renderId, material, mesh);
}
/**
 * @brief 渲染前处理
 * 上一帧渲染完成，清空渲染目标，准备新帧渲染
 */
void GfxRenderer::frameRendererBefore()
{
    // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    Gfx::_bufferUBO->clear();
    Gfx::_bufferInstance->clear();
    this->_defaultRenderer->frameRendererBefore();
    this->_builtinRenderer->frameRendererBefore();
    // 清空销毁队列
    this->_clearDestroyCaches();
    // 更新所有纹理数据
    for (auto &[textureUuid, texture] : Gfx::_textures)
    {
        texture->updateData();
    }
    // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    // LOGI("render before duration: %f", frameDuration);
}
void GfxRenderer::frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    // std::chrono::high_resolution_clock::time_point frameStart1 = std::chrono::high_resolution_clock::now();
    // 先获取上一帧的离屏渲染输出提交到默认队列
    std::vector<std::string> pipelineOutds;
    this->_builtinRenderer->getOffScreenOutds(pipelineOutds);
    this->_defaultRenderer->frameRenderer(imageIndex, commandBuffers, pipelineOutds);
    // // 默认渲染队列
    // std::chrono::high_resolution_clock::time_point frameEnd1 = std::chrono::high_resolution_clock::now();
    // float frameDuration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd1 - frameStart1).count() / 1000000.0f;
    // LOGI("render default renderer end duration: %f", frameDuration1);



//    std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    // 离屏渲染队列
    // LOGI("render frame renderer start...........");
    this->_builtinRenderer->frameRenderer(commandBuffers);
    // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    // LOGI("render frame renderer end duration: %f", frameDuration);
}
void GfxRenderer::frameRendererAfter()
{
    // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    this->_defaultRenderer->frameRendererAfter();
    this->_builtinRenderer->frameRendererAfter();
    // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    // LOGI("render after duration: %f", frameDuration);
}
void GfxRenderer::_clearDestroyCaches()
{
    // 清空销毁纹理队列
    for (auto &texture : this->_destroyTextureCaches)
    {
        texture->destroy();
        delete texture;
        texture = nullptr;
    }
    this->_destroyTextureCaches.clear();
    // 清空销毁着色器队列
    for (auto &shader : this->_destroyShaderCaches)
    {
        shader->destroy();
        delete shader;
        shader = nullptr;
    }
    this->_destroyShaderCaches.clear();
    // 清空销毁渲染纹理队列
    for (auto &renderTexture : this->_destroyRenderTextureCaches)
    {
        renderTexture->destroy();
        delete renderTexture;
        renderTexture = nullptr;
    }
    this->_destroyRenderTextureCaches.clear();
    // 清空销毁网格队列
    for (auto &mesh : this->_destroyMeshCaches)
    {
        mesh->destroy();
        delete mesh;
        mesh = nullptr;
    }
    this->_destroyMeshCaches.clear();
}
void GfxRenderer::_cleanRendererState()
{
    this->_defaultRenderer->_cleanRendererState();
}
void GfxRenderer::_resetRendererState()
{
    this->_defaultRenderer->_resetRendererState();
}

GfxRenderer::~GfxRenderer()
{
}