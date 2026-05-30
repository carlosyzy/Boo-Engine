# 场景系统 API 文档

## 场景类 (Scene)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `Scene(const std::string name, const std::string uuid = "")` | `name`: 场景名称<br>`uuid`: 场景唯一标识符 | - | 创建场景实例 |

### 根节点管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getRoot3D()` | 无 | `Node3D*` | 获取 3D 根节点 |
| `getRoot2D()` | 无 | `Node2D*` | 获取 2D 根节点 |

### 场景控制

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `launch()` | 无 | `void` | 启动场景 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新场景 |
| `lateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新场景 |
| `clearNodeFrameFlag()` | 无 | `void` | 清除节点帧变换标志 |
| `destroy()` | 无 | `void` | 销毁场景 |

### 组件管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addComponent(std::string name, std::string uuid = "")` | `name`: 组件名称<br>`uuid`: 组件唯一标识符 | `Component*` | 添加组件到场景 |

---

## 节点基类 (Node)

### 基本属性

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getLayer()` | 无 | `ENodeLayer` | 获取节点类型 |
| `getUuid()` | 无 | `const std::string&` | 获取节点唯一标识符 |
| `setUuid(const std::string &uuid)` | `uuid`: 节点唯一标识符 | `void` | 设置节点唯一标识符 |
| `setName(const std::string &name)` | `name`: 节点名称 | `void` | 设置节点名称 |
| `getName()` | 无 | `const std::string&` | 获取节点名称 |
| `getGroupId()` | 无 | `int` | 获取节点组 ID |
| `setGroupId(int groupID)` | `groupID`: 节点组 ID | `void` | 设置节点组 ID |

### 变换操作

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setPosition(float x, float y, float z)` | `x`, `y`, `z`: 位置坐标 | `void` | 设置节点本地位置 |
| `getPosition()` | 无 | `const Vec3&` | 获取节点本地位置 |
| `setWorldPosition(float x, float y, float z)` | `x`, `y`, `z`: 世界位置坐标 | `void` | 设置节点世界位置 |
| `getWorldPosition()` | 无 | `const Vec3&` | 获取节点世界位置 |
| `setRotation(float x, float y, float z, float w)` | `x`, `y`, `z`, `w`: 四元数 | `void` | 设置节点本地旋转 |
| `getRotation()` | 无 | `const Quat&` | 获取节点本地旋转 |
| `setWorldRotation(float x, float y, float z, float w)` | `x`, `y`, `z`, `w`: 世界四元数 | `void` | 设置节点世界旋转 |
| `getWorldRotation()` | 无 | `const Quat&` | 获取节点世界旋转 |
| `setScale(float x, float y, float z)` | `x`, `y`, `z`: 缩放因子 | `void` | 设置节点本地缩放 |
| `getScale()` | 无 | `const Vec3&` | 获取节点本地缩放 |
| `setWorldScale(float x, float y, float z)` | `x`, `y`, `z`: 世界缩放因子 | `void` | 设置节点世界缩放 |
| `getWorldScale()` | 无 | `const Vec3&` | 获取节点世界缩放 |
| `setEulerAngles(float x, float y, float z)` | `x`, `y`, `z`: 欧拉角 | `void` | 设置节点欧拉角 |
| `getEulerAngles()` | 无 | `const Vec3&` | 获取节点欧拉角 |
| `setMatrix(const Mat4 &matrix)` | `matrix`: 本地矩阵 | `void` | 通过矩阵设置节点变换 |
| `getLocalMatrix()` | 无 | `const Mat4&` | 获取节点本地矩阵 |
| `getWorldMatrix()` | 无 | `const Mat4&` | 获取节点世界矩阵 |

### 变换状态

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `hasFrameTransformFlag()` | 无 | `bool` | 检查节点当前帧内是否发生过变换 |
| `clearNodeFrameFlag()` | 无 | `void` | 清除节点帧变换标志 |

### 节点层次结构

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addChild(Node *node)` | `node`: 子节点 | `void` | 添加子节点 |
| `removeChild(Node *node)` | `node`: 子节点 | `void` | 移除子节点 |
| `removeFromParent()` | 无 | `void` | 从父节点中移除自身 |
| `getChildren()` | 无 | `std::vector<Node*>&` | 获取子节点列表 |
| `getChildByName(std::string name)` | `name`: 子节点名称 | `Node*` | 根据名称获取子节点 |
| `setParent(Node *parent)` | `parent`: 父节点 | `void` | 设置父节点 |
| `getParent()` | 无 | `Node*` | 获取父节点 |
| `destroyAllChildren()` | 无 | `void` | 销毁所有子节点 |

### 激活状态

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setActive(bool active)` | `active`: 是否激活 | `void` | 设置节点激活状态 |
| `getActive()` | 无 | `bool` | 获取节点自身激活状态 |
| `getActiveInHierarchy()` | 无 | `bool` | 获取节点在层级中的实际激活状态 |

### 组件管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addComponent(std::string name, std::string uuid = "")` | `name`: 组件名称<br>`uuid`: 组件唯一标识符 | `Component*` | 添加组件到节点 |
| `getComponent(const std::string &name)` | `name`: 组件名称 | `Component*` | 获取节点上的组件 |
| `getComponents()` | 无 | `std::vector<Component*>` | 获取节点上的所有组件 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新节点 |
| `lateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新节点 |
| `destroy()` | 无 | `void` | 销毁节点 |

---

## 2D 节点类 (Node2D)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `Node2D(const std::string name, const std::string uuid = "")` | `name`: 节点名称<br>`uuid`: 节点唯一标识符 | - | 创建 2D 节点实例 |

### 2D 特有属性

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setAnchor(float x, float y)` | `x`, `y`: 锚点坐标 | `void` | 设置节点锚点 |
| `getAnchor()` | 无 | `const Vec2&` | 获取节点锚点 |
| `setSize(float width, float height)` | `width`, `height`: 节点大小 | `void` | 设置节点大小 |
| `getSize()` | 无 | `const Size&` | 获取节点大小 |
| `setAngle(float angle)` | `angle`: 旋转角度（度） | `void` | 设置节点旋转角度 |
| `getUIWorldMatrix()` | 无 | `const Mat4&` | 获取 UI 世界矩阵 |

### SizeLock

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addSizeLock(ENode2DSizeLock sizeLock)` | `sizeLock`: 尺寸锁类型 | `void` | 添加尺寸锁 |
| `removeSizeLock(ENode2DSizeLock sizeLock)` | `sizeLock`: 尺寸锁类型 | `void` | 移除尺寸锁 |
| `setSizeLock(ENode2DSizeLock sizeLock)` | `sizeLock`: 尺寸锁类型 | `void` | 设置尺寸锁 |
| `getSizeLock()` | 无 | `int` | 获取当前尺寸锁标志 |

### 碰撞检测

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `inHitMask(float x, float y)` | `x`, `y`: 碰撞点坐标 | `bool` | 检查点是否在碰撞掩码内 |
| `inHitOnNode(float x, float y)` | `x`, `y`: 碰撞点坐标 | `bool` | 检查点是否在节点矩形范围内 |

### 组件管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addComponent(std::string name, std::string uuid = "")` | `name`: 组件名称<br>`uuid`: 组件唯一标识符 | `Component*` | 添加组件到节点 |
| `getUIRenderer()` | 无 | `UIRenderer*` | 获取 UI 渲染器组件 |

---

## 3D 节点类 (Node3D)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `Node3D(const std::string name, const std::string uuid = "")` | `name`: 节点名称<br>`uuid`: 节点唯一标识符 | - | 创建 3D 节点实例 |

### 3D 特有属性

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getWorldMatrixIT()` | 无 | `const Mat4&` | 获取世界矩阵的逆转置矩阵（用于法线变换） |
| `getMeshRenderer()` | 无 | `MeshRenderer*` | 获取网格渲染器组件 |

### 组件管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `addComponent(std::string name, std::string uuid = "")` | `name`: 组件名称<br>`uuid`: 组件唯一标识符 | `Component*` | 添加组件到节点 |

---

## 枚举类型

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
| `Default` | 默认组（1 << 0） |
| `Node3D` | 3D 节点组（1 << 1） |
| `Node2D` | 2D 节点组（1 << 2） |

### ENode2DSizeLock

| 值 | 描述 |
|----|------|
| `None` | 无锁 |
| `SpriteRaw` | 锁定为精灵原始尺寸 |
