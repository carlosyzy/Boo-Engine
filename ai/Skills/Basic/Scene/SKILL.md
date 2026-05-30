---
name: "scene"
description: "Boo引擎的场景系统，负责管理游戏中的所有节点和组件，提供层次化的结构来组织游戏对象，支持2D和3D场景的管理。"
---

# Scene 系统

Scene 系统是 Boo 引擎的核心功能之一，负责管理游戏中的所有节点和组件，提供了一个层次化的结构来组织游戏对象。场景系统支持 2D 和 3D 场景的管理，为游戏开发提供了灵活的场景组织方式。

## 核心功能

### 场景管理
- **场景创建**：创建新的场景实例
- **根节点管理**：管理 2D 和 3D 根节点
- **场景生命周期**：处理场景的启动、更新和销毁
- **场景切换**：通过 `game->openScene()` 切换场景

### 节点系统
- **节点层次结构**：支持节点的父子关系，形成树状结构
- **变换系统**：支持位置、旋转、缩放等变换操作
- **激活状态管理**：控制节点的激活状态和层级激活状态
- **组件管理**：为节点添加和管理组件

### 2D 节点特性
- **锚点系统**：支持 2D 节点的锚点设置
- **大小设置**：设置 2D 节点的大小
- **UI 渲染**：集成 UI 渲染功能

### 3D 节点特性
- **3D 变换**：支持 3D 空间中的变换操作
- **3D 场景管理**：管理 3D 场景中的对象

## 主要接口

### 场景类 (Scene)

```cpp
// 构造函数
Scene(const std::string name, const std::string uuid = "");

// 根节点获取（引擎自动创建，无需手动调用 createRoot2D/3D）
Node3D *getRoot3D();
Node2D *getRoot2D();

// 生命周期方法
void update(float deltaTime) override;
void lateUpdate(float deltaTime) override;
void clearNodeFrameFlag() override;
void destroy() override;

// 变换方法（重写自 Node）
void setPosition(float x, float y, float z) override;
void setWorldPosition(float x, float y, float z) override;
void setScale(float x, float y, float z) override;
void setWorldScale(float x, float y, float z) override;
void setEulerAngles(float x, float y, float z) override;
void setRotation(float x, float y, float z, float w) override;
void setWorldRotation(float x, float y, float z, float w) override;
void setActive(bool active) override;

// 组件管理
Component *addComponent(std::string name, std::string uuid = "") override;
```

### 节点基类 (Node)

```cpp
// 构造函数
Node(const std::string name, const std::string uuid = "");

// 基本属性
const ENodeLayer getLayer() const;
const std::string& getUuid() const;
void setUuid(const std::string &uuid);
void setName(const std::string &name);
const std::string& getName() const;
int getGroupId() const;
void setGroupId(int groupID);

// 变换操作
virtual void setPosition(float x, float y, float z);
const Vec3 &getPosition();
virtual void setWorldPosition(float x, float y, float z);
const Vec3 &getWorldPosition();
virtual void setRotation(float x, float y, float z, float w);
const Quat &getRotation();
virtual void setWorldRotation(float x, float y, float z, float w);
const Quat &getWorldRotation();
virtual void setScale(float x, float y, float z);
const Vec3 &getScale();
virtual void setWorldScale(float x, float y, float z);
const Vec3 &getWorldScale();
virtual void setEulerAngles(float x, float y, float z);
const Vec3 &getEulerAngles();
const Mat4 &getLocalMatrix();
const Mat4 &getWorldMatrix();

// 变换状态
const bool hasFrameTransformFlag() const;
void setMatrix(const std::array<float, 16>& matrix);
virtual void clearNodeFrameFlag();

// 节点层次结构
void addChild(Node *node);
void removeChild(Node *node);
void removeFromParent();
const std::vector<Node *> &getChildren();
Node *getChildByName(std::string name);
void setParent(Node *parent);
Node *getParent();
void destroyAllChildren();

// 激活状态
virtual void setActive(bool active);
bool getActive() const;
const bool getActiveInHierarchy() const;

// 组件管理
virtual Component *addComponent(std::string name, std::string uuid = "");
Component *getComponent(std::string name);
std::vector<Component *> getComponents();

// 生命周期方法
virtual void update(float deltaTime);
virtual void lateUpdate(float deltaTime);
virtual void destroy();
```

### 2D 节点类 (Node2D)

```cpp
// 构造函数
Node2D(const std::string name, const std::string uuid = "");

// 2D 特有属性
void setAnchor(float x, float y);
void setSize(float width, float height);
const Vec2 &getAnchor();
const Size &getSize();
const Mat4 &getUIWorldMatrix();

// 变换方法（重写自 Node）
void setPosition(float x, float y, float z) override;
void setWorldPosition(float x, float y, float z) override;
void setScale(float x, float y, float z) override;
void setWorldScale(float x, float y, float z) override;
void setEulerAngles(float x, float y, float z) override;
void setRotation(float x, float y, float z, float w) override;
void setWorldRotation(float x, float y, float z, float w) override;

// 激活状态
void setActive(bool active) override;

// 组件管理
Component *addComponent(std::string name, std::string uuid = "") override;
UIRenderer *getUIRenderer();

// 碰撞检测
bool inHitMask(float x, float y);
bool inHitOnNode(float x, float y);

// 生命周期方法
void update(float deltaTime) override;
void lateUpdate(float deltaTime) override;
void clearNodeFrameFlag() override;
void destroy() override;
```

### 3D 节点类 (Node3D)

```cpp
// 构造函数
Node3D(const std::string name, const std::string uuid = "");

// 获取世界矩阵逆转置（用于法线变换）
const Mat4& getWorldMatrixIT();

// 获取网格渲染器
MeshRenderer* getMeshRenderer();
```

## 使用示例

### 创建并打开场景

```cpp
// 创建场景
Boo::Scene* scene = new Boo::Scene("MainScene");

// 打开场景（重要：new Scene 后必须紧接着调用此方法）
Boo::game->openScene(scene);

// 注意：根节点由引擎自动创建，不要手动调用 createRoot2D/createRoot3D
```

### 创建 2D 节点

```cpp
// 获取当前场景
Boo::Scene* scene = Boo::game->getScene();

// 获取 2D 根节点
Boo::Node2D* root2D = scene->getRoot2D();

// 创建 2D 节点
Boo::Node2D* uiNode = new Boo::Node2D("UINode");
uiNode->setPosition(100, 100, 0);
uiNode->setSize(200, 100);
uiNode->setAnchor(0.5f, 0.5f);

// 添加到根节点
root2D->addChild(uiNode);

```

### 创建 3D 节点

```cpp
// 获取当前场景
Boo::Scene* scene = Boo::game->getScene();

// 获取 3D 根节点
Boo::Node3D* root3D = scene->getRoot3D();

// 创建 3D 节点
Boo::Node3D* modelNode = new Boo::Node3D("ModelNode");

// 添加到根节点
root3D->addChild(modelNode);
```

### 节点变换操作

```cpp
// 创建节点
Boo::Node2D* node = new Boo::Node2D("TransformNode");

// 设置位置
node->setPosition(100, 100, 0);

// 设置旋转（欧拉角）
node->setEulerAngles(0, 0, 45); // 45度旋转

// 设置缩放
node->setScale(2, 2, 1); // 放大2倍

// 获取变换信息
Boo::Vec3 position = node->getPosition();
Boo::Vec3 rotation = node->getEulerAngles();
Boo::Vec3 scale = node->getScale();

std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
std::cout << "Rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;
std::cout << "Scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;
```

### 组件管理

```cpp
// 创建节点
Boo::Node2D* node = new Boo::Node2D("ComponentNode");

// 添加相机组件
Boo::Component* camera = node->addComponent("Camera");

// 添加 UI 精灵组件
Boo::Component* sprite = node->addComponent("UISprite");

// 获取组件
Boo::Component* existingCamera = node->getComponent("Camera");

// 获取所有组件
std::vector<Boo::Component*> components = node->getComponents();
std::cout << "Number of components: " << components.size() << std::endl;
```

## 节点层次结构

### 场景层次

```
Scene
├── Root2D (Node2D)
│   ├── UI节点1
│   ├── UI节点2
│   └── ...
└── Root3D (Node3D)
    ├── 3D模型1
    ├── 3D模型2
    └── ...
```

### 节点关系

- **父节点**：每个节点可以有一个父节点
- **子节点**：每个节点可以有多个子节点
- **根节点**：Scene 作为最顶层的根节点
- **层级激活**：节点的激活状态会影响其子节点的激活状态

## 变换系统

### 本地变换与世界变换

- **本地变换**：相对于父节点的变换
- **世界变换**：相对于世界坐标系的变换
- **变换矩阵**：使用 4x4 矩阵表示变换

### 变换标志

- **世界变换标志**：标记世界变换是否需要更新
- **帧变换标志**：标记当前帧是否发生了变换

## 激活状态管理

- **激活状态**：节点自身的激活状态
- **层级激活状态**：节点在整个层级中的激活状态，取决于自身和所有父节点的激活状态

## 性能优化

1. **减少变换操作**：避免在每一帧频繁修改节点的变换属性
2. **合理组织节点层次**：避免过深的节点层次结构
3. **使用激活状态**：通过禁用不需要的节点来减少更新和渲染开销
4. **批量操作**：对多个节点的相同操作进行批量处理

## 常见问题

- **节点不显示**：检查节点的激活状态和层级激活状态
- **变换不生效**：检查变换操作是否正确，以及是否需要手动更新变换矩阵
- **性能问题**：检查节点数量是否过多，或变换操作是否过于频繁
- **组件添加失败**：检查组件名称是否正确，以及组件是否已注册

## 总结

场景系统是 Boo 引擎的核心功能之一，它提供了一个灵活、高效的方式来组织和管理游戏对象。通过场景系统，开发者可以：

- 创建和管理 2D 和 3D 场景
- 构建复杂的节点层次结构
- 控制节点的变换和激活状态
- 为节点添加和管理组件
- 优化游戏性能

场景系统的设计理念是"层次化管理"，通过树状结构来组织游戏对象，使得游戏开发更加模块化和可维护。

## 注意事项

- 创建场景后，必须立即调用 `Boo::game->openScene(scene)` 来打开场景，否则场景不会被引擎管理和渲染
- 场景中的 2D 和 3D 根节点由引擎在 `openScene` 后自动创建，不要手动调用 `createRoot2D/3D`
- 节点的激活状态会影响其子节点的激活状态，因此需要合理管理节点的激活状态