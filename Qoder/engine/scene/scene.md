# Scene 模块说明

Scene类是场景管理类，继承自Node基类。负责管理场景中的所有节点，提供节点的添加、删除、查找等功能。Scene作为场景树的根节点，组织和管理整个场景的层次结构。

## 类关系

Scene继承了Node的所有属性和方法，并扩展了场景管理功能。

## 构造函数

- `Scene(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建场景，可选择性提供UUID

## 方法

### 场景管理方法
- `void addNode(std::shared_ptr<Node> node)`: 添加节点到场景
- `void removeNode(std::shared_ptr<Node> node)`: 从场景中移除指定节点
- `std::shared_ptr<Node> getNode(const std::string& name)`: 根据名称获取节点
- `std::vector<std::shared_ptr<Node>> getNodes() const`: 获取场景中的所有节点

### 生命周期方法
- `virtual void update() override`: 场景更新逻辑
- `virtual void render() override`: 场景渲染逻辑

## 使用示例

```cpp
Scene scene("MyScene");

// 添加节点到场景
auto node = std::make_shared<Node2D>("MyNode");
scene.addNode(node);
```

## 注意事项

1. Scene作为场景树的根节点，是场景管理的入口点
2. 所有场景中的节点都应该通过Scene进行管理
3. Scene继承自Node，具有节点的所有功能