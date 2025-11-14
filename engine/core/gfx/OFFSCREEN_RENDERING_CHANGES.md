# Boo-Engine 离屏渲染功能修改文档

## 📋 修改概述

本次修改为 Boo-Engine 的图形渲染系统添加了**离屏渲染（Offscreen Rendering）**功能，允许将渲染结果输出到纹理而不是窗口，实现渲染到纹理（Render to Texture, RTT）的能力。

**修改日期**: 2025-01-14
**修改范围**: `/engine/core/gfx` 目录
**兼容性**: 完全向后兼容，不影响现有窗口渲染功能

---

## 📁 文件修改清单

### 新增文件（2个）

1. **gfx-render-target.h** - 渲染目标头文件
2. **gfx-render-target.cpp** - 渲染目标实现文件

### 修改文件（6个）

1. **gfx-pass.h** - 渲染通道头文件
2. **gfx-pass.cpp** - 渲染通道实现文件
3. **gfx-queue.h** - 渲染队列头文件
4. **gfx-queue.cpp** - 渲染队列实现文件
5. **gfx-renderer.h** - 渲染器头文件
6. **gfx-renderer.cpp** - 渲染器实现文件

---

## 🆕 新增文件详细说明

### 1. gfx-render-target.h

**文件路径**: `/engine/core/gfx/gfx-render-target.h`

**功能**: 封装离屏渲染目标，管理渲染到纹理所需的所有资源

**新增类**:
```cpp
class GfxRenderTarget
{
private:
    std::string _name;                    // 渲染目标名称
    GfxContext *_context;                 // Vulkan 上下文
    GfxPass *_pass;                       // 关联的渲染通道
    uint32_t _width;                      // 纹理宽度
    uint32_t _height;                     // 纹理高度

    GfxTexture *_colorTexture;            // 颜色附件纹理（最终输出）
    GfxTexture *_depthTexture;            // 深度附件纹理
    GfxTexture *_colorMsaaTexture;        // MSAA 颜色附件（如果启用）
    VkFramebuffer _framebuffer;           // 帧缓冲区

    void _createTextures();               // 创建所需纹理
    void _createFramebuffer();            // 创建帧缓冲区
    void _cleanTextures();                // 清理纹理
    void _cleanFramebuffer();             // 清理帧缓冲区
    void _Log(std::string msg);           // 日志输出

public:
    GfxRenderTarget(std::string name, GfxContext *context, uint32_t width, uint32_t height);
    void create(GfxPass *pass);           // 初始化渲染目标
    void clear();                         // 清理资源
    void reset();                         // 重置资源

    // Getter 方法
    std::string name() const;
    uint32_t width() const;
    uint32_t height() const;
    VkFramebuffer getFramebuffer() const;
    GfxTexture* getColorTexture() const;  // 获取渲染结果纹理

    ~GfxRenderTarget();
};
```

**关键特性**:
- 自动管理颜色、深度、MSAA 纹理的生命周期
- 支持 MSAA（多重采样抗锯齿）
- 创建的颜色纹理可作为采样纹理使用（`VK_IMAGE_USAGE_SAMPLED_BIT`）
- 自动创建和管理 Framebuffer

---

### 2. gfx-render-target.cpp

**文件路径**: `/engine/core/gfx/gfx-render-target.cpp`

**核心实现**:

#### `_createTextures()` 方法
- 根据 `MsaaSamples` 设置决定是否创建 MSAA 纹理
- MSAA 模式：创建 MSAA 颜色附件 + Resolve 颜色附件 + MSAA 深度附件
- 非 MSAA 模式：创建颜色附件 + 深度附件
- 颜色纹理格式：使用 SwapChain 的格式（通常是 `VK_FORMAT_B8G8R8A8_UNORM`）
- 深度纹理格式：`VK_FORMAT_D32_SFLOAT`
- 颜色纹理用途：`VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT`

#### `_createFramebuffer()` 方法
- MSAA 模式：附件顺序为 [MSAA 颜色, Resolve 颜色, MSAA 深度]
- 非 MSAA 模式：附件顺序为 [颜色, 深度]
- 绑定到关联的 RenderPass
- 设置正确的宽度和高度

#### 资源管理
- `clear()`: 清理所有创建的纹理和 Framebuffer
- `reset()`: 先清理再重新创建（用于窗口大小改变等场景）

---

## 🔧 修改文件详细说明

### 1. gfx-pass.h

**修改内容**:

#### 新增枚举类型
```cpp
enum class PassType
{
    Window,    /* // 渲染到窗口（SwapChain） */
    Offscreen  /* // 渲染到纹理 */
};
```

#### GfxPass 类修改
```cpp
class GfxPass
{
private:
    PassType _passType;                   // 新增：Pass 类型

    // 新增私有方法
    void _createWindowRenderPass();       // 创建窗口渲染 Pass
    void _createOffscreenRenderPass();    // 创建离屏渲染 Pass

public:
    // 修改构造函数签名
    GfxPass(std::string name, GfxContext *context, PassType passType = PassType::Window);

    // 新增 Getter
    PassType passType() const;
};
```

**修改原因**:
- 窗口渲染和离屏渲染的 RenderPass 配置不同
- 窗口渲染的 finalLayout 是 `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`
- 离屏渲染的 finalLayout 是 `VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL`（可作为纹理采样）

---

### 2. gfx-pass.cpp

**修改内容**:

#### 构造函数修改
```cpp
// 旧版本
GfxPass::GfxPass(std::string name, GfxContext *context)

// 新版本
GfxPass::GfxPass(std::string name, GfxContext *context, PassType passType)
{
    this->_name = name;
    this->_context = context;
    this->_passType = passType;  // 新增
}
```

#### `_createVkRenderPass()` 方法重构
```cpp
void GfxPass::_createVkRenderPass()
{
    if (this->_passType == PassType::Window)
    {
        this->_createWindowRenderPass();
    }
    else
    {
        this->_createOffscreenRenderPass();
    }
}
```

#### 新增 `_createWindowRenderPass()` 方法
- 将原有的 `_createVkRenderPass()` 逻辑移入此方法
- 保持窗口渲染的原有逻辑不变
- 3个附件：[MSAA 颜色, Resolve 颜色(SwapChain), MSAA 深度]
- finalLayout: `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`

#### 新增 `_createOffscreenRenderPass()` 方法

**MSAA 模式（3个附件）**:
```cpp
// 附件 0: MSAA 颜色附件
attachments[0].format = SwapChainImageFormat;
attachments[0].samples = MsaaSamples;
attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// 附件 1: Resolve 颜色附件（最终输出）
attachments[1].format = SwapChainImageFormat;
attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;  // 必须存储
attachments[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;  // 可作为纹理

// 附件 2: MSAA 深度附件
attachments[2].format = VK_FORMAT_D32_SFLOAT;
attachments[2].samples = MsaaSamples;
attachments[2].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
```

**非 MSAA 模式（2个附件）**:
```cpp
// 附件 0: 颜色附件
attachments[0].format = SwapChainImageFormat;
attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

// 附件 1: 深度附件
attachments[1].format = VK_FORMAT_D32_SFLOAT;
attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
```

**关键差异**:
- 离屏渲染不使用 SwapChain 图像，所有附件都是自己创建的
- finalLayout 设置为 `SHADER_READ_ONLY_OPTIMAL` 而不是 `PRESENT_SRC_KHR`
- storeOp 必须是 `STORE`，确保渲染结果被保存

---

### 3. gfx-queue.h

**修改内容**:

#### 新增前置声明
```cpp
class GfxRenderTarget;
```

#### GfxQueue 类修改
```cpp
class GfxQueue
{
private:
    GfxRenderTarget *_renderTarget;  // 新增：可选的渲染目标

public:
    // 新增公共方法
    void setRenderTarget(GfxRenderTarget *renderTarget);
};
```

**修改原因**:
- 需要支持同一个 Queue 既可以渲染到窗口，也可以渲染到纹理
- 通过 `setRenderTarget()` 动态切换渲染目标

---

### 4. gfx-queue.cpp

**修改内容**:

#### 头文件包含
```cpp
#include "gfx-render-target.h"  // 新增
```

#### 构造函数修改
```cpp
GfxQueue::GfxQueue(GfxContext *context, GfxPass *pass)
{
    this->_context = context;
    this->_pass = pass;
    this->_renderTarget = nullptr;  // 新增：初始化为 nullptr
    this->_createBuffers();
}
```

#### 新增 `setRenderTarget()` 方法
```cpp
void GfxQueue::setRenderTarget(GfxRenderTarget *renderTarget)
{
    this->_renderTarget = renderTarget;
    // 重新创建 Framebuffers 和 CommandBuffers
    this->_cleanFramebuffers();
    this->_cleanCommandBuffers();
    this->_createBuffers();
}
```

#### `_createFramebuffers()` 方法修改

**新逻辑**:
```cpp
void GfxQueue::_createFramebuffers()
{
    this->_swapChainFramebuffers.clear();

    // 如果设置了 RenderTarget，使用它的 Framebuffer
    if (this->_renderTarget != nullptr)
    {
        // 离屏渲染模式：只需要一个 framebuffer
        this->_swapChainFramebuffers.resize(1);
        this->_swapChainFramebuffers[0] = this->_renderTarget->getFramebuffer();
        return;
    }

    // 否则，渲染到 SwapChain（原有逻辑）
    std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();
    this->_swapChainFramebuffers.resize(swapChainImageViews.size());
    // ... 为每个 SwapChain 图像创建 Framebuffer
}
```

**关键点**:
- 离屏渲染只需要 1 个 Framebuffer（由 RenderTarget 管理）
- 窗口渲染需要多个 Framebuffer（对应每个 SwapChain 图像）

#### `_beginBindRenderPass()` 方法修改

**新逻辑**:
```cpp
void GfxQueue::_beginBindRenderPass(uint32_t imageIndex)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = this->_pass->getVkRenderPass();

    // 选择正确的 framebuffer 和 extent
    if (this->_renderTarget != nullptr)
    {
        // 离屏渲染：使用 RenderTarget 的 framebuffer（索引总是 0）
        renderPassInfo.framebuffer = this->_swapChainFramebuffers[0];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {
            this->_renderTarget->width(),
            this->_renderTarget->height()
        };
    }
    else
    {
        // 窗口渲染：使用 SwapChain 的 framebuffer
        renderPassInfo.framebuffer = this->_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = this->_context->getSwapChainExtent();
    }

    // ... 设置 clearValues 并开始渲染
}
```

**关键点**:
- 离屏渲染使用自定义的宽高
- 离屏渲染的 imageIndex 始终使用 0（因为只有一个 framebuffer）

#### `_cleanFramebuffers()` 方法修改

**新逻辑**:
```cpp
void GfxQueue::_cleanFramebuffers()
{
    // 如果使用 RenderTarget，framebuffer 由 RenderTarget 管理
    if (this->_renderTarget != nullptr)
    {
        this->_swapChainFramebuffers.clear();
        return;  // 不销毁，因为所有权在 RenderTarget
    }

    // 销毁窗口 Framebuffers（原有逻辑）
    for (auto framebuffer : this->_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(this->_context->getVkDevice(), framebuffer, nullptr);
    }
    this->_swapChainFramebuffers.clear();
}
```

**关键点**:
- 离屏渲染的 Framebuffer 由 RenderTarget 管理，Queue 不负责销毁
- 窗口渲染的 Framebuffer 由 Queue 管理和销毁

---

### 5. gfx-renderer.h

**修改内容**:

#### 新增前置声明
```cpp
class GfxRenderTarget;
```

#### GfxRenderer 类修改

**新增私有成员**:
```cpp
private:
    std::map<std::string, GfxRenderTarget*> _renderTargets;  // 渲染目标集合
```

**修改公共方法签名**:
```cpp
public:
    // 修改：添加 PassType 参数
    void createRenderPass(std::string name, PassType passType = PassType::Window);
```

**新增公共方法**:
```cpp
public:
    /**
     * @brief 创建渲染目标（用于离屏渲染到纹理）
     * @param name 渲染目标名称
     * @param passName 使用的 Pass 名称（必须是 Offscreen 类型的 Pass）
     * @param width 纹理宽度
     * @param height 纹理高度
     */
    void createRenderTarget(const std::string& name, const std::string& passName,
                           uint32_t width, uint32_t height);

    /**
     * @brief 销毁渲染目标
     * @param name 渲染目标名称
     */
    void destroyRenderTarget(const std::string& name);

    /**
     * @brief 获取渲染目标的输出纹理（可用作其他渲染的输入）
     * @param name 渲染目标名称
     * @return GfxTexture* 纹理指针，如果不存在返回 nullptr
     */
    GfxTexture* getRenderTargetTexture(const std::string& name);
```

---

### 6. gfx-renderer.cpp

**修改内容**:

#### 头文件包含
```cpp
#include "gfx-render-target.h"  // 新增
```

#### `createRenderPass()` 方法修改

**旧版本**:
```cpp
void GfxRenderer::createRenderPass(std::string name)
{
    if (this->_passes.find(name) == this->_passes.end())
    {
        GfxPass *pass = new GfxPass(name, this->_context);
        // ...
    }
}
```

**新版本**:
```cpp
void GfxRenderer::createRenderPass(std::string name, PassType passType)
{
    if (this->_passes.find(name) == this->_passes.end())
    {
        GfxPass *pass = new GfxPass(name, this->_context, passType);  // 传递 passType
        // ...
    }
}
```

#### `cleanRendererState()` 方法修改

**新增**:
```cpp
void GfxRenderer::cleanRendererState()
{
    // ... 原有清理逻辑

    // 新增：渲染目标清除
    for (auto &[name, renderTarget] : this->_renderTargets)
    {
        renderTarget->clear();
    }
}
```

#### `resetRendererState()` 方法修改

**新增**:
```cpp
void GfxRenderer::resetRendererState()
{
    // ... 原有重置逻辑

    // 新增：渲染目标重置
    for (auto &[name, renderTarget] : this->_renderTargets)
    {
        renderTarget->reset();
    }
}
```

#### 新增 `createRenderTarget()` 方法实现

```cpp
void GfxRenderer::createRenderTarget(const std::string& name, const std::string& passName,
                                     uint32_t width, uint32_t height)
{
    // 检查是否已存在
    if (this->_renderTargets.find(name) != this->_renderTargets.end())
    {
        this->_Log("createRenderTarget: name already exists");
        return;
    }

    // 检查 Pass 是否存在
    if (this->_passes.find(passName) == this->_passes.end())
    {
        this->_Log("createRenderTarget: pass not found");
        return;
    }

    GfxPass* pass = this->_passes[passName];

    // 创建 RenderTarget
    GfxRenderTarget* renderTarget = new GfxRenderTarget(name, this->_context, width, height);
    renderTarget->create(pass);
    this->_renderTargets[name] = renderTarget;

    // 如果对应的 Queue 存在，设置 RenderTarget
    if (this->_queues.find(passName) != this->_queues.end())
    {
        this->_queues[passName]->setRenderTarget(renderTarget);
    }

    this->_Log("RenderTarget created: " + name);
}
```

**执行流程**:
1. 检查名称是否已存在
2. 检查关联的 Pass 是否存在
3. 创建 GfxRenderTarget 实例
4. 调用 RenderTarget 的 `create()` 方法初始化
5. 将 RenderTarget 设置到对应的 Queue

#### 新增 `destroyRenderTarget()` 方法实现

```cpp
void GfxRenderer::destroyRenderTarget(const std::string& name)
{
    if (this->_renderTargets.find(name) != this->_renderTargets.end())
    {
        delete this->_renderTargets[name];
        this->_renderTargets.erase(name);
        this->_Log("RenderTarget destroyed: " + name);
    }
}
```

#### 新增 `getRenderTargetTexture()` 方法实现

```cpp
GfxTexture* GfxRenderer::getRenderTargetTexture(const std::string& name)
{
    if (this->_renderTargets.find(name) != this->_renderTargets.end())
    {
        return this->_renderTargets[name]->getColorTexture();
    }
    return nullptr;
}
```

---

## 🎯 使用指南

### 基本使用流程

```cpp
// ===== 1. 创建离屏渲染 Pass =====
// 注意：第二个参数必须是 PassType::Offscreen
renderer->createRenderPass("offscreen_pass", PassType::Offscreen);

// ===== 2. 创建渲染管线（使用离屏 Pass） =====
renderer->createPipeline("offscreen_pass", "vert:your_vert|frag:your_frag");

// ===== 3. 创建渲染目标 =====
// 参数：名称、Pass名称、宽度、高度
renderer->createRenderTarget("my_render_target", "offscreen_pass", 1024, 1024);

// ===== 4. 创建要渲染的对象 =====
std::vector<float> points = { /* 顶点数据 */ };
std::vector<float> colors = { /* 颜色数据 */ };
std::vector<float> normals = { /* 法线数据 */ };
std::vector<float> uvs = { /* UV数据 */ };
std::vector<uint32_t> indices = { /* 索引数据 */ };

renderer->createObject("offscreen_object", "offscreen_pass",
                      points, colors, normals, uvs, indices);

// ===== 5. 设置渲染对象属性 =====
renderer->setObjectPipeline("offscreen_object", "vert:your_vert|frag:your_frag");
renderer->setObjectModelMatrix("offscreen_object", modelMatrix);
renderer->setObjectViewMatrix("offscreen_object", viewMatrix);
renderer->setObjectProjMatrix("offscreen_object", projMatrix);

// ===== 6. 提交渲染对象 =====
renderer->submit("offscreen_object");

// ===== 7. 在渲染循环中执行渲染 =====
// 离屏渲染会在 frameRenderer() 中自动执行

// ===== 8. 获取渲染结果纹理 =====
GfxTexture* resultTexture = renderer->getRenderTargetTexture("my_render_target");

// ===== 9. 将渲染结果用作其他对象的纹理 =====
// 先将纹理注册到 renderer（如果还未注册）
// 然后设置给其他对象
renderer->setObjectTexture("window_object", "my_render_target");

// ===== 10. 清理 =====
renderer->destroyRenderTarget("my_render_target");
renderer->destroyObject("offscreen_object");
```

---

### 高级使用场景

#### 场景 1: 后处理效果

```cpp
// 1. 创建离屏渲染 Pass 和 RenderTarget
renderer->createRenderPass("scene_pass", PassType::Offscreen);
renderer->createRenderTarget("scene_rt", "scene_pass", 1920, 1080);

// 2. 渲染场景到纹理
renderer->createObject("scene", "scene_pass", ...);
renderer->submit("scene");

// 3. 应用后处理（模糊、色调映射等）
renderer->createRenderPass("postprocess_pass", PassType::Offscreen);
renderer->createRenderTarget("postprocess_rt", "postprocess_pass", 1920, 1080);

GfxTexture* sceneTexture = renderer->getRenderTargetTexture("scene_rt");
renderer->createObject("postprocess_quad", "postprocess_pass", ...);
renderer->setObjectTexture("postprocess_quad", "scene_rt");
renderer->submit("postprocess_quad");

// 4. 渲染到窗口
GfxTexture* finalTexture = renderer->getRenderTargetTexture("postprocess_rt");
renderer->setObjectTexture("screen_quad", "postprocess_rt");
renderer->submit("screen_quad");
```

#### 场景 2: 阴影贴图

```cpp
// 1. 创建阴影贴图渲染目标（从光源视角渲染）
renderer->createRenderPass("shadow_pass", PassType::Offscreen);
renderer->createRenderTarget("shadow_map", "shadow_pass", 2048, 2048);

// 2. 从光源视角渲染场景
renderer->createObject("shadow_casters", "shadow_pass", ...);
renderer->setObjectViewMatrix("shadow_casters", lightViewMatrix);
renderer->setObjectProjMatrix("shadow_casters", lightProjMatrix);
renderer->submit("shadow_casters");

// 3. 在主渲染中使用阴影贴图
GfxTexture* shadowMap = renderer->getRenderTargetTexture("shadow_map");
renderer->setObjectTexture("main_scene", "shadow_map");
renderer->submit("main_scene");
```

#### 场景 3: 多渲染目标（Deferred Rendering）

```cpp
// 注意：当前实现每个 RenderTarget 只支持一个颜色附件
// 如需多个颜色附件，需创建多个 RenderTarget

// 1. G-Buffer Pass（位置、法线、颜色）
renderer->createRenderPass("gbuffer_pass", PassType::Offscreen);
renderer->createRenderTarget("gbuffer_position", "gbuffer_pass", 1920, 1080);
renderer->createRenderTarget("gbuffer_normal", "gbuffer_pass", 1920, 1080);
renderer->createRenderTarget("gbuffer_albedo", "gbuffer_pass", 1920, 1080);

// 2. 光照 Pass
renderer->createRenderPass("lighting_pass", PassType::Offscreen);
renderer->createRenderTarget("lighting_rt", "lighting_pass", 1920, 1080);

GfxTexture* positionTex = renderer->getRenderTargetTexture("gbuffer_position");
GfxTexture* normalTex = renderer->getRenderTargetTexture("gbuffer_normal");
GfxTexture* albedoTex = renderer->getRenderTargetTexture("gbuffer_albedo");

// 将 G-Buffer 纹理绑定到光照 Pass
// ...
```

---

## 📊 API 参考

### GfxRenderer 新增/修改方法

#### `createRenderPass()`

```cpp
void createRenderPass(std::string name, PassType passType = PassType::Window);
```

**参数**:
- `name`: 渲染通道名称
- `passType`: 渲染通道类型
  - `PassType::Window`: 渲染到窗口（默认）
  - `PassType::Offscreen`: 渲染到纹理

**示例**:
```cpp
renderer->createRenderPass("ui", PassType::Window);         // 窗口渲染
renderer->createRenderPass("shadow", PassType::Offscreen);  // 离屏渲染
```

---

#### `createRenderTarget()`

```cpp
void createRenderTarget(const std::string& name, const std::string& passName,
                       uint32_t width, uint32_t height);
```

**参数**:
- `name`: 渲染目标唯一名称
- `passName`: 关联的 Pass 名称（必须是 `PassType::Offscreen` 类型）
- `width`: 纹理宽度（像素）
- `height`: 纹理高度（像素）

**返回**: 无

**注意事项**:
- Pass 必须先创建且类型为 `Offscreen`
- 名称不能重复
- 宽度和高度必须大于 0

**示例**:
```cpp
renderer->createRenderTarget("rt_1024", "offscreen_pass", 1024, 1024);
renderer->createRenderTarget("shadow_2k", "shadow_pass", 2048, 2048);
```

---

#### `destroyRenderTarget()`

```cpp
void destroyRenderTarget(const std::string& name);
```

**参数**:
- `name`: 要销毁的渲染目标名称

**返回**: 无

**注意事项**:
- 会自动销毁关联的纹理和 Framebuffer
- 如果名称不存在，不会报错

**示例**:
```cpp
renderer->destroyRenderTarget("rt_1024");
```

---

#### `getRenderTargetTexture()`

```cpp
GfxTexture* getRenderTargetTexture(const std::string& name);
```

**参数**:
- `name`: 渲染目标名称

**返回**:
- `GfxTexture*`: 渲染结果纹理指针
- `nullptr`: 如果渲染目标不存在

**注意事项**:
- 返回的纹理可以作为其他渲染的输入
- 纹理的生命周期由 RenderTarget 管理，不要手动删除

**示例**:
```cpp
GfxTexture* tex = renderer->getRenderTargetTexture("rt_1024");
if (tex != nullptr)
{
    renderer->setObjectTexture("display_quad", "rt_1024");
}
```

---

### GfxRenderTarget 类

#### 构造函数

```cpp
GfxRenderTarget(std::string name, GfxContext *context,
               uint32_t width, uint32_t height);
```

#### 公共方法

| 方法 | 说明 |
|------|------|
| `void create(GfxPass *pass)` | 初始化渲染目标（创建纹理和 Framebuffer） |
| `void clear()` | 清理所有资源 |
| `void reset()` | 重置资源（先清理再重新创建） |
| `std::string name() const` | 获取名称 |
| `uint32_t width() const` | 获取宽度 |
| `uint32_t height() const` | 获取高度 |
| `VkFramebuffer getFramebuffer() const` | 获取 Framebuffer 句柄 |
| `GfxTexture* getColorTexture() const` | 获取颜色纹理（渲染结果） |

---

### PassType 枚举

```cpp
enum class PassType
{
    Window,    // 渲染到窗口（SwapChain）
    Offscreen  // 渲染到纹理
};
```

---

## ⚠️ 注意事项和限制

### 1. Pass 类型必须匹配

```cpp
// ❌ 错误：使用窗口 Pass 创建 RenderTarget
renderer->createRenderPass("ui", PassType::Window);
renderer->createRenderTarget("rt", "ui", 1024, 1024);  // 会出错

// ✅ 正确：使用离屏 Pass
renderer->createRenderPass("offscreen", PassType::Offscreen);
renderer->createRenderTarget("rt", "offscreen", 1024, 1024);
```

### 2. 渲染顺序

离屏渲染通常应该在窗口渲染之前执行：

```cpp
// ✅ 正确顺序
void render()
{
    // 1. 离屏渲染
    renderer->submit("offscreen_object");

    // 2. 窗口渲染（可以使用离屏渲染的结果）
    renderer->submit("window_object");

    // 3. 执行渲染
    renderer->frameRenderer(imageIndex, commandBuffers);
}
```

### 3. 纹理格式

- 颜色纹理格式：自动使用 SwapChain 的格式（通常是 `VK_FORMAT_B8G8R8A8_UNORM`）
- 深度纹理格式：固定为 `VK_FORMAT_D32_SFLOAT`
- 如需其他格式，需要修改 `GfxRenderTarget::_createTextures()` 方法

### 4. MSAA 支持

- 自动根据全局 `MsaaSamples` 设置创建对应的 MSAA 纹理
- MSAA 模式下会自动创建 Resolve 附件
- 最终输出的颜色纹理始终是单采样（`VK_SAMPLE_COUNT_1_BIT`）

### 5. 性能考虑

- 离屏渲染会额外消耗 GPU 内存和带宽
- 建议根据实际需求选择合适的纹理尺寸
- 不需要每帧都创建/销毁 RenderTarget，可以复用

### 6. 资源生命周期

- RenderTarget 创建的纹理由 RenderTarget 管理
- 不要手动删除 `getRenderTargetTexture()` 返回的纹理指针
- RenderTarget 销毁时会自动清理所有关联资源

---

## 🔍 常见问题

### Q1: 为什么渲染到纹理后是黑屏？

**可能原因**:
1. Pass 类型不是 `PassType::Offscreen`
2. 没有提交渲染对象（`renderer->submit()`）
3. 着色器输出有问题
4. 渲染顺序不对

**解决方法**:
```cpp
// 检查 Pass 类型
renderer->createRenderPass("offscreen", PassType::Offscreen);  // 确保是 Offscreen

// 确保提交了对象
renderer->submit("offscreen_object");

// 检查渲染循环
renderer->frameRenderer(imageIndex, commandBuffers);
```

---

### Q2: 如何在窗口中显示离屏渲染的结果？

```cpp
// 1. 创建全屏四边形
std::vector<float> quadPoints = {
    -1.0f,  1.0f, 0.0f,  // 左上
     1.0f,  1.0f, 0.0f,  // 右上
    -1.0f, -1.0f, 0.0f,  // 左下
     1.0f, -1.0f, 0.0f   // 右下
};

std::vector<float> quadUVs = {
    0.0f, 0.0f,  // 左上
    1.0f, 0.0f,  // 右上
    0.0f, 1.0f,  // 左下
    1.0f, 1.0f   // 右下
};

std::vector<uint32_t> quadIndices = {0, 1, 2, 1, 3, 2};

// 2. 创建窗口渲染对象
renderer->createObject("display_quad", "ui", quadPoints, {}, {}, quadUVs, quadIndices);

// 3. 设置纹理为离屏渲染结果
GfxTexture* offscreenTexture = renderer->getRenderTargetTexture("my_render_target");
renderer->setObjectTexture("display_quad", "my_render_target");

// 4. 提交渲染
renderer->submit("display_quad");
```

---

### Q3: 如何实现多个离屏渲染目标？

```cpp
// 创建多个 Pass 和 RenderTarget
renderer->createRenderPass("pass1", PassType::Offscreen);
renderer->createRenderPass("pass2", PassType::Offscreen);

renderer->createRenderTarget("rt1", "pass1", 1024, 1024);
renderer->createRenderTarget("rt2", "pass2", 512, 512);

// 渲染到 rt1
renderer->createObject("obj1", "pass1", ...);
renderer->submit("obj1");

// 渲染到 rt2
renderer->createObject("obj2", "pass2", ...);
renderer->submit("obj2");

// 在主渲染中使用
GfxTexture* tex1 = renderer->getRenderTargetTexture("rt1");
GfxTexture* tex2 = renderer->getRenderTargetTexture("rt2");
```

---

### Q4: 如何动态改变 RenderTarget 的尺寸？

当前实现不支持直接修改尺寸，需要先销毁再重新创建：

```cpp
// 销毁旧的
renderer->destroyRenderTarget("my_rt");

// 创建新的（新尺寸）
renderer->createRenderTarget("my_rt", "offscreen_pass", newWidth, newHeight);
```

---

### Q5: 离屏渲染支持深度测试吗？

支持。每个 RenderTarget 都会自动创建深度附件，深度测试在着色器和管线配置中启用即可。

---

## 🧪 测试建议

### 基本功能测试

```cpp
void TestOffscreenRendering()
{
    // 1. 创建离屏 Pass
    renderer->createRenderPass("test_pass", PassType::Offscreen);

    // 2. 创建 RenderTarget
    renderer->createRenderTarget("test_rt", "test_pass", 512, 512);

    // 3. 创建简单的三角形
    std::vector<float> points = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    std::vector<uint32_t> indices = {0, 1, 2};

    renderer->createObject("triangle", "test_pass", points, colors, {}, {}, indices);
    renderer->setObjectPipeline("triangle", "vert:simple|frag:simple");

    // 4. 提交渲染
    renderer->submit("triangle");

    // 5. 在渲染循环中执行
    renderer->frameRenderer(imageIndex, commandBuffers);

    // 6. 验证结果
    GfxTexture* result = renderer->getRenderTargetTexture("test_rt");
    assert(result != nullptr);

    // 7. 清理
    renderer->destroyRenderTarget("test_rt");
    renderer->destroyObject("triangle");
}
```

---

## 📈 性能优化建议

### 1. 合理选择纹理尺寸

```cpp
// ❌ 不推荐：过大的纹理
renderer->createRenderTarget("shadow", "shadow_pass", 4096, 4096);

// ✅ 推荐：根据实际需求选择
renderer->createRenderTarget("shadow", "shadow_pass", 1024, 1024);
```

### 2. 复用 RenderTarget

```cpp
// ❌ 不推荐：每帧创建/销毁
void render()
{
    renderer->createRenderTarget("temp", "pass", 1024, 1024);
    // ... 渲染
    renderer->destroyRenderTarget("temp");
}

// ✅ 推荐：创建一次，多次使用
void init()
{
    renderer->createRenderTarget("persistent_rt", "pass", 1024, 1024);
}

void render()
{
    // ... 直接使用 persistent_rt
}

void cleanup()
{
    renderer->destroyRenderTarget("persistent_rt");
}
```

### 3. 避免不必要的格式转换

- 使用与 SwapChain 相同的颜色格式
- 深度格式保持 `VK_FORMAT_D32_SFLOAT`

### 4. 批量渲染

```cpp
// 将多个对象提交到同一个离屏 Pass
renderer->submit("obj1");
renderer->submit("obj2");
renderer->submit("obj3");
// 会在同一个 RenderPass 中批量渲染
```

---

## 🛠️ 故障排除

### 编译错误

**错误**: `undefined reference to GfxRenderTarget`

**解决**: 确保在 CMakeLists.txt 中包含了新文件
```cmake
add_library(gfx
    gfx-render-target.cpp  # 添加这一行
    # ... 其他文件
)
```

---

### 运行时错误

**错误**: `failed to create offscreen render pass!`

**原因**:
- MSAA 采样数不支持
- 图像格式不支持

**解决**: 检查 `MsaaSamples` 和设备支持的格式

---

**错误**: `createRenderTarget: pass not found`

**原因**: 关联的 Pass 不存在或名称错误

**解决**: 先创建 Pass，确保名称正确
```cpp
renderer->createRenderPass("offscreen_pass", PassType::Offscreen);
renderer->createRenderTarget("rt", "offscreen_pass", 1024, 1024);  // 名称要匹配
```

---

## 📝 未来改进建议

1. **多颜色附件支持**: 支持创建具有多个颜色附件的 RenderTarget（MRT）
2. **格式自定义**: 允许指定颜色和深度纹理的格式
3. **动态尺寸调整**: 支持不销毁重新创建的情况下调整尺寸
4. **纹理 Mipmap**: 自动生成 Mipmap 链
5. **立方体贴图支持**: 支持渲染到立方体贴图（Cubemap）

---

## 📚 相关文档

- [Vulkan Specification - Render Pass](https://www.khronos.org/registry/vulkan/specs/1.3/html/vkspec.html#renderpass)
- [Vulkan Specification - Framebuffer](https://www.khronos.org/registry/vulkan/specs/1.3/html/vkspec.html#framebuffer)
- [Vulkan Tutorial - Offscreen Rendering](https://vulkan-tutorial.com/Texture_mapping/Images)

---

## 📧 联系信息

如有问题或建议，请联系开发团队。

---

**文档版本**: 1.0
**最后更新**: 2025-01-14
**维护者**: Boo-Engine Graphics Team
