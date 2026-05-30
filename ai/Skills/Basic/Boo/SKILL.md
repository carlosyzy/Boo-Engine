---
name: "boo"
description: "Boo引擎的主命名空间，提供对各个核心模块的访问。所有引擎功能都通过Boo命名空间统一访问。"
---

# Boo 命名空间

Boo 是引擎的主命名空间，提供对各个核心模块的统一访问。通过 Boo 命名空间，你可以方便地使用引擎的所有核心功能。

## 核心模块

### 全局实例

Boo 命名空间提供了以下全局实例，用于访问各个核心系统：

| 实例 | 类型 | 描述 |
|------|------|------|
| `Boo::game` | `Game*` | 游戏核心实例，负责游戏的初始化、更新、渲染和场景管理 |
| `Boo::event` | `Event*` | 事件系统，处理引擎和游戏事件的监听与触发 |
| `Boo::view` | `View*` | 视图系统，管理窗口尺寸和视口 |
| `Boo::assetManager` | `AssetsManager*` | 资产管理系统，负责加载和管理游戏资源 |

### 枚举类型

| 枚举 | 描述 |
|------|------|
| `Boo::UIDesignFitMode` | UI适配模式，包括 None（无适配）、Width（宽适配）、Height（高适配） |
| `Boo::ENodeGroup` | 节点组，用于确定节点的可见性，包括 Default、Node3D、Node2D |
| `Boo::ENodeTransformFlag` | 节点变换标志，包括 NONE_FLAG、POSITION_FLAG、ROTATION_FLAG、SCALE_FLAG、SIZE_FLAG、ANCHOR_FLAG、ALL_FLAG |
| `Boo::ENodeLayer` | 节点层，包括 Default、Node2D、Node3D、Scene |

### 核心类

Boo 命名空间包含以下核心类：

#### 场景管理
- `Boo::Scene` - 场景类，游戏世界的根容器
- `Boo::Node` - 基础节点类
- `Boo::Node2D` - 2D节点类，继承自Node
- `Boo::Node3D` - 3D节点类，独立类（暂未继承自Node）

#### 数学
- `Boo::Vec2` - 2D向量
- `Boo::Vec3` - 3D向量
- `Boo::Mat4` - 4x4矩阵
- `Boo::Quat` - 四元数
- `Boo::Color` - 颜色类
- `Boo::Size` - 尺寸类

#### 资产
- `Boo::Asset` - 基础资产类
- `Boo::TextureAsset` - 纹理资产
- `Boo::MaterialAsset` - 材质资产
- `Boo::ShaderAsset` - 着色器资产
- `Boo::SceneAsset` - 场景资产
- `Boo::MeshAsset` - 网格资产

#### 组件
- `Boo::Component` - 基础组件类
- `Boo::ComponentFactory` - 组件工厂

#### 渲染
- `Boo::Renderer` - 渲染器
- `Boo::Camera` - 相机类

#### 工具
- `Boo::FileUtil` - 文件工具
- `Boo::UUIDUtil` - UUID工具
- `Boo::TimeUtil` - 时间工具

## 引擎头文件

所有引擎功能通过统一头文件引入：

```cpp
#include "engine/boo.h"      // 包含所有核心功能
#include "engine/log.h"      // 日志系统（LOGI, LOGW, LOGE等）
```

## 命名空间结构

Boo 命名空间的结构如下：

```
Boo 命名空间
├── 全局实例
│   ├── Game* game              // 游戏核心实例
│   ├── Event* event            // 事件系统
│   ├── View* view              // 视图系统
│   ├── AssetsManager* assetManager  // 资产管理
│   └── Renderer* renderer      // 渲染系统
│
├── 场景管理
│   ├── Scene                   // 场景类
│   ├── Node                    // 基础节点类
│   ├── Node2D                  // 2D节点类
│   └── Node3D                  // 3D节点类
│
├── 数学库
│   ├── Vec2, Vec3              // 向量类
│   ├── Mat4                    // 矩阵类
│   ├── Quat                    // 四元数类
│   ├── Color                   // 颜色类
│   └── Size                    // 尺寸类
│
├── 资产系统
│   ├── Asset                   // 基础资产类
│   ├── TextureAsset            // 纹理资产
│   ├── MaterialAsset           // 材质资产
│   ├── ShaderAsset             // 着色器资产
│   ├── MeshAsset               // 网格资产
│   └── SceneAsset              // 场景资产
│
├── 组件系统
│   ├── Component               // 基础组件类
│   └── ComponentFactory        // 组件工厂
│