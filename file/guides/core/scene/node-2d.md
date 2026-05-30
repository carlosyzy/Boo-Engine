# Node2D - 2D 节点

Node2D 用于 2D UI 渲染，支持位置、缩放、旋转、锚点等变换操作。

### 头文件

```cpp
#include "core/scene/node-2d.h"
```

### API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `Node2D(name, uuid = "")` | 创建 2D 节点，name 为节点名称，uuid 可选 |

#### 变换控制

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setPosition(x, y, z)` | 设置本地位置 | x, y, z: 坐标值 | void |
| `setWorldPosition(x, y, z)` | 设置世界位置 | x, y, z: 坐标值 | void |
| `getPosition()` | 获取本地位置 | - | Vec3& |
| `getWorldPosition()` | 获取世界位置 | - | Vec3& |
| `setScale(x, y, z)` | 设置缩放 | x, y, z: 缩放比例 | void |
| `setWorldScale(x, y, z)` | 设置世界缩放 | x, y, z: 缩放比例 | void |
| `getScale()` | 获取缩放 | - | Vec3& |
| `getWorldScale()` | 获取世界缩放 | - | Vec3& |
| `setEulerAngles(x, y, z)` | 设置欧拉角（度） | x, y, z: 旋转角度 | void |
| `setRotation(x, y, z, w)` | 设置四元数旋转 | x, y, z, w: 四元数分量 | void |
| `setWorldRotation(x, y, z, w)` | 设置世界四元数旋转 | x, y, z, w: 四元数分量 | void |
| `getRotation()` | 获取旋转四元数 | - | Quat& |
| `getWorldRotation()` | 获取世界旋转四元数 | - | Quat& |
| `getEulerAngles()` | 获取欧拉角 | - | Vec3& |
| `setAngle(angle)` | 设置 Z 轴旋转角度（度） | angle: 旋转角度 | void |

#### 2D 特有属性

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setAnchor(x, y)` | 设置锚点 | x, y: 锚点坐标 (0-1) | void |
| `getAnchor()` | 获取锚点 | - | Vec2& |
| `setSize(width, height)` | 设置尺寸 | width, height: 宽高像素值 | void |
| `getSize()` | 获取尺寸 | - | Size& |
| `getUIWorldMatrix()` | 获取 UI 世界矩阵 | - | Mat4& |

#### 层级管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `addChild(node)` | 添加子节点 | node: Node2D/Node3D 指针 | void |
| `removeChild(node)` | 移除子节点 | node: Node 指针 | void |
| `removeFromParent()` | 从父节点移除 | - | void |
| `getChildren()` | 获取子节点列表 | - | vector<Node*>& |
| `getChildByName(name)` | 按名称查找子节点 | name: 节点名称 | Node* |
| `setParent(parent)` | 设置父节点 | parent: Node 指针 | void |
| `getParent()` | 获取父节点 | - | Node* |

#### 组件管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `addComponent(name, uuid = "")` | 添加组件 | name: 组件名称，uuid: 可选 | Component* |
| `getComponent(name)` | 获取组件 | name: 组件名称 | Component* |
| `getComponents()` | 获取所有组件 | - | vector<Component*> |
| `getUIRenderer()` | 获取 UI 渲染组件 | - | UIRenderer* |

#### 激活状态

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setActive(active)` | 设置激活状态 | active: true/false | void |
| `getActive()` | 获取自身激活状态 | - | bool |
| `getActiveInHierarchy()` | 获取层级激活状态 | - | bool |

#### 节点信息

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getName()` | 获取节点名称 | - | string |
| `setName(name)` | 设置节点名称 | name: 节点名称 | void |
| `getUuid()` | 获取节点 UUID | - | string |
| `setUuid(uuid)` | 设置节点 UUID | uuid: UUID 字符串 | void |
| `getLayer()` | 获取节点层级 | - | ENodeLayer |
| `setGroupId(groupId)` | 设置节点组 ID | groupId: 组 ID | void |
| `getGroupId()` | 获取节点组 ID | - | int |

---

## 使用示例

### 创建 start-ui 节点

```cpp
// ── 创建开始场景 ───────────────────────────────────────────
auto *scene = new Boo::Scene("StartScene");
Boo::game->openScene(scene);
auto *root2D = scene->getRoot2D();

// ── start-ui 节点 ──────────────────────────────────────────
auto *startUINode = new Boo::Node2D("start-ui");
startUINode->setPosition(0.0f, 0.0f, 0.0f);
startUINode->setSize(720.0f, 1280.0f);
startUINode->setAnchor(0.5f, 0.5f);
root2D->addChild(startUINode);
```
