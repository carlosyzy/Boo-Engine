# Vulkan Shader 编译方案文档

## 目录
1. [概述与对比](#概述与对比)
2. [CMakeLists 配置](#cmakelists-配置)
3. [方案 A：glslang 编译器](#方案-a-glslang-编译器)
4. [方案 B：shaderc 编译器](#方案-b-shaderc-编译器)
5. [上层调用封装](#上层调用封装)
6. [注意事项与踩坑记录](#注意事项与踩坑记录)

---

## 概述与对比

| 维度 | glslang | shaderc |
|------|---------|---------|
| 来源 | Khronos 官方 | Google（基于 glslang） |
| API 风格 | 较底层，步骤多 | 简洁，一行编译 |
| 宏注入 | 手动拼接到源码头部 | API 直接添加 |
| 优化 | 需配合 SPIRV-Tools | 内置优化级别 |
| 依赖库 | `libglslang.a` + `libSPIRV.a` + `libglslang-default-resource-limits.a` | `libshaderc_combined.a`（包含所有依赖） |
| 适合场景 | 需要精细控制编译流程 | 快速集成，接口简单 |

---

## CMakeLists 配置

```cmake
# 头文件目录
target_include_directories(Engine BEFORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/include   # glslang 头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc/include   # shaderc 头文件
    # ... 其他头文件目录
)

# 链接库（macOS / arm64）
if(APPLE)
    target_link_libraries(Engine PRIVATE
        # glslang 方案需要以下三个库
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libglslang.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV-Tools.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libSPIRV-Tools-opt.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/mac/libglslang-default-resource-limits.a

        # shaderc 方案只需一个合并库（内部已包含 glslang + SPIRV-Tools）
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc/lib/mac/libshaderc_combined.a
    )
elseif(WIN32)
    target_link_libraries(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslang.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/SPIRV.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang/lib/win/glslang-default-resource-limits.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc/lib/win/shaderc_combined.lib
    )
endif()
```

> **注意**：两种方案可以同时链接，也可以只选其中一种。
> `libglslang-default-resource-limits.a` 提供 `GetDefaultResources()` 函数，
> 缺少这个库会导致链接错误：`Undefined symbols: GetDefaultResources()`

---

## 方案 A：glslang 编译器

### 头文件

```cpp
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
```

### 全局初始化（必须调用）

```cpp
// 引擎启动时调用一次
void GfxRenderer::initShaderCompiler()
{
    glslang::InitializeProcess();
}

// 引擎关闭时调用一次
void GfxRenderer::destroyShaderCompiler()
{
    glslang::FinalizeProcess();
}
```

> 不调用 `InitializeProcess()` 会导致编译崩溃或结果异常。

### 完整编译实现

```cpp
std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(
    const std::string &type,
    const std::string &cacheKey,
    const std::string &source,
    const std::map<std::string, int> &macros)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    // 1. 确定着色器阶段
    EShLanguage stage;
    if (type == "Vert")       stage = EShLangVertex;
    else if (type == "Frag")  stage = EShLangFragment;
    else if (type == "Comp")  stage = EShLangCompute;
    else
    {
        LOGE("[Gfx : Renderer] :: Unsupported shader type: %s", type.c_str());
        return {};
    }

    // 2. 预处理源码：提取 #version，注入宏定义
    //    注意：GL_ 开头的名字是 GLSL 保留前缀，不能手动 #define
    //          GL_SPIRV 由 glslang 在设置 SPIR-V 目标时自动注入，不需要手动添加
    std::string processedSource;
    {
        std::string versionLine = "#version 450\n";
        std::string restSource  = source;

        // 用字符串查找代替 regex（更快，避免 \r\n 兼容问题）
        size_t versionPos = source.find("#version");
        if (versionPos != std::string::npos)
        {
            size_t lineEnd = source.find('\n', versionPos);
            if (lineEnd != std::string::npos)
            {
                versionLine = source.substr(versionPos, lineEnd - versionPos + 1);
                restSource  = source.substr(lineEnd + 1);
            }
        }

        std::stringstream ss;
        ss << versionLine;
        ss << "#define VULKAN 100\n";   // 标识当前目标平台为 Vulkan
        for (const auto &[key, value] : macros)
            ss << "#define " << key << " " << value << "\n";
        ss << restSource;
        processedSource = ss.str();
    }

    // 3. 创建着色器对象
    glslang::TShader shader(stage);
    const char *src = processedSource.c_str();
    shader.setStrings(&src, 1);
    shader.setEntryPoint("main");
    shader.setSourceEntryPoint("main");

    // 4. 设置目标环境（Vulkan 1.0 + SPIR-V 1.0）
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    // 5. 编译消息标志（不加 EShMsgReadHlsl，避免影响 GLSL 编译行为）
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    // 6. 解析（第一个参数必须是资源限制结构体，由 libglslang-default-resource-limits 提供）
    if (!shader.parse(GetDefaultResources(), 100, false, messages))
    {
        LOGE("[Gfx : Renderer] :: Shader compile failed: %s\n%s\n%s",
             cacheKey.c_str(),
             shader.getInfoLog(),
             shader.getInfoDebugLog());
        return {};
    }

    // 7. 创建程序并链接
    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(messages))
    {
        LOGE("[Gfx : Renderer] :: Shader link failed: %s\n%s",
             cacheKey.c_str(),
             program.getInfoLog());
        return {};
    }

    // 8. 获取中间表示
    const glslang::TIntermediate *intermediate = program.getIntermediate(stage);
    if (!intermediate)
    {
        LOGE("[Gfx : Renderer] :: Failed to get intermediate: %s", cacheKey.c_str());
        return {};
    }

    // 9. 生成 SPIR-V（区分 Debug / Release）
    glslang::SpvOptions spvOptions{};
#ifdef NDEBUG
    spvOptions.generateDebugInfo = false;  // Release：不含调试信息
    spvOptions.disableOptimizer  = false;  // Release：开启优化
    spvOptions.optimizeSize      = false;
#else
    spvOptions.generateDebugInfo = true;   // Debug：保留调试信息
    spvOptions.disableOptimizer  = true;   // Debug：关闭优化，方便调试
#endif

    std::vector<uint32_t> spirvCode;
    glslang::GlslangToSpv(*intermediate, spirvCode, &spvOptions);

    if (spirvCode.empty())
    {
        LOGE("[Gfx : Renderer] :: GlslangToSpv produced empty output: %s", cacheKey.c_str());
        return {};
    }

    // 10. 输出警告
    const char *infoLog = shader.getInfoLog();
    if (infoLog && strlen(infoLog) > 0)
        LOGW("[Gfx : Renderer] :: Shader warnings [%s]:\n%s", cacheKey.c_str(), infoLog);

    // 11. 耗时统计
    auto ms = std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - startTime).count();
    LOGI("[Gfx : Renderer] :: Compiled %s (%zu words) %.2fms",
         cacheKey.c_str(), spirvCode.size(), ms);

    return spirvCode;
}
```

### glslang 编译流程图

```
GLSL 源码
    │
    ▼
预处理（提取 #version，注入宏）
    │
    ▼
glslang::TShader::parse()       ← 词法/语法分析
    │
    ▼
glslang::TProgram::link()       ← 链接（可多个 shader 联合）
    │
    ▼
TProgram::getIntermediate()     ← 获取 AST 中间表示
    │
    ▼
glslang::GlslangToSpv()         ← 生成 SPIR-V 字节码
    │
    ▼
std::vector<uint32_t>           ← 最终 SPIR-V
```

---

## 方案 B：shaderc 编译器

### 头文件

```cpp
#include <shaderc/shaderc.hpp>
```

### 全局初始化

shaderc 不需要全局初始化，`shaderc::Compiler` 直接使用即可。

### 完整编译实现

```cpp
std::vector<uint32_t> GfxRenderer::_compileShaderGlslToSpirv(
    const std::string &type,
    const std::string &cacheKey,
    const std::string &source,
    const std::map<std::string, int> &macros)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    // 1. 确定着色器阶段
    shaderc_shader_kind kind;
    if (type == "Vert")       kind = shaderc_vertex_shader;
    else if (type == "Frag")  kind = shaderc_fragment_shader;
    else if (type == "Comp")  kind = shaderc_compute_shader;
    else
    {
        LOGE("[Gfx : Renderer] :: Unsupported shader type: %s", type.c_str());
        return {};
    }

    // 2. 编译选项
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);

    // 注意：GL_SPIRV 不能手动添加（GL_ 前缀是 GLSL 保留名），shaderc 会自动处理
    options.AddMacroDefinition("VULKAN", "100");
    for (const auto &[key, value] : macros)
        options.AddMacroDefinition(key, std::to_string(value));

#ifdef NDEBUG
    options.SetOptimizationLevel(shaderc_optimization_level_performance);  // Release：性能优化
#else
    options.SetGenerateDebugInfo();                                          // Debug：生成调试信息
    options.SetOptimizationLevel(shaderc_optimization_level_zero);          // Debug：不优化
#endif

    // 3. 编译
    shaderc::Compiler compiler;
    auto result = compiler.CompileGlslToSpv(source, kind, cacheKey.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOGE("[Gfx : Renderer] :: Shader compile failed: %s\nStatus: %d\n%s",
             cacheKey.c_str(),
             (int)result.GetCompilationStatus(),
             result.GetErrorMessage().c_str());
        return {};
    }

    // 4. 输出警告
    if (result.GetNumWarnings() > 0)
        LOGW("[Gfx : Renderer] :: Shader warnings [%s]:\n%s",
             cacheKey.c_str(), result.GetErrorMessage().c_str());

    // 5. 耗时统计
    auto ms = std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - startTime).count();
    LOGI("[Gfx : Renderer] :: Compiled %s (%zu words) %.2fms",
         cacheKey.c_str(),
         (size_t)std::distance(result.cbegin(), result.cend()),
         ms);

    return {result.cbegin(), result.cend()};
}
```

### shaderc 编译流程图

```
GLSL 源码
    │
    ▼
shaderc::CompileOptions         ← 配置宏、目标环境、优化级别
    │
    ▼
shaderc::Compiler::CompileGlslToSpv()   ← 一步完成编译
    │
    ▼
SpvCompilationResult            ← 检查状态、提取数据
    │
    ▼
std::vector<uint32_t>           ← 最终 SPIR-V
```

---

## 上层调用封装

两种方案的 `_compileShaderGlslToSpirv` 接口完全一致，上层代码无需改动：

```cpp
GfxShader *GfxRenderer::createGlslShader(
    const std::string &uuid,
    const std::string &shaderType,
    const std::string &data,
    const std::map<std::string, int> &macros)
{
    // 生成唯一缓存键（uuid + 宏定义组合）
    std::stringstream newUuid;
    newUuid << uuid;
    if (!macros.empty())
    {
        newUuid << "[";
        bool first = true;
        for (const auto &[key, value] : macros)
        {
            if (!first) newUuid << "|";
            newUuid << key << ":" << value;
            first = false;
        }
        newUuid << "]";
    }

    // 已存在直接返回缓存
    if (Gfx::_shaders.find(newUuid.str()) == Gfx::_shaders.end())
    {
        std::vector<uint32_t> spirvCode =
            this->_compileShaderGlslToSpirv(shaderType, newUuid.str(), data, macros);

        if (spirvCode.empty())
            return nullptr;

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
        LOGE("[Gfx : Renderer] :: createGlslShader: uuid not found: %s", newUuid.str().c_str());
        return nullptr;
    }
}

void GfxRenderer::destroyShader(GfxShader *shader)
{
    if (shader == nullptr) return;

    if (Gfx::_shaders.find(shader->getUuid()) != Gfx::_shaders.end())
    {
        this->_destroyShaderCaches.push_back(shader);
        Gfx::_shaders.erase(shader->getUuid());
    }
}
```

---

## 注意事项与踩坑记录

### 1. GL_ 前缀宏不能手动定义

```glsl
// GLSL 规范：GL_ 开头是保留前缀，用户不能手动 #define 或 #undef
// 错误做法（会导致编译报错）：
#define GL_SPIRV 1

// GL_SPIRV 由编译器在目标为 SPIR-V 时自动注入，无需手动添加
```

报错示例：
```
ERROR: 0:2: '#define' : names beginning with "GL_" can't be (un)defined: GL_SPIRV
```

### 2. glslang 必须全局初始化

```cpp
// 忘记调用会导致 parse() 崩溃或异常结果
glslang::InitializeProcess();   // 启动时
glslang::FinalizeProcess();     // 关闭时
```

### 3. `GetDefaultResources()` 需要链接额外库

```
Undefined symbols: "GetDefaultResources()"
```

解决：CMakeLists 中必须链接 `libglslang-default-resource-limits.a`。

### 4. parse() 第一个参数是资源限制，不是字符串

```cpp
// 错误（会产生未定义行为）
shader.parse(&shaderStrings, 100, false, messages);

// 正确
shader.parse(GetDefaultResources(), 100, false, messages);
```

### 5. 不要加 EShMsgReadHlsl

```cpp
// 错误（影响 GLSL 编译行为）
EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgReadHlsl);

// 正确
EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
```

### 6. 宏注入必须在 #version 之后

GLSL 要求 `#version` 必须是文件第一条有效指令，所有 `#define` 必须放在它后面：

```glsl
// 正确顺序
#version 450
#define VULKAN 100
#define MY_MACRO 1
// ... 其余代码
```

### 7. Debug / Release 编译选项区别

| 选项 | Debug | Release |
|------|-------|---------|
| 调试信息 | 保留（方便 RenderDoc 调试） | 不生成 |
| 优化 | 关闭 | 开启 |
| SPIR-V 大小 | 较大 | 较小 |
