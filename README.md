# Boo-Engine

<div align="center">

**一个简单、灵活、高效的 Vulkan 游戏引擎**

基于 Vulkan 开发 | 跨平台支持 | 现代架构设计

</div>

---

## 项目简介

**Boo-Engine** 是一个用 C++ 开发的跨平台游戏引擎，采用高性能的 Vulkan 图形 API，支持 2D/3D 渲染。引擎设计遵循现代游戏引擎的最佳实践，提供场景管理、组件系统、事件系统、UI 框架等完整功能。

### 作者
- **宗宝**

### 技术栈
- **渲染 API**: Vulkan
- **编程语言**: C++
- **字体渲染**: FreeType
- **构建系统**: CMake

---

## 核心特性

- **跨平台支持** - Windows、macOS、Linux 等多平台运行
- **现代渲染架构** - 基于 Vulkan，支持多相机、离屏渲染、实例化绘制
- **完整的场景系统** - 场景图设计，支持 2D/3D 分层
- **灵活的组件系统** - ECS 架构，轻松扩展游戏功能
- **强大的 UI 框架** - 响应式布局、自动适配、事件系统
- **事件驱动架构** - 低耦合的模块通信
- **资产异步加载** - 不阻塞主线程的资源管理
- **可视化编辑器** - 类似 Unity 的场景编辑工具

---

## 引擎架构

Boo-Engine 采用模块化设计，各个模块职责清晰，易于理解和扩展。

### 架构图

```
Boo-Engine
│
├─ 引擎启动层 (Engine)
│  └─ 窗口管理 + 主循环
│
├─ 游戏核心 (Game)
│  ├─ 场景管理
│  ├─ 定时器调度
│  └─ 全局状态
│
├─ 场景系统 (Scene)
│  ├─ 场景图 (Scene Graph)
│  ├─ 节点树 (Node Hierarchy)
│  └─ 2D/3D 节点分层
│
├─ 组件系统 (Component)
│  ├─ 相机 (Camera)
│  ├─ UI 组件 (Sprite/Text/Layout)
│  └─ 自定义组件 (用户扩展)
│
├─ 渲染系统 (Renderer)
│  ├─ Vulkan 图形层 (GFX)
│  ├─ 渲染管线 (Pipeline)
│  └─ 批量渲染 (Batch Rendering)
│
├─ 输入系统 (Input)
│  ├─ 鼠标 & 键盘
│  └─ UI 事件分发
│
├─ 资产系统 (Assets)
│  ├─ 纹理加载
│  ├─ 材质管理
│  └─ 异步加载
│
├─ 事件系统 (Event)
│  └─ 全局事件总线
│
└─ 编辑器 (Editor)
   ├─ 场景编辑器
   ├─ 层级视图
   └─ 属性检查器
```

---

## 模块功能说明

### 1. 引擎启动层 (Engine)

**作用**: 程序的入口点，负责窗口创建和主循环。

**对外接口**:
```cpp
Engine::init(Window* window, Platform platform)  // 初始化引擎
Engine::tick(float deltaTime)                     // 每帧更新
Engine::updateViewSize(width, height)            // 窗口大小改变
```

**通俗理解**: 就像游戏的"发动机"，负责启动整个引擎并保持运转。

---

### 2. 游戏核心 (Game)

**作用**: 引擎的"大脑"，统筹管理所有子系统。

**主要职责**:
- 管理场景的创建和销毁
- 调度定时器任务
- 协调各个子系统运行
- 管理多个相机

**对外接口**:
```cpp
Game::getInstance()                              // 获取全局实例（单例模式）
Game::openScene(Scene* scene)                    // 打开场景
Game::destroyScene(Scene* scene)                 // 销毁场景
Game::schedule(callback, interval, repeat)       // 定时执行任务
Game::extractCamera(Camera* camera)              // 添加相机
```

**通俗理解**: 类似游戏的"导演"，负责协调演员（场景、相机）和剧本（定时任务）。

---

### 3. 场景系统 (Scene)

**作用**: 游戏世界的容器，所有游戏对象都存在于场景中。

#### 3.1 场景 (Scene)
**对外接口**:
```cpp
Scene::create()                                  // 创建场景
scene->createRoot2D()                            // 创建 2D 根节点（用于 UI）
scene->createRoot3D()                            // 创建 3D 根节点（用于游戏物体）
scene->setActive(bool)                           // 激活/暂停场景
```

**层级结构**:
```
Scene (场景根)
├─ Root3D (3D 世界)
│  ├─ 游戏角色
│  ├─ 环境物体
│  └─ 灯光
└─ Root2D (2D 界面)
   ├─ UI 面板
   ├─ 按钮
   └─ 文本
```

#### 3.2 节点 (Node)
**作用**: 场景中的基本对象，可以是角色、UI 元素、灯光等。

**核心属性**:
- **位置** (Position) - 对象在空间的坐标
- **旋转** (Rotation) - 对象的朝向
- **缩放** (Scale) - 对象的大小
- **父子关系** - 节点可以嵌套（如角色手上拿着武器）

**对外接口**:
```cpp
node->setPosition(x, y, z)                       // 设置位置
node->setRotation(x, y, z, w)                    // 设置旋转（四元数）
node->setScale(x, y, z)                          // 设置缩放
node->addChild(childNode)                        // 添加子节点
node->addComponent("ComponentName")              // 添加组件
```

**通俗理解**: 场景是"舞台"，节点是"道具"，你可以移动、旋转、缩放道具，也可以让道具互相连接。

---

### 4. 组件系统 (Component)

**作用**: 为节点添加功能的"模块"，采用组合优于继承的设计。

#### 核心理念 (ECS 架构)
- **Entity** (实体) = Node (节点)
- **Component** (组件) = 功能模块
- **System** (系统) = 自动调用组件的 Update

**生命周期**:
```cpp
Awake()        // 初始化（仅一次）
Enable()       // 激活时
Update(dt)     // 每帧更新
Disable()      // 禁用时
destroy()      // 销毁时
```

#### 内置组件

| 组件名称 | 功能说明 |
|---------|---------|
| **Camera** | 定义视角和渲染目标 |
| **UISprite** | 显示 2D 图片 |
| **UIText** | 显示文本 |
| **UIMask** | 实现遮罩效果（如进度条） |
| **UILayoutHorizontal** | 水平自动布局 |
| **UILayoutVertical** | 垂直自动布局 |
| **UIWidget** | 响应式适配（屏幕大小改变时自动调整） |

**对外接口**:
```cpp
// 创建组件
auto sprite = node->addComponent("UISprite");
auto text = node->addComponent("UIText");

// 获取组件
auto camera = node->getComponent("Camera");

// 销毁组件
node->destroyAllComponents();
```

**通俗理解**: 节点是"空盒子"，组件是"功能插件"。你可以给盒子装上"显示图片"插件、"显示文字"插件等，随意组合。

---

### 5. 渲染系统 (Renderer)

**作用**: 将场景中的内容绘制到屏幕上。

#### 5.1 相机 (Camera)
**作用**: 定义"从哪里看"和"看到什么"。

**核心功能**:
```cpp
camera->setPriority(0)                           // 渲染优先级（数字越小越先渲染）
camera->setVisibility(layerMask)                 // 可见层（哪些对象能被看到）
camera->setIsOnScreen(true)                      // 是否渲染到屏幕
camera->resize(width, height)                    // 调整渲染区域大小
```

**多相机支持**:
- 相机 1（优先级 0）：渲染 3D 世界
- 相机 2（优先级 1）：渲染 2D UI

**离屏渲染**:
相机可以渲染到纹理（RenderTexture），然后作为材质贴到其他对象上（如监控画面、小地图）。

#### 5.2 渲染流程
```
1. 遍历所有相机（按优先级）
2. 对每个相机：
   ├─ 设置视图矩阵和投影矩阵
   ├─ 遍历场景节点树
   ├─ 收集可见的渲染对象
   ├─ 按材质批量提交绘制命令
   └─ 执行 Vulkan 渲染
3. 交换缓冲区（显示到屏幕）
```

**通俗理解**: 相机是"摄像机"，可以有多个摄像机从不同角度拍摄，最后合成一张画面。

---

### 6. Vulkan 图形层 (GFX)

**作用**: 封装 Vulkan 底层 API，提供高性能渲染能力。

#### 核心模块

| 模块 | 说明 |
|------|------|
| **GfxContext** | 管理 Vulkan 设备、队列、交换链 |
| **GfxRenderer** | 渲染管线和命令提交 |
| **GfxShader** | 顶点和片段着色器 |
| **GfxTexture** | 纹理资源 |
| **GfxMaterial** | 材质（纹理 + 着色器） |
| **GfxMesh** | 网格（顶点 + 索引） |
| **GfxPipeline** | 渲染管线状态 |
| **GfxRenderTexture** | 离屏渲染目标 |

**对外接口**:
```cpp
// 创建纹理
GfxMgr::createTexture(uuid, width, height, channels, pixels)

// 创建着色器
GfxMgr::createGlslShader(name, type, glslCode, macros)

// 提交渲染对象
GfxMgr::submitRenderObject(renderId, material, mesh, instanceData)
```

**性能优化**:
- **实例化绘制** - 一次绘制多个相同对象
- **批量渲染** - 合并相同材质的对象
- **Bindless 纹理** - 减少状态切换开销

**通俗理解**: Vulkan 是底层"绘画工具"，GFX 层把复杂的画笔、画布操作封装成"画个圆"、"画条线"这样的简单命令。

---

### 7. UI 框架

**作用**: 提供丰富的 UI 组件和布局系统。

#### 核心 UI 组件

**UISprite** - 图片显示
```cpp
auto sprite = node->addComponent("UISprite");
// 设置纹理、颜色等
```

**UIText** - 文本显示
```cpp
auto text = node->addComponent("UIText");
// 设置文字内容、字体大小、颜色等
```

**UIMask** - 遮罩效果
```cpp
auto mask = node->addComponent("UIMask");
// 实现进度条、头像裁剪等效果
```

**UILayout** - 自动布局
```cpp
// 水平布局（按钮横向排列）
auto layout = node->addComponent("UILayoutHorizontal");

// 垂直布局（列表纵向排列）
auto layout = node->addComponent("UILayoutVertical");
```

**UIWidget** - 响应式适配
```cpp
auto widget = node->addComponent("UIWidget");
widget->setHorizontal(HORIZONTAL_ALL, {});  // 水平拉伸填满
widget->setVertical(VERTICAL_TOP, {offset});  // 顶部对齐
```

**适配模式**:
- **LEFT/RIGHT** - 左/右对齐
- **TOP/BOTTOM** - 上/下对齐
- **CENTER** - 居中
- **ALL** - 拉伸填满

**通俗理解**: UI 框架提供"按钮"、"文本框"、"进度条"等现成的零件，还能自动排版，适配不同屏幕大小。

---

### 8. 输入系统 (Input)

**作用**: 处理鼠标、键盘输入，并分发到 UI 节点。

**对外接口**:
```cpp
// 注册节点输入事件
Input::onNodeInputEvent(node, INPUT_MOUSE_DOWN, [](NodeInputValue value) {
    // 处理点击事件
});

// 移除输入监听
Input::offNodeInputEvent(inputID);
```

**输入事件类型**:
- `INPUT_MOUSE_DOWN` - 鼠标按下
- `INPUT_MOUSE_UP` - 鼠标抬起
- `INPUT_MOUSE_MOVE` - 鼠标移动
- `INPUT_MOUSE_ENTER` - 鼠标进入节点
- `INPUT_MOUSE_LEAVE` - 鼠标离开节点

**事件传播机制**:
从场景根节点开始向下传播，最深的节点优先处理（类似 HTML DOM 事件）。

**通俗理解**: 当你点击屏幕时，输入系统会找到"被点击的按钮"，然后通知它"你被点击了"。

---

### 9. 资产系统 (Assets)

**作用**: 管理游戏资源（纹理、材质、网格、场景等）。

**对外接口**:
```cpp
AssetsManager::getInstance()->loadAsset(uuid)    // 加载资源
AssetsManager::getInstance()->getAsset(uuid)     // 获取资源
AssetsManager::getInstance()->setAssetsRoot(path) // 设置资源根目录
```

**资产类型**:
- **TextureAsset** - 图像资源（PNG/JPG 等）
- **MaterialAsset** - 材质资源
- **ShaderAsset** - 着色器资源
- **MeshAsset** - 网格资源
- **SceneAsset** - 场景资源

**异步加载**:
资源加载不会阻塞主线程，加载完成后通过回调通知。

**通俗理解**: 资产系统是"仓库管理员"，你需要图片、音乐、模型时，告诉它，它会去仓库找出来给你。

---

### 10. 事件系统 (Event)

**作用**: 全局事件总线，实现模块间解耦通信。

**对外接口**:
```cpp
// 监听事件
Event::on<MyClass>("event_name", [](EventValue value) {
    // 处理事件
}, this);

// 触发事件
Event::emit("event_name", 123);  // 值可以是 string/int/float/bool

// 移除监听
Event::off(listenerId);
Event::offTarget<MyClass>(this);  // 移除实例的所有监听
```

**应用场景**:
- 游戏逻辑事件（如角色死亡、关卡完成）
- UI 事件（如按钮点击、对话框关闭）
- 系统事件（如资源加载完成、场景切换）

**通俗理解**: 事件系统是"广播电台"，一个模块可以"发布消息"，其他模块可以"订阅频道"来接收消息。

---

### 11. 字体系统 (Font)

**作用**: 使用 FreeType 将文本渲染成纹理。

**对外接口**:
```cpp
FreetypeMgr::crateFont(textTexture, str, fontSize, lineHeight)
FreetypeMgr::clearFont(key)
```

**工作流程**:
1. 加载字体文件（.ttf）
2. 解析文本字符串
3. 生成纹理位图
4. 返回纹理数据供 UIText 使用

**通俗理解**: 字体系统把"文字"变成"图片"，这样就能用渲染引擎显示文字了。

---

### 12. 数学库 (Math)

**作用**: 提供 3D 数学运算支持。

**数学类型**:
- **Vec3** - 3D 向量（位置、方向）
- **Vec2** - 2D 向量（UI 坐标）
- **Mat4** - 4x4 矩阵（变换）
- **Quat** - 四元数（旋转）
- **Color** - 颜色（RGBA）
- **Size** - 大小（宽高）

**对外接口**:
```cpp
Vec3 position(1.0f, 2.0f, 3.0f);
position.add(Vec3(1, 0, 0));         // 向量加法
float length = position.len();        // 向量长度

Mat4 transform;
transform.translate(1, 2, 3);         // 平移
transform.scale(2, 2, 2);             // 缩放
```

**通俗理解**: 数学库提供"计算器"，帮你算物体的位置、角度、距离等。

---

### 13. 编辑器 (Editor)

**作用**: 可视化编辑工具，类似 Unity Editor。

**核心功能**:
- **场景编辑器** - 可视化编辑场景
- **层级视图** - 树状显示节点结构
- **属性检查器** - 编辑节点和组件属性
- **资源浏览器** - 管理项目资源
- **游戏预览** - 实时预览游戏效果

**工作流程**:
1. 在编辑器中创建场景
2. 添加节点和组件
3. 调整属性
4. 保存场景文件
5. 运行时加载场景

**通俗理解**: 编辑器是"可视化工作台"，不用写代码就能拖拽创建游戏界面和场景。

---

## 使用示例

### 创建一个简单场景

```cpp
// 1. 创建场景
auto scene = Scene::create();
Game::getInstance()->openScene(scene);

// 2. 创建 UI 根节点
auto root2D = scene->createRoot2D();

// 3. 创建一个精灵节点
auto spriteNode = Node2D::create();
spriteNode->setPosition(0, 0, 0);
spriteNode->setSize(100, 100);
root2D->addChild(spriteNode);

// 4. 添加精灵组件
auto sprite = spriteNode->addComponent("UISprite");
// 设置纹理和属性...

// 5. 注册点击事件
Input::onNodeInputEvent(spriteNode, INPUT_MOUSE_DOWN, [](NodeInputValue value) {
    std::cout << "Sprite clicked!" << std::endl;
});
```

### 使用定时器

```cpp
// 每 1 秒执行一次，重复 5 次
Game::getInstance()->schedule([]() {
    std::cout << "Hello every second!" << std::endl;
}, 1.0f, 5);

// 5 秒后执行一次
Game::getInstance()->scheduleOnce([]() {
    std::cout << "Hello after 5 seconds!" << std::endl;
}, 5.0f);
```

### 使用事件系统

```cpp
// 监听事件
Event::on<MyClass>("player_died", [](EventValue value) {
    int score = std::get<int>(value);
    std::cout << "Game Over! Score: " << score << std::endl;
}, this);

// 触发事件
Event::emit("player_died", 12345);

// 移除监听
Event::offTarget<MyClass>(this);
```

---

## 程序启动流程

```
1. main() 函数启动
   ↓
2. 创建窗口 (Window)
   ↓
3. Engine::init()
   ├─ Game::init()
   │  ├─ Vulkan 初始化 (GFX)
   │  ├─ 输入系统初始化 (Input)
   │  ├─ 事件系统初始化 (Event)
   │  ├─ 字体系统初始化 (Font)
   │  ├─ 资产系统初始化 (Assets)
   │  └─ 加载启动画面 (Alpha Scene)
   ↓
4. 游戏主循环
   └─ while (running)
      ├─ 处理输入事件
      ├─ 更新所有节点和组件
      ├─ 渲染场景
      └─ 交换缓冲区（显示画面）
```

---

## 每帧渲染流程

```
1. 遍历所有相机（按优先级排序）
   ↓
2. 对每个相机：
   ├─ 设置视图矩阵和投影矩阵
   ├─ 遍历场景节点树
   ├─ 筛选可见节点
   ├─ 收集渲染对象（Material + Mesh）
   ├─ 按材质批量提交到 Vulkan
   └─ 执行绘制命令
   ↓
3. 交换缓冲区，显示到屏幕
```

---

## 设计特点

### 优点
1. **模块化设计** - 各模块职责清晰，易于维护
2. **ECS 架构** - 灵活的组件系统，易于扩展
3. **事件驱动** - 模块间解耦，降低依赖
4. **多相机支持** - 可实现复杂的渲染效果（小地图、镜子反射等）
5. **异步资源加载** - 不阻塞主线程，流畅体验
6. **响应式 UI** - 自动适配不同屏幕尺寸
7. **高性能渲染** - 实例化绘制、批量渲染、Bindless 纹理

### 关键优化
- **批量渲染** - 合并相同材质的对象，减少绘制调用
- **实例化绘制** - 一次绘制多个相同对象
- **离屏渲染** - 支持后处理、中间纹理
- **推送常量** - 快速传递小量数据到着色器

---

## 项目结构

```
Boo-Engine/
├─ engine/                    # 引擎核心代码
│  ├─ engine.h               # 引擎入口
│  └─ core/
│     ├─ game.h              # 游戏核心
│     ├─ scene/              # 场景系统
│     ├─ component/          # 组件系统
│     ├─ renderer/           # 渲染器
│     ├─ gfx/                # Vulkan 图形层
│     ├─ input/              # 输入系统
│     ├─ assets/             # 资产系统
│     ├─ event/              # 事件系统
│     ├─ font/               # 字体系统
│     └─ math/               # 数学库
│
├─ editor/                    # 编辑器
│  └─ core/
│     ├─ main/               # 编辑器主界面
│     ├─ cache/              # 编辑器缓存
│     └─ config/             # 编辑器配置
│
├─ examples/                  # 示例项目
├─ assets/                    # 资源文件
└─ CMakeLists.txt            # 构建配置
```

---

## 快速开始

### 构建项目

```bash
# 克隆仓库
git clone <repository-url>
cd Boo-Engine

# 创建构建目录
mkdir build && cd build

# 配置 CMake
cmake ..

# 编译
cmake --build .

# 运行示例
./examples/example_project
```

### 系统要求

- **编译器**: 支持 C++17 的编译器（GCC 7+, Clang 5+, MSVC 2019+）
- **Vulkan SDK**: 1.2 或更高版本
- **CMake**: 3.15 或更高版本

---

## 相关文档

- **引擎架构详解**: 参见 `docs/architecture.md`（如果有）
- **API 参考**: 参见各模块头文件中的注释
- **示例项目**: 参见 `examples/` 目录

---

## 开发路线图

- [ ] 物理系统集成
- [ ] 音频系统
- [ ] 粒子系统
- [ ] 动画系统
- [ ] 网络系统
- [ ] 脚本系统（Lua/Python）
- [ ] 更多示例项目

---

## 许可证

（请根据实际情况填写许可证信息）

---

## 联系方式

- **作者**: 宗宝
- **项目地址**: （如有 GitHub/Gitee 仓库请填写）

---

**Boo-Engine** - 让游戏开发更简单！
