# Scene 模块说明

## 概述
Scene类用于管理游戏场景中的所有节点。提供了节点的添加、删除、查找等功能，以及统一的更新和渲染机制。

## 功能特性

### 构造函数
- `Scene()`: 默认构造函数，创建名为"Scene"的场景，自动生成UUID
- `Scene(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建场景，可选择性提供UUID

### 场景属性
- `void setName(const std::string& name)`: 设置场景名称
- `std::string getName() const`: 获取场景名称
- `void setUuid(const std::string& uuid)`: 设置场景UUID
- `std::string getUuid() const`: 获取场景UUID

### 节点管理
- `void addNode(std::shared_ptr<NodeBase> node)`: 添加节点到场景
- `void removeNode(std::shared_ptr<NodeBase> node)`: 从场景中移除指定节点
- `void removeNode(const std::string& name)`: 根据名称从场景中移除节点
- `std::shared_ptr<NodeBase> getNode(const std::string& name)`: 根据名称获取节点
- `std::vector<std::shared_ptr<NodeBase>> getNodes() const`: 获取场景中的所有节点

### 场景操作
- `virtual void update()`: 更新场景中的所有节点
- `virtual void render()`: 渲染场景中的所有节点

## 使用示例
```cpp
// 创建场景
Scene scene("MainScene");

// 创建节点
auto node2d = std::make_shared<Node2D>("Player");
auto node3d = std::make_shared<Node3D>("Enemy");

// 添加节点到场景
scene.addNode(node2d);
scene.addNode(node3d);

// 更新和渲染整个场景
scene.update();
scene.render();

// 获取场景UUID
std::string uuid = scene.getUuid();
```

## 注意事项
1. Scene使用std::shared_ptr管理节点生命周期
2. 场景的update()和render()方法会遍历所有节点并调用对应方法
3. removeNode()方法有两种重载形式，可以根据指针或名称移除节点
4. getNode()方法根据名称查找节点，未找到时返回nullptr
5. 每个场景都有唯一的UUID标识符