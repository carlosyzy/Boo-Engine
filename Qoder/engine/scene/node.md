# Node 模块说明

Node类是所有场景节点的基类，提供了节点的基本属性和功能。所有的2D和3D节点都继承自这个基类。Node使用Vec3类型来表示位置、缩放和欧拉角，所有节点都具有这些变换属性。

## 构造函数

- `Node()`: 默认构造函数，创建名为"Node"的节点，自动生成UUID
- `Node(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建节点，可选择性提供UUID

## 属性

### 基础属性
- `_name`: 节点名称
- `_uuid`: 节点唯一标识符
- `_active`: 节点激活状态
- `_layer`: 节点层级类型

### 变换属性
- `_position`: 节点位置(Vec3)
- `_scale`: 节点缩放(Vec3)
- `_eulerAngles`: 节点欧拉角(Vec3)
- `_rotate`: 节点旋转(Quat)

### 关系属性
- `_parent`: 父节点指针
- `_children`: 子节点列表

## 方法

### 基础属性方法
- `void setName(const std::string& name)`: 设置节点名称
- `std::string getName() const`: 获取节点名称
- `void setActive(bool active)`: 设置节点激活状态
- `bool isActive() const`: 获取节点激活状态
- `NodeType getLayer() const`: 获取节点层级类型
- `std::string getUuid() const`: 获取节点UUID

### 变换属性方法
- `void setPosition(const Vec3& position)`: 设置节点位置
- `const Vec3& getPosition() const`: 获取节点位置
- `void setScale(const Vec3& scale)`: 设置节点缩放
- `const Vec3& getScale() const`: 获取节点缩放
- `void setEulerAngles(const Vec3& eulerAngles)`: 设置节点欧拉角
- `const Vec3& getEulerAngles() const`: 获取节点欧拉角

### 节点关系方法
- `void setParent(Node* node)`: 设置父节点
- `Node* getParent()`: 获取父节点
- `virtual void addChild(Node* node)`: 添加子节点
- `void removeChild(Node* node)`: 移除子节点
- `void destroyAllChildren()`: 销毁所有子节点

### 生命周期方法
- `virtual void update(float deltaTime)`: 更新方法
- `virtual void lateUpdate(float deltaTime)`: 晚期更新方法
- `virtual void render()`: 渲染方法

## 使用示例

```cpp
Node node("MyNode");
```

## 注意事项

1. Node是一个虚基类，通常不会直接实例化使用
2. 所有具体的节点类型都应该继承自Node类
3. 变换属性遵循标准的3D变换规则