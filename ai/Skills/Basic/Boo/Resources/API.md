# Boo 命名空间 API

## 全局实例

| 实例 | 类型 | 描述 |
|------|------|------|
| `Boo::game` | `Game*` | 游戏核心实例 |
| `Boo::event` | `Event*` | 事件系统 |
| `Boo::view` | `View*` | 视图系统 |
| `Boo::assetManager` | `AssetsManager*` | 资产管理系统 |

## 内置资产常量

| 常量 | 描述 |
|------|------|
| `Boo::AssetBuiltinTexture::Default` | 默认白色纹理，可直接传给 `setTexture` |
| `Boo::AssetBuiltinMaterial::UI` | 内置 UI 材质，可直接传给 `setMaterial` |

## 内置事件名称

| 常量 | 值 | 描述 |
|------|----|------|
| `Boo::EventEngine_Launch` | `"Launch"` | 引擎启动完成事件 |

## 枚举类型

### UIDesignFitMode

| 值 | 描述 |
|----|------|
| `None` | 不适配 |
| `Width` | 按宽度适配 |
| `Height` | 按高度适配 |

### ENodeLayer

| 值 | 描述 |
|----|------|
| `Default` | 默认层 |
| `Node2D` | 2D 节点层 |
| `Node3D` | 3D 节点层 |
| `Scene` | 场景层 |

### ENodeGroup

| 值 | 描述 |
|----|------|
| `Default` | 默认组 |
| `Node3D` | 3D 节点组 |
| `Node2D` | 2D 节点组 |

### ENodeTransformFlag

| 值 | 描述 |
|----|------|
| `NONE_FLAG` | 无变换 |
| `POSITION_FLAG` | 位置变换 |
| `ROTATION_FLAG` | 旋转变换 |
| `SCALE_FLAG` | 缩放变换 |
| `SIZE_FLAG` | 大小变换 |
| `ANCHOR_FLAG` | 锚点变换 |
| `ALL_FLAG` | 所有变换 |

### EComponentLayer

| 值 | 描述 |
|----|------|
| `Default` | 默认层 |
| `Node2D` | 2D 组件层 |
| `Node3D` | 3D 组件层 |

## 引擎头文件

```cpp
#include "engine/boo.h"   // 包含所有核心功能
#include "engine/log.h"   // 日志系统（LOGI, LOGW, LOGE 等）
```
