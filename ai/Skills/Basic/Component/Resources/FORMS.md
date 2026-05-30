# 组件系统表单定义

## 组件基类表单

### 公有属性

| 字段名 | 类型 | 描述 | 默认值 |
|--------|------|------|--------|
| `name` | `std::string` | 组件名称 | - |
| `node` | `Node*` | 所属节点 | - |
| `uuid` | `std::string` | 组件唯一标识符 | "" |
| `enabled` | `bool` | 是否启用 | true |

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `Component(std::string name, Node *node, std::string uuid = "")` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | - | 构造函数 |
| `  OnAwake()` | 无 | void | 组件唤醒时调用 |
| `  OnEnable ()` | 无 | void | 组件启用时调用 |
| `Update(float deltaTime)` | deltaTime: 时间步长 | void | 每帧更新时调用 |
| `LateUpdate(float deltaTime)` | deltaTime: 时间步长 | void | 每帧晚更新时调用 |
| `  OnDisable()` | 无 | void | 组件禁用时调用 |
| `destroy()` | 无 | void | 组件销毁时调用 |
| `isEnabled()` | 无 | bool | 获取组件是否启用 |
| `setEnabled(bool enabled)` | enabled: 是否启用 | void | 设置组件是否启用 |
| `isEnabledInHierarchy()` | 无 | bool | 获取组件在层级中是否启用 |
| `getName()` | 无 | std::string | 获取组件名称 |
| `getNode()` | 无 | Node* | 获取所属节点 |
| `getLayer()` | 无 | EComponentLayer | 获取组件所在层级 |
| `getUuid()` | 无 | std::string | 获取组件唯一标识符 |

## 组件工厂表单

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `createComponent(const std::string& name, Boo::Node* node, std::string uuid)` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | Component* | 创建组件实例 |
| `isComponentRegistered(const std::string& typeName) const` | typeName: 组件类型名称 | bool | 检查组件是否注册 |

## 相机组件表单

### 公有属性

| 字段名 | 类型 | 描述 | 默认值 |
|--------|------|------|--------|
| `isOnScreen` | `bool` | 是否在屏幕上渲染 | false |
| `priority` | `int` | 渲染优先级 | 0 |
| `groupIDs` | `int` | 可见组ID | 1 |

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `Camera(std::string name, Node *node, std::string uuid = "")` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | - | 构造函数 |
| `  OnAwake()` | 无 | void | 组件唤醒时调用 |
| `  OnEnable ()` | 无 | void | 组件启用时调用 |
| `Update(float deltaTime)` | deltaTime: 时间步长 | void | 每帧更新时调用 |
| `LateUpdate(float deltaTime)` | deltaTime: 时间步长 | void | 每帧晚更新时调用 |
| `Render()` | 无 | void | 渲染时调用 |
| `  OnDisable()` | 无 | void | 组件禁用时调用 |
| `destroy()` | 无 | void | 组件销毁时调用 |
| `setIsOnScreen(bool isOnScreen)` | isOnScreen: 是否在屏幕上渲染 | void | 设置是否在屏幕上渲染 |
| `setPriority(int priority)` | priority: 渲染优先级 | void | 设置渲染优先级 |
| `setGroupIDs(int groupIDs)` | groupIDs: 可见组ID | void | 设置可见组ID |
| `addGroupID(int groupID)` | groupID: 组ID | void | 添加可见组ID |
| `updateViewSize()` | 无 | void | 更新视图大小 |
| `getIsOnScreen()` | 无 | bool | 获取是否在屏幕上渲染 |
| `getPriority()` | 无 | int | 获取渲染优先级 |
| `getGroupIDs()` | 无 | int | 获取可见组ID |

## UI精灵组件表单

### 公有属性

| 字段名 | 类型 | 描述 | 默认值 |
|--------|------|------|--------|
| `color` | `Color` | 精灵颜色 | Color(1, 1, 1, 1) |
| `texture` | `std::string` | 纹理路径 | "" |
| `material` | `std::string` | 材质路径 | "" |

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `UISprite(std::string name, Node *node, std::string uuid = "")` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | - | 构造函数 |
| `  OnAwake()` | 无 | void | 组件唤醒时调用 |
| `  OnEnable ()` | 无 | void | 组件启用时调用 |
| `Update(float deltaTime)` | deltaTime: 时间步长 | void | 每帧更新时调用 |
| `LateUpdate(float deltaTime)` | deltaTime: 时间步长 | void | 每帧晚更新时调用 |
| `Render(Camera *camera)` | camera: 相机组件 | void | 渲染时调用 |
| `  OnDisable()` | 无 | void | 组件禁用时调用 |
| `destroy()` | 无 | void | 组件销毁时调用 |
| `setColor(Color &color)` | color: 颜色对象 | void | 设置精灵颜色 |
| `setColor(std::string color)` | color: 颜色字符串 | void | 设置精灵颜色 |
| `setColor(float r, float g, float b, float a)` | r: 红色通道<br>g: 绿色通道<br>b: 蓝色通道<br>a: 透明度通道 | void | 设置精灵颜色 |
| `setAlpha(float alpha)` | alpha: 透明度 | void | 设置精灵透明度 |
| `setTexture(std::string texture)` | texture: 纹理路径 | void | 设置精灵纹理 |
| `setTexture(TextureAsset *texture)` | texture: 纹理资产 | void | 设置精灵纹理 |
| `setRenderTexture()` | 无 | void | 设置渲染纹理 |
| `setMaterial(std::string material)` | material: 材质路径 | void | 设置精灵材质 |
| `setMaterial(MaterialAsset *material)` | material: 材质资产 | void | 设置精灵材质 |

## UI文本组件表单

### 公有属性

| 字段名 | 类型 | 描述 | 默认值 |
|--------|------|------|--------|
| `text` | `std::string` | 文本内容 | "" |
| `fontSize` | `int` | 字体大小 | 16 |
| `color` | `Color` | 文本颜色 | Color(1, 1, 1, 1) |

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `UIText(std::string name, Node *node, std::string uuid = "")` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | - | 构造函数 |
| `  OnAwake()` | 无 | void | 组件唤醒时调用 |
| `  OnEnable ()` | 无 | void | 组件启用时调用 |
| `Update(float deltaTime)` | deltaTime: 时间步长 | void | 每帧更新时调用 |
| `LateUpdate(float deltaTime)` | deltaTime: 时间步长 | void | 每帧晚更新时调用 |
| `Render(Camera *camera)` | camera: 相机组件 | void | 渲染时调用 |
| `  OnDisable()` | 无 | void | 组件禁用时调用 |
| `destroy()` | 无 | void | 组件销毁时调用 |
| `setText(std::string text)` | text: 文本内容 | void | 设置文本内容 |
| `setSize(int fontSize)` | size: 字体大小 | void | 设置字体大小 |
| `setColor(Color &color)` | color: 颜色对象 | void | 设置文本颜色 |
| `setColor(std::string color)` | color: 颜色字符串 | void | 设置文本颜色 |
| `setColor(float r, float g, float b, float a)` | r: 红色通道<br>g: 绿色通道<br>b: 蓝色通道<br>a: 透明度通道 | void | 设置文本颜色 |
| `setAlpha(float alpha)` | alpha: 透明度 | void | 设置文本透明度 |

## UI遮罩组件表单

### 公有属性

| 字段名 | 类型 | 描述 | 默认值 |
|--------|------|------|--------|
| `alpha` | `float` | 遮罩透明度 | 1.0f |

### 公有函数

| 函数名 | 参数 | 返回值 | 描述 |
|--------|------|--------|------|
| `UIMask(std::string name, Node *node, std::string uuid = "")` | name: 组件名称<br>node: 所属节点<br>uuid: 唯一标识符 | - | 构造函数 |
| `  OnAwake()` | 无 | void | 组件唤醒时调用 |
| `  OnEnable ()` | 无 | void | 组件启用时调用 |
| `Update(float deltaTime)` | deltaTime: 时间步长 | void | 每帧更新时调用 |
| `LateUpdate(float deltaTime)` | deltaTime: 时间步长 | void | 每帧晚更新时调用 |
| `Render(Camera *camera)` | camera: 相机组件 | void | 渲染时调用 |
| `  OnDisable()` | 无 | void | 组件禁用时调用 |
| `destroy()` | 无 | void | 组件销毁时调用 |
| `setAlpha(float alpha)` | alpha: 透明度 | void | 设置遮罩透明度 |
