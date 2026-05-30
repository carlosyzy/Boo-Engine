# 组件系统 API 文档

## 组件基类 (Component)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `Component(std::string name, Node *node, std::string uuid = "")` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | - | 创建组件实例 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `  OnAwake()` | 无 | `void` | 组件初始化时调用 |
| `  OnEnable ()` | 无 | `void` | 组件启用时调用 |
| `Update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新时调用 |
| `LateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新时调用 |
| `  OnDisable()` | 无 | `void` | 组件禁用时调用 |
| `destroy()` | 无 | `void` | 组件销毁时调用 |

### 状态管理

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `isEnabled()` | 无 | `bool` | 获取组件是否启用 |
| `setEnabled(bool enabled)` | `enabled`: 是否启用 | `void` | 设置组件是否启用 |
| `isEnabledInHierarchy()` | 无 | `bool` | 获取组件在层级中是否激活 |
| `setNodeActiveInHierarchy(bool isActiveInHierarchy)` | `isActiveInHierarchy`: 是否在层级中激活 | `void` | 设置组件在层级中是否激活 |

### 属性访问

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getUuid()` | 无 | `std::string` | 获取组件唯一标识符 |
| `getName()` | 无 | `std::string` | 获取组件名称 |
| `getNode()` | 无 | `Node*` | 获取组件所属节点 |
| `getLayer()` | 无 | `EComponentLayer` | 获取组件所在层级 |

## 组件工厂 (ComponentFactory)

### 单例方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getInstance()` | 无 | `ComponentFactory&` | 获取组件工厂实例 |

### 组件创建与查询

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `createComponent(const std::string &name, Boo::Node *node, std::string uuid)` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | `Component*` | 创建组件实例 |
| `getComponentDescription(const std::string &name)` | `name`: 组件名称 | `std::string` | 获取组件描述 |
| `isComponentRegistered(const std::string &typeName) const` | `typeName`: 组件类型名称 | `bool` | 检查组件是否注册 |

## 相机组件 (Camera)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `Camera(std::string name, Node *node, std::string uuid = "")` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | - | 创建相机组件实例 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `  OnAwake()` | 无 | `void` | 相机初始化时调用 |
| `  OnEnable ()` | 无 | `void` | 相机启用时调用 |
| `Update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新时调用 |
| `LateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新时调用 |
| `Render()` | 无 | `void` | 相机渲染时调用 |
| `  OnDisable()` | 无 | `void` | 相机关用时调用 |
| `destroy()` | 无 | `void` | 相机销毁时调用 |

### 属性设置

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setIsOnScreen(bool isOnScreen)` | `isOnScreen`: 是否在屏幕上 | `void` | 设置相机是否在屏幕上渲染 |
| `setPriority(int priority)` | `priority`: 优先级 | `void` | 设置相机渲染优先级 |
| `setGroupIDs(int groupIDs)` | `groupIDs`: 组ID | `void` | 设置相机可见的组ID |
| `addGroupID(int groupID)` | `groupID`: 组ID | `void` | 添加相机可见的组ID |
| `updateViewSize()` | 无 | `void` | 更新相机视图大小 |

### 属性获取

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `getIsOnScreen()` | 无 | `bool` | 获取相机是否在屏幕上渲染 |
| `getPriority()` | 无 | `int` | 获取相机渲染优先级 |
| `getGroupIDs()` | 无 | `int` | 获取相机可见的组ID |

## UI精灵组件 (UISprite)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `UISprite(std::string name, Node *node, std::string uuid = "")` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | - | 创建UI精灵组件实例 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `  OnAwake()` | 无 | `void` | 精灵初始化时调用 |
| `  OnEnable ()` | 无 | `void` | 精灵启用时调用 |
| `Update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新时调用 |
| `LateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新时调用 |
| `Render(Camera *camera)` | `camera`: 相机 | `void` | 精灵渲染时调用 |
| `  OnDisable()` | 无 | `void` | 精灵禁用时调用 |
| `destroy()` | 无 | `void` | 精灵销毁时调用 |

### 属性设置

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setColor(Color &color)` | `color`: 颜色 | `void` | 设置精灵颜色 |
| `setColor(std::string color)` | `color`: 颜色字符串 | `void` | 设置精灵颜色 |
| `setColor(float r, float g, float b, float a)` | `r`, `g`, `b`, `a`: 颜色通道值 | `void` | 设置精灵颜色 |
| `setAlpha(float alpha)` | `alpha`: 透明度 | `void` | 设置精灵透明度 |
| `setTexture(std::string texture)` | `texture`: 纹理路径 | `void` | 设置精灵纹理 |
| `setTexture(TextureAsset *texture)` | `texture`: 纹理资产 | `void` | 设置精灵纹理 |
| `setRenderTexture()` | 无 | `void` | 设置精灵渲染目标 |
| `setMaterial(std::string material)` | `material`: 材质路径 | `void` | 设置精灵材质 |
| `setMaterial(MaterialAsset *material)` | `material`: 材质资产 | `void` | 设置精灵材质 |

## UI文本组件 (UIText)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `UIText(std::string name, Node *node, std::string uuid = "")` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | - | 创建UI文本组件实例 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `  OnAwake()` | 无 | `void` | 文本初始化时调用 |
| `  OnEnable ()` | 无 | `void` | 文本启用时调用 |
| `Update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新时调用 |
| `LateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新时调用 |
| `Render(Camera *camera)` | `camera`: 相机 | `void` | 文本渲染时调用 |
| `  OnDisable()` | 无 | `void` | 文本禁用时调用 |
| `destroy()` | 无 | `void` | 文本销毁时调用 |

### 属性设置

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setText(std::string text)` | `text`: 文本内容 | `void` | 设置文本内容 |
| `setSize(int fontSize)` | `fontSize`: 字体大小 | `void` | 设置字体大小 |
| `setLineHeight(int lineHeight)` | `lineHeight`: 行高 | `void` | 设置文本行高 |
| `setColor(Color &color)` | `color`: 颜色 | `void` | 设置文本颜色 |
| `setColor(std::string color)` | `color`: 颜色字符串 | `void` | 设置文本颜色 |
| `setColor(float r, float g, float b, float a)` | `r`, `g`, `b`, `a`: 颜色通道值 | `void` | 设置文本颜色 |
| `setAlpha(float alpha)` | `alpha`: 透明度 | `void` | 设置文本透明度 |

## UI遮罩组件 (UIMask)

### 构造函数

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `UIMask(std::string name, Node *node, std::string uuid = "")` | `name`: 组件名称<br>`node`: 所属节点<br>`uuid`: 组件唯一标识符 | - | 创建UI遮罩组件实例 |

### 生命周期方法

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `  OnAwake()` | 无 | `void` | 遮罩初始化时调用 |
| `  OnEnable ()` | 无 | `void` | 遮罩启用时调用 |
| `Update(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧更新时调用 |
| `LateUpdate(float deltaTime)` | `deltaTime`: 时间步长 | `void` | 每一帧晚更新时调用 |
| `Render(Camera *camera)` | `camera`: 相机 | `void` | 遮罩渲染时调用 |
| `  OnDisable()` | 无 | `void` | 遮罩禁用时调用 |
| `destroy()` | 无 | `void` | 遮罩销毁时调用 |

### 属性设置

| 方法 | 参数 | 返回类型 | 描述 |
|------|------|----------|------|
| `setAlpha(float alpha)` | `alpha`: 透明度 | `void` | 设置遮罩透明度 |
