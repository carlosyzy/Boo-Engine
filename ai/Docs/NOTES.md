# Boo Engine 使用注意事项(AI 笔记)

## 重要纠正性问题

### 1. 场景根节点创建
- **问题**：不要手动调用 `createRoot2D()` 和 `createRoot3D()` 接口
- **原因**：引擎内部已经自动处理根节点的创建
- **正确做法**：直接使用 `scene->getRoot2D()` 或 `scene->getRoot3D()` 获取根节点

### 2. 场景打开顺序
- **问题**：不要在设置完所有场景内容后才调用 `openScene()`
- **原因**：场景需要在创建后立即打开，以确保内部初始化正确
- **正确做法**：在 `new Scene()` 后立即调用 `Boo::game->openScene(scene)`

### 3. 2D 渲染要求
- **问题**：2D 场景中没有相机组件会导致图片无法渲染
- **原因**：渲染系统需要相机来确定视图和投影
- **正确做法**：在 `root2D` 下创建一个带有 `Camera` 组件的节点

### 4. 2D 节点尺寸设置
- **问题**：使用 `setScale()` 设置 2D 节点的尺寸
- **原因**：2D 节点的尺寸应该通过宽高设置，而不是缩放
- **正确做法**：使用 `setSize(width, height)` 方法设置 2D 节点的尺寸

### 5. 屏幕中心坐标
- **问题**：使用 `width/2, height/2` 计算屏幕中心
- **原因**：游戏引擎的锚点 (0, 0) 就是屏幕中心
- **正确做法**：直接使用 `(0, 0, 0)` 表示屏幕中心

### 6. UISprite 图片设置
- **问题**：使用 `setProperty("image", path)` 设置图片
- **原因**：UISprite 组件应该使用 `setTexture()` 方法设置图片
- **正确做法**：使用 `sprite->setTexture("path/to/image.png")` 设置图片路径
- **路径说明**：
  - `setTexture()`：图片路径是相对于 assets 目录的相对路径，**不需要包含assets**
  - `AssetsManager::loadTexture()`：需要包含 assets
- **示例**：
  - setTexture 正确：`textures/player.png`
  - setTexture 错误：`assets/textures/player.png`
  - AssetsManager：`assets/textures/player.png`
- **默认图片**：使用 `Boo::AssetBuiltinTexture::Default` 作为默认贴图
- **重要**：使用图片前，必须先调用 `Boo::assetManager->loadAsset()` 加载图片，否则获取不到
- **加载方式**：`loadAsset()` 会根据文件后缀自动确定加载方式，比使用特定的 `loadTexture()` 更通用

### 7. 全屏设置
- **问题**：使用 `view->getWidth()` 和 `view->getHeight()` 获取窗口尺寸
- **原因**：全屏应该使用设计分辨率，view下的宽高是实际窗口尺寸
- **正确做法**：使用 `Boo::view->getDesignWidth()` 和 `Boo::view->getDesignHeight()` 获取设计分辨率
- **示例代码**：
  ```cpp
  int designWidth = Boo::view->getDesignWidth();
  int designHeight = Boo::view->getDesignHeight();
  node->setSize(designWidth, designHeight);
  ```

### 8. 组件类型转换
- **问题**：直接使用 `addComponent()` 的返回值调用组件特定方法
- **原因**：`addComponent()` 返回的是通用组件类型，需要转换为具体类型
- **正确做法**：使用 `dynamic_cast` 将组件转换为具体类型后再调用方法
- **示例代码**：
  ```cpp
  auto spriteComponent = node->addComponent("UISprite");
  auto sprite = dynamic_cast<Boo::UISprite*>(spriteComponent);
  if (sprite) {
      sprite->setTexture("claw.png");
  }
  ```

### 8. 场景和节点使用
- **问题**：继承 `Scene` 和 `Node` 类
- **原因**：没有特殊需求时，不需要继承这些类，直接使用实例即可
- **正确做法**：使用 `new Scene()` 创建场景实例，使用 `new Node2D()` 或 `new Node3D()` 创建节点实例
- **示例代码**：
  ```cpp
  // 创建场景
  auto scene = new Boo::Scene("SceneName");
  Boo::game->openScene(scene);
  
  // 创建节点
  auto node = new Boo::Node2D("NodeName");
  ```

### 9. 头文件设计
- **问题**：在头文件中使用 `void*` 类型来声明指针
- **原因**：`void*` 类型不安全，缺乏类型检查，容易导致运行时错误
- **正确做法**：使用向前声明来减少依赖，提高类型安全性
- **示例代码**：
  ```cpp
  // 推荐的头文件设计
  namespace Boo {
      class Scene;
      class Node2D;
  }
  
  class PixelLobster {
  private:
      Boo::Scene* m_scene;
      
  public:
      PixelLobster();
      ~PixelLobster();
      
      void createScene();
  };
  ```

### 10. 自定义组件注册
- **问题**：自定义组件无法被 `addComponent()` 识别
- **原因**：没有使用 `REGISTER_COMPONENT` 宏进行注册
- **正确做法**：引入头文件并使用 `REGISTER_COMPONENT` 宏注册组件
- **必须引入的头文件**：
  ```cpp
  #include "engine/core/component/component-register.h"
  ```
- **示例代码**：
  ```cpp
  #pragma once
  #include "engine/core/component/component.h"
  #include "engine/core/component/component-register.h"
  
  class GobangGame : public Boo::Component
  {
  public:
      GobangGame(std::string name, Boo::Node *node, std::string uuid = "");
      void   OnAwake() override;
      void   OnEnable () override;
      void Update(float deltaTime) override;
      void LateUpdate(float deltaTime) override;
      void   OnDisable() override;
      void destroy() override;
      ~GobangGame() override;
  };
  
  REGISTER_COMPONENT(GobangGame, "Gobang Game Manager Component")
  ```

### 11. 组件内节点操作
- **问题**：在组件中使用 `root2D` 或 `getScene()->getRoot2D()` 获取根节点
- **原因**：组件应该操作自己的节点，而不是操作根节点
- **正确做法**：使用 `getNode()` 获取组件所在的节点，所有子节点都添加到该节点下
- **示例代码**：
  ```cpp
  void MyComponent::  OnAwake() {
      auto node = getNode();
      
      auto child = new Boo::Node2D("Child");
      node->addChild(child);
      child->setPosition(100, 100, 0);
  }
  ```

## 其他注意事项

### 4. 组件添加
- 为节点添加组件时，使用 `node->addComponent("ComponentName")`
- 常用组件包括：`UISprite`、`Camera`、`UIText` 等

### 5. 坐标系统
- 2D 节点的位置基于屏幕坐标系
- **重要**：游戏引擎的锚点 (0, 0) 就是屏幕中心，不需要计算宽高的一半
- 坐标示例：(0, 0, 0) 表示屏幕中心

### 6. 资源管理
- 图片资源需要通过 `AssetsManager` 加载
- 使用 `UISprite` 组件时，可以设置图片路径

### 7. 事件系统
- 使用 `Boo::event->on()` 注册事件回调
- 常用事件：`Boo::EventEngine_Launch`、`Boo::EventEngine_Tick` 等

### 8. 生命周期管理
- 游戏对象的创建和销毁需要正确管理
- 使用智能指针或在适当时候手动清理资源

## 示例代码结构

```cpp
// 正确的场景创建和初始化流程
Scene *scene = new Scene("SceneName");
Boo::game->openScene(scene);

// 获取根节点
auto root2D = scene->getRoot2D();

// 创建相机
auto cameraNode = new Node2D("Camera");
root2D->addChild(cameraNode);
cameraNode->addComponent("Camera");

// 创建精灵
auto sprite = new Node2D("Sprite");
root2D->addChild(sprite);
sprite->addComponent("UISprite");

// 设置精灵大小（使用setSize而不是setScale）
sprite->setSize(100, 100);

// 设置精灵位置到屏幕中心（锚点(0,0)就是屏幕中心）
sprite->setPosition(0, 0, 0);
```

---

## 自定义组件写法

自定义组件继承 `Boo::Component`，在头文件末尾用 `REGISTER_COMPONENT` 宏注册。

```cpp
// foo.h
#pragma once
#include "core/component/component.h"
#include "core/component/component-register.h"

namespace Boo { class Node; class Node2D; }

class Foo : public Boo::Component
{
public:
    // 构造函数必须与基类签名完全一致，透传所有参数
    Foo(std::string name, Boo::Node *node, std::string uuid = "")
        : Boo::Component(name, node, uuid) {}

    void   OnAwake() override;          // 节点加入场景后调用，在此初始化子节点
    void   OnEnable () override {}       // 必须显式声明（可为空实现）
    void Update(float dt) override {} // 必须显式声明（可为空实现）
    void LateUpdate(float dt) override {} // 必须显式声明（可为空实现）
    void   OnDisable() override {}       // 必须显式声明（可为空实现）
};

REGISTER_COMPONENT(Foo, "Foo")
```

```cpp
// foo.cpp
#include "foo.h"
#include "core/scene/node-2d.h"

void Foo::OnAwake()
{
    auto *self = dynamic_cast<Boo::Node2D *>(_node);
    // 在此创建子节点、添加子组件
}
```

**挂载方式：**
```cpp
node->addComponent("Foo"); // 字符串必须与 REGISTER_COMPONENT 第二参数一致
```

**生命周期对应关系：**
| 生命周期函数 | 对应关系 | 说明 |
|-------------|---------|------|
| `OnAwake()` | ↔ `OnDestroy()` | 节点加入场景时调用 / 节点销毁时调用 |
| `OnEnable()` | ↔ `OnDisable()` | 节点激活时调用 / 节点禁用时调用 |

> **注意**：资源创建放在 `OnAwake()`，资源销毁放在 `OnDestroy()`；事件注册放在 `OnEnable()`，事件注销放在 `OnDisable()`。

---

## 引擎关键 API 速查

### 全局单例

```cpp
Boo::game          // 游戏系统（场景、帧率、FPS）
Boo::event         // 事件系统
Boo::assetManager  // 资产管理（只读，勿调用 setAssetsRoot）
Boo::input         // 输入系统
Boo::profiler      // 性能分析
```

### 常用节点操作

```cpp
auto *node = new Boo::Node2D("NodeName");
node->setPosition(x, y, 0.0f);
node->setSize(w, h);
node->setAnchor(0.5f, 0.5f); // 0~1
parent->addChild(node);

// 添加内置组件
auto *sprite = dynamic_cast<Boo::UISprite *>(node->addComponent("UISprite"));
sprite->setTexture("textures/xxx.png"); // 相对 assetsRoot
sprite->setColor(1.0f, 1.0f, 1.0f, 1.0f);

auto *cam = dynamic_cast<Boo::Camera *>(camNode->addComponent("Camera"));
cam->setIsOnScreen(true);
cam->setPriority(0);
```

### UIBMFontText 组件

```cpp
auto *text = dynamic_cast<Boo::UIBMFontText *>(node->addComponent("UIBMFontText"));
if (text)
{
    text->setBMFont("fonts/num4/num4.fnt");  // 注意是 setBMFont，不是 setFont
    text->setText(L"消除结束");  // 注意是宽字符 L""
    text->setLineHeight(42.0f);  // 可选：设置行高
}
```

### 节点显示/隐藏

```cpp
// 使用 setActive(bool) 控制节点显示隐藏，不是 setVisible
node->setActive(true);   // 显示
node->setActive(false);  // 隐藏
```

### Vec3 向量类

```cpp
// 获取位置
Vec3 pos = node->getPosition();
float x = pos.getX();
float y = pos.getY();
float z = pos.getZ();

// 设置位置
node->setPosition(x, y, z);
```

### 节点旋转设置

```cpp
// 设置四元数旋转（用于复杂旋转或插值）
node->setRotation(x, y, z, w);

// 设置欧拉角旋转（用于简单角度设置）
node->setEulerAngles(pitch, yaw, roll); // 单位：度
```

> **注意**：使用 `setEulerAngles` 传入的是角度值，而非弧度。

### 事件系统（节点触摸）

```cpp
// 注册触摸事件
uint64_t touchID = Boo::input->onNodeTouchStart(node, [this](Boo::FInputNodeEvent &event)
{
    // 处理触摸事件
}, false);

// 注销触摸事件（必须传入节点和ID）
Boo::input->offNodeTouch(node, touchID);
```

> **注意**：不能使用 `Boo::input->off(id)` 来注销节点触摸事件，必须使用 `offNodeTouch(node, id)`

### NodeInputEventIntercept 组件

引擎内部 UI 事件拦截组件，用于拦截弹窗背景的触摸事件，防止事件穿透到下层节点。

**使用方式：**
```cpp
// 在弹窗背景节点创建后添加
_bgNode->addComponent("NodeInputEventIntercept");
```

**头文件引用：**
```cpp
#include "core/components/ui/node-input-event-intercept.h"
```

**功能说明：**
- 在 `OnEnable()` 中监听节点的触摸开始、移动、结束事件
- 设置 `isIntercept = false`（不可穿透），防止事件传递到下层节点
- 在 `OnDisable()` 和 `OnDestroy()` 中注销事件监听器
- 构造函数中设置组件层级为 `Layer2D`

**已应用场景：**
- `popup-settlement.cpp` - 结算弹窗背景
- `popup-exit.cpp` - 退出确认弹窗背景
- `popup-tips.cpp` - 温馨提示弹窗背景

### 场景生命周期

```cpp
auto *scene = new Boo::Scene("SceneName");
Boo::game->openScene(scene);
auto *root2D = scene->getRoot2D(); // 引擎自动创建，直接使用
// 不要手动调用 createRoot2D() / launch() / update()
```

**获取当前场景：**
```cpp
auto *scene = Boo::game->getScene();  // 正确
// auto *scene = Boo::game->getCurrentScene();  // 错误：不存在此方法
auto *root2D = scene->getRoot2D();
```

**所需头文件：**
```cpp
#include "core/scene/scene.h"    // Boo::Scene
#include "core/scene/node-2d.h"  // Boo::Node2D
#include "core/renderer/camera.h" // Boo::Camera, Boo::ECameraProjection, Boo::ENodeGroup
```

### 事件系统

```cpp
uint64_t id = Boo::event->on("EventName", []() { ... });
Boo::event->off(id);
Boo::event->emit("EventName");
```

---

## 禁止事项

- **禁止调用 `Boo::assetManager->setAssetsRoot(...)`**：AssetsRoot 由引擎内部根据平台自动识别，手动设置会导致资源加载异常。纹理路径直接传相对路径即可，如 `setTexture("textures/bg.jpg")`

---

## 节点坐标系

- **设计分辨率**：720（宽）× 1280（高），原点在左上角，+x 向右，+y 向下
- **setPosition(x, y, z)**：相对父节点原点的本地坐标
- **setAnchor(ax, ay)**：自身锚点（0~1），影响 position 对齐基准
  - `(0.5, 0.5)` = 以自身中心定位
  - `(0.0, 0.0)` = 以自身左上角定位

---

## Node2D 节点默认属性

除非特殊要求，所有节点创建时应使用以下默认属性：

| 属性 | 默认值 | 说明 |
|------|--------|------|
| `setPosition(x, y, 0.0f)` | `(0, 0, 0)` | 默认位置原点 |
| `setAnchor(ax, ay)` | `(0.5, 0.5)` | 默认锚点为中心 |
| `setScale(sx, sy, sz)` | `(1.0, 1.0, 1.0)` | 默认不缩放 |
| `setSize(w, h)` | **必须显式设置** | 节点尺寸需根据实际需求显式设置 |

**注意**：
- 节点尺寸（Size）没有默认值，创建节点时必须显式设置
- 位置、缩放、锚点如无特殊需求可使用默认值
- **没有特殊说明时，统一使用锚点 (0.5, 0.5) 和缩放 (1.0, 1.0, 1.0)**

---

## 功能模块规范

1. **目录按功能划分**：同一功能的文件放在同一子目录
2. **头文件分离**：`.h` 只声明接口，`.cpp` 实现
3. **自定义组件优先**：有节点生命周期需求的模块应写成 `Boo::Component` 子类，在 `OnAwake()` 中初始化
4. **普通类用于无生命周期的数据/渲染单元**：如需要管理引擎节点但无生命周期需求的类，用 `init()` 方法代替构造函数初始化引擎节点
5. **资源路径**：纹理统一放 `assets/textures/`，`setTexture` 传相对 assetsRoot 的路径
6. **组件注册字符串**：`addComponent` 传入的字符串必须与 `REGISTER_COMPONENT` 第二参数完全一致
   - `"UISprite"` — UISprite
   - `"UIText"` — UIText
   - `"UIBMFontText"` — UIBMFontText
   - `"Camera"` — Camera
7. **CMake**：`GLOB_RECURSE src/**` 已配置，新文件无需手动注册

### `.h` 头文件 include 规范

**不得在 `.h` 中直接 `#include "boo.h"`，只做前向声明，把完整 include 推迟到 `.cpp`。**

| 需求 | `.h` 写法 | `.cpp` 写法 |
|------|-----------|-------------|
| 继承 `Boo::Component` | `#include "core/component/component.h"` | — |
| 使用 `REGISTER_COMPONENT` 宏 | `#include "core/component/component-register.h"` | — |
| 构造参数 / 成员用到 `Boo::Node` | `namespace Boo { class Node; }` | `#include "core/scene/node-2d.h"` |
| 构造参数 / 成员用到 `Boo::Node2D` | `namespace Boo { class Node2D; }` | `#include "core/scene/node-2d.h"` |
| 成员用到 `Boo::UISprite` | `namespace Boo { class UISprite; }` | `#include "core/renderer/ui/ui-sprite.h"` |
| 成员用到 `Boo::UIBMFontText` | `namespace Boo { class UIBMFontText; }` | `#include "core/renderer/ui/ui-bmfont-text.h"` |
| 需要场景、全局单例等 | — | `#include "boo.h"` |
| 使用 `Boo::Scene` | — | `#include "core/scene/scene.h"` |
| 使用 `Boo::Camera` / `ECameraProjection` / `ENodeGroup` | — | `#include "core/renderer/camera.h"` |

**原则：`.h` 中只 include 必须在编译期可见的内容（基类、宏），其余用前向声明；`.cpp` 中 include 实现所需的完整头文件。**

---

## 启动流程

```
app.cpp
  └── Launch::init()
        ├── event->on(EventEngine_Launch, onLaunch)
        ├── profiler->openDebug()
        └── game->setFrameRate(120)

onLaunch()
  ├── assetManager->setAssetsRoot("assets/")
  └── GameEntry::init()
        ├── new Scene("GameScene") → game->openScene()
        ├── root2D = scene->getRoot2D()
        ├── Camera2D 节点 + "Camera Component"
        │     addGroupID(ENodeGroup::Node2D)
        │     setProjection(ECameraProjection::Ortho)
        │     setPriority(100)
        └── 业务相关节点创建
```

### 相机规范

场景打开后第一件事必须创建相机，否则画面不可见。

**2D 相机写法：**
```cpp
auto *cameraNode = new Boo::Node2D("Camera2D");
cameraNode->setPosition(0.0f, 0.0f, 0.0f);
root2D->addChild(cameraNode);
auto *camera = dynamic_cast<Boo::Camera *>(cameraNode->addComponent("Camera"));
if (camera)
{
    camera->addGroupID(Boo::ENodeGroup::Node2D);
    camera->setProjection(Boo::ECameraProjection::Ortho);
    camera->setPriority(100);
}
```

注意事项：
- 组件注册名是 `"Camera"`
- 枚举是 `ENodeGroup`（非 `NodeGroup`）、`ECameraProjection`（非 `CameraProjection`）
- 头文件：`#include "core/renderer/camera.h"`（包含 Camera、ECameraProjection、ENodeGroup）
