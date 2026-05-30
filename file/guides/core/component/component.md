# Component 组件系统设计文档

## 概述

Component（组件）是 Boo 引擎中用于扩展节点功能的可插拔模块。组件必须挂载到节点（Node）上才能工作，通过将不同的组件组合到节点上，可以实现各种功能。

### 核心职责

- **功能扩展**：为节点添加渲染、脚本、物理等功能
- **组件层级**：通过层级限制组件可添加的节点类型
- **生命周期管理**：提供 Awake、Enable、Update、LateUpdate、Disable 等生命周期回调

---

## 组件类型

引擎提供多种组件：

- **Layer2D 组件**：用于 Node2D 节点（如 UISprite、UIText）
- **Layer3D 组件**：用于 Node3D 节点（如 MeshRenderer）

---

## 核心概念

### 组件层级（EComponentLayer）

组件层级与节点层级对应，确保组件只能添加到对应类型的节点上：

```cpp
enum class EComponentLayer
{
    Default,   // 默认层级
    Layer2D,   // 2D 组件层
    Layer3D,   // 3D 组件层
};
```

### 组件层级与节点层级的对应关系

组件只能添加到对应层级的节点上：
- Layer2D 组件只能添加到 Node2D
- Layer3D 组件只能添加到 Node3D

如果在不匹配的节点上添加组件，操作将会失败并输出警告日志。

---

## Component - 组件基类

Component 是所有组件的基类，提供了组件的基本功能和生命周期管理。

### 头文件

```cpp
#include "core/component/component.h"
```

### API 接口

#### 基础信息

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getUuid()` | 获取组件 UUID | - | const string |
| `getName()` | 获取组件名称 | - | string |
| `getNode()` | 获取所属节点 | - | Node* |
| `getLayer()` | 获取组件层级 | - | EComponentLayer |

#### 激活控制

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `isEnabled()` | 获取自身启用状态 | - | bool |
| `setEnabled(enabled)` | 设置启用状态 | enabled: true/false | void |
| `isEnabledInHierarchy()` | 获取层级启用状态 | - | const bool |

#### 生命周期

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `OnAwake()` | 组件唤醒函数（纯虚函数） | - | void |
| `OnEnable()` | 组件启用函数（纯虚函数） | - | void |
| `Update(deltaTime)` | 组件更新函数（纯虚函数） | deltaTime: 时间增量 | void |
| `LateUpdate(deltaTime)` | 组件延迟更新函数（纯虚函数） | deltaTime: 时间增量 | void |
| `OnDisable()` | 组件禁用函数（纯虚函数） | - | void |
| `OnDestroy()` | 组件销毁回调函数（纯虚函数） | - | void |

#### 销毁

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁组件 | - | void |

#### 属性设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setProperty(data)` | 从 JSON 数据设置组件属性 | data: json 对象 | void |

---

## 自定义组件实现

用户可以通过继承 Component 基类来创建自定义组件。

### 实现步骤

1. **继承 Component 基类**
2. **实现纯虚函数**
3. **注册组件**

### 代码示例

#### 引擎内组件

适用于引擎内部的组件，与引擎共享同一命名空间：

```cpp
#pragma once
#include <string>
#include "core/component/component.h"
#include "core/component/component-register.h"

namespace Boo
{
    class MyComponent : public Component
    {
    public:
        // 构造函数
        MyComponent(std::string name, Node *node, std::string uuid = "");

        // 生命周期函数
        void OnAwake() override;
        void OnEnable() override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void OnDisable() override;
        void OnDestroy() override;

        // 可选：重写销毁函数
        void destroy() override;

        // 可选：重写属性设置函数
        void setProperty(json &data) override;

        // 析构函数
        ~MyComponent() override;
    };

    // 注册组件
    REGISTER_COMPONENT(MyComponent, "My Custom Component")
}
```

#### 引擎外组件（用户项目）

适用于引擎外部的用户项目，可使用自定义命名空间：

```cpp
#pragma once
#include <string>
#include "core/component/component.h"
#include "core/component/component-register.h"

namespace MyGame
{
    class Boo::Node;
    class Boo::Node3D;

    class PlayerController : public Boo::Component
    {
    public:
        // 构造函数
        PlayerController(std::string name, Boo::Node *node, std::string uuid = "");

        // 生命周期函数
        void OnAwake() override;
        void OnEnable() override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void OnDisable() override;
        void OnDestroy() override;

        // 可选：重写销毁函数
        void destroy() override;

        // 可选：重写属性设置函数
        void setProperty(json &data) override;

        // 析构函数
        ~PlayerController() override;
    };

    // 注册组件
    REGISTER_COMPONENT(PlayerController, "Player Controller Component")
}
```

### 头文件

```cpp
#include "core/component/component.h"
#include "core/component/component-register.h"
```

### 生命周期函数说明

| 函数 | 调用时机 | 说明 |
|------|----------|------|
| `OnAwake()` | 组件创建时调用 | 仅调用一次，用于初始化 |
| `OnEnable()` | 节点激活时调用 | 组件被启用时执行 |
| `Update(dt)` | 每帧调用 | 每帧更新逻辑 |
| `LateUpdate(dt)` | Update 后调用 | 延迟更新逻辑 |
| `OnDisable()` | 节点停用时调用 | 组件被禁用时执行 |
| `OnDestroy()` | 组件销毁前调用 | 组件销毁前的回调 |

### 注册宏说明

```cpp
REGISTER_COMPONENT(ComponentClass, Description)
```

- `ComponentClass`：组件类名
- `Description`：组件描述（字符串）

注册后，可以通过 `node->addComponent("ComponentClass")` 创建组件实例。
