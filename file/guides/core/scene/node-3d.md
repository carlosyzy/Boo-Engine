# Node3D - 3D 节点

Node3D 用于 3D 场景渲染，支持位置、缩放、旋转等变换操作。

### 头文件

```cpp
#include "core/scene/node-3d.h"
```

### API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `Node3D(name, uuid = "")` | 创建 3D 节点，name 为节点名称，uuid 可选 |

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

#### 3D 特有属性

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getWorldMatrixIT()` | 获取逆转置矩阵 | - | Mat4& |

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
| `getMeshRenderer()` | 获取网格渲染组件 | - | MeshRenderer* |

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

### 创建 3D 节点

```cpp
// ── 创建开始场景 ────────────────────────────────────────
auto *scene = new Boo::Scene("StartScene");
Boo::game->openScene(scene);
auto *root3D = scene->getRoot3D();

// ── 节点 ──────────────────────────────────────────
Boo::Node3D* node3D = new Boo::Node3D("Cylinder");
root3D->addChild(node3D);
```
