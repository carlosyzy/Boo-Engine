# 场景系统使用示例

## 创建场景

### 基本场景创建

```cpp
#include "engine/boo.h"

void createBasicScene() {
    // 创建场景
    Boo::Scene* scene = new Boo::Scene("MainScene");
    
    // 打开场景（重要：创建场景后立即调用此方法）
    Boo::game->openScene(scene);
    
    // 注意：createRoot2D、createRoot3D 和 launch 方法会由引擎自动调用
}
```

## 节点操作

### 创建 2D 节点层次结构

```cpp
#include "engine/boo.h"

void create2DHierarchy() {
    // 获取场景
    Boo::Scene* scene = Boo::game->getScene();
    
    // 获取 2D 根节点
    Boo::Node2D* root2D = scene->getRoot2D();
    
    // 创建 UI 容器节点
    Boo::Node2D* uiContainer = new Boo::Node2D("UIContainer");
    uiContainer->setPosition(0, 0, 0);
    root2D->addChild(uiContainer);
    
    // 创建背景节点
    Boo::Node2D* background = new Boo::Node2D("Background");
    background->setPosition(400, 300, 0);
    background->setSize(800, 600);
    uiContainer->addChild(background);
    
    // 创建按钮节点
    Boo::Node2D* button = new Boo::Node2D("Button");
    button->setPosition(400, 300, 1);
    button->setSize(200, 50);
    button->setAnchor(0.5f, 0.5f);
    uiContainer->addChild(button);
    
    // 创建文本节点
    Boo::Node2D* text = new Boo::Node2D("Text");
    text->setPosition(0, 0, 1);
    text->setAnchor(0.5f, 0.5f);
    button->addChild(text);
}
```

### 创建 3D 节点层次结构

```cpp
#include "engine/boo.h"

void create3DHierarchy() {
    // 获取场景
    Boo::Scene* scene = Boo::game->getScene();
    
    // 获取 3D 根节点
    Boo::Node3D* root3D = scene->getRoot3D();
    
    // 创建玩家节点
    Boo::Node3D* player = new Boo::Node3D("Player");
    player->setPosition(0, 0, 0);
    root3D->addChild(player);
    
    // 创建武器节点
    Boo::Node3D* weapon = new Boo::Node3D("Weapon");
    weapon->setPosition(0.5f, 0, 1);
    player->addChild(weapon);
    
    // 创建头部节点
    Boo::Node3D* head = new Boo::Node3D("Head");
    head->setPosition(0, 1.8f, 0);
    player->addChild(head);
}
```

## 变换操作

### 2D 节点变换

```cpp
#include "engine/boo.h"

void transform2DNode() {
    // 创建 2D 节点
    Boo::Node2D* node = new Boo::Node2D("TransformNode");
    
    // 设置位置
    node->setPosition(100, 100, 0);
    
    // 设置大小
    node->setSize(200, 100);
    
    // 设置锚点
    node->setAnchor(0.5f, 0.5f);
    
    // 设置旋转（欧拉角）
    node->setEulerAngles(0, 0, 45); // 45度旋转
    
    // 设置旋转（角度，弧度）
    node->setAngle(45); // 45度旋转（π/4弧度）
    
    // 设置缩放
    node->setScale(1.5f, 1.5f, 1); // 放大1.5倍
    
    // 获取变换信息
    Boo::Vec3 position = node->getPosition();
    Boo::Vec3 rotation = node->getEulerAngles();
    Boo::Vec3 scale = node->getScale();
    Boo::Vec2 anchor = node->getAnchor();
    Boo::Size size = node->getSize();
    
    std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    std::cout << "Rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;
    std::cout << "Scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;
    std::cout << "Anchor: " << anchor.x << ", " << anchor.y << std::endl;
    std::cout << "Size: " << size.width << ", " << size.height << std::endl;
}
```

### 3D 节点变换

```cpp
#include "engine/boo.h"

void transform3DNode() {
    // 创建 3D 节点
    Boo::Node3D* node = new Boo::Node3D("TransformNode3D");
    
    // 设置位置
    node->setPosition(1, 2, 3);
    
    // 设置旋转（欧拉角）
    node->setEulerAngles(45, 30, 0); // 45度X轴旋转，30度Y轴旋转
    
    // 设置缩放
    node->setScale(2, 2, 2); // 放大2倍
    
    // 获取变换信息
    Boo::Vec3 position = node->getPosition();
    Boo::Vec3 rotation = node->getEulerAngles();
    Boo::Vec3 scale = node->getScale();
    
    std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    std::cout << "Rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;
    std::cout << "Scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;
}
```

## 激活状态管理

### 节点激活与禁用

```cpp
#include "engine/boo.h"

void manageActivation() {
    Boo::Scene* scene = new Boo::Scene("TestScene");
    Boo::game->openScene(scene);
    Boo::Node2D* root2D = scene->getRoot2D();
    
    // 创建父节点
    Boo::Node2D* parentNode = new Boo::Node2D("ParentNode");
    parentNode->setActive(true);
    root2D->addChild(parentNode);
    
    // 创建子节点
    Boo::Node2D* childNode1 = new Boo::Node2D("ChildNode1");
    childNode1->setActive(true);
    parentNode->addChild(childNode1);
    
    Boo::Node2D* childNode2 = new Boo::Node2D("ChildNode2");
    childNode2->setActive(false);
    parentNode->addChild(childNode2);
    
    // 检查激活状态
    std::cout << "Parent active: " << parentNode->getActive() << std::endl;
    std::cout << "Parent active in hierarchy: " << parentNode->getActiveInHierarchy() << std::endl;
    std::cout << "Child1 active: " << childNode1->getActive() << std::endl;
    std::cout << "Child1 active in hierarchy: " << childNode1->getActiveInHierarchy() << std::endl;
    std::cout << "Child2 active: " << childNode2->getActive() << std::endl;
    std::cout << "Child2 active in hierarchy: " << childNode2->getActiveInHierarchy() << std::endl;
    
    // 禁用父节点
    parentNode->setActive(false);
    
    // 检查激活状态
    std::cout << "\nAfter disabling parent:" << std::endl;
    std::cout << "Parent active: " << parentNode->getActive() << std::endl;
    std::cout << "Parent active in hierarchy: " << parentNode->getActiveInHierarchy() << std::endl;
    std::cout << "Child1 active: " << childNode1->getActive() << std::endl;
    std::cout << "Child1 active in hierarchy: " << childNode1->getActiveInHierarchy() << std::endl;
    std::cout << "Child2 active: " << childNode2->getActive() << std::endl;
    std::cout << "Child2 active in hierarchy: " << childNode2->getActiveInHierarchy() << std::endl;
}
```

## 组件管理

### 为节点添加组件

```cpp
#include "engine/boo.h"

void addComponentsToNode() {
    // 创建节点
    Boo::Node2D* node = new Boo::Node2D("ComponentNode");
    
    // 添加相机组件
    Boo::Component* camera = node->addComponent("Camera");
    if (camera) {
        std::cout << "Added Camera component" << std::endl;
    }
    
    // 添加 UI 精灵组件
    Boo::Component* sprite = node->addComponent("UISprite");
    if (sprite) {
        std::cout << "Added UISprite component" << std::endl;
    }
    
    // 添加 UI 文本组件
    Boo::Component* text = node->addComponent("UIText");
    if (text) {
        std::cout << "Added UIText component" << std::endl;
    }
    
    // 获取组件
    Boo::Component* existingCamera = node->getComponent("Camera");
    if (existingCamera) {
        std::cout << "Retrieved Camera component" << std::endl;
    }
    
    // 获取所有组件
    std::vector<Boo::Component*> components = node->getComponents();
    std::cout << "Total components: " << components.size() << std::endl;
    for (auto comp : components) {
        std::cout << "Component: " << comp->getName() << std::endl;
    }
}
```

## 场景更新与销毁

### 场景生命周期管理

```cpp
#include "engine/boo.h"

void manageSceneLifecycle() {
    // 创建并打开场景（引擎接管后自动调用 launch）
    Boo::Scene* scene = new Boo::Scene("LifecycleScene");
    Boo::game->openScene(scene);

    // 销毁场景时通过 game 管理
    Boo::game->destroyScene();
}
```

## 完整游戏示例

### 2D 游戏场景

```cpp
#include "engine/boo.h"

class Player {
private:
    Boo::Node2D* _node;
    Boo::UISprite* _sprite;
    float _speed;

public:
    Player(Boo::Node2D* parent) {
        // 创建节点
        _node = new Boo::Node2D("Player");
        _node->setPosition(100, 300, 0);
        _node->setSize(50, 50);
        _node->setAnchor(0.5f, 0.5f);
        parent->addChild(_node);
        
        // 添加精灵组件
        _sprite = static_cast<Boo::UISprite*>(_node->addComponent("UISprite"));
        _sprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
        _sprite->setTexture("player.png");
        
        _speed = 200.0f;
    }
    
    void update(float deltaTime) {
        // 简单的移动逻辑
        Boo::Vec3 pos = _node->getPosition();
        pos.x += _speed * deltaTime;
        if (pos.x > 800) {
            pos.x = 0;
        }
        _node->setPosition(pos.x, pos.y, pos.z);
    }
};

class Enemy {
private:
    Boo::Node2D* _node;
    Boo::UISprite* _sprite;
    float _speed;

public:
    Enemy(Boo::Node2D* parent, float x, float y) {
        // 创建节点
        _node = new Boo::Node2D("Enemy");
        _node->setPosition(x, y, 0);
        _node->setSize(40, 40);
        _node->setAnchor(0.5f, 0.5f);
        parent->addChild(_node);
        
        // 添加精灵组件
        _sprite = static_cast<Boo::UISprite*>(_node->addComponent("UISprite"));
        _sprite->setColor(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
        _sprite->setTexture("enemy.png");
        
        _speed = 150.0f;
    }
    
    void update(float deltaTime) {
        // 简单的移动逻辑
        Boo::Vec3 pos = _node->getPosition();
        pos.x -= _speed * deltaTime;
        if (pos.x < 0) {
            pos.x = 800;
        }
        _node->setPosition(pos.x, pos.y, pos.z);
    }
};

class GameManager {
private:
    Boo::Scene* _scene;
    Player* _player;
    std::vector<Enemy*> _enemies;

public:
    void init() {
        // 创建场景
        _scene = new Boo::Scene("GameScene");

        // 打开场景（new Scene后必须紧接着调用open）
        Boo::game->openScene(_scene);

        // 获取 2D 根节点（引擎自动创建）
        Boo::Node2D* root2D = _scene->getRoot2D();
        
        // 创建玩家
        _player = new Player(root2D);
        
        // 创建敌人
        for (int i = 0; i < 5; i++) {
            float y = 100 + i * 100;
            _enemies.push_back(new Enemy(root2D, 700, y));
        }
        
        // 创建相机
        Boo::Node2D* cameraNode = new Boo::Node2D("CameraNode");
        root2D->addChild(cameraNode);
        Boo::Camera* camera = static_cast<Boo::Camera*>(cameraNode->addComponent("Camera"));
    }
    
    void update(float deltaTime) {
        // 更新玩家
        _player->update(deltaTime);

        // 更新敌人
        for (auto enemy : _enemies) {
            enemy->update(deltaTime);
        }
    }
    
    void cleanup() {
        // 销毁敌人
        for (auto enemy : _enemies) {
            delete enemy;
        }
        _enemies.clear();

        // 销毁玩家
        delete _player;

        // 通过 game 销毁场景
        Boo::game->destroyScene();
    }
};

// 主函数
int main() {
    // 初始化引擎
    // ...

    // 创建游戏管理器
    GameManager gameManager;
    gameManager.init();

    // 引擎主循环由 window/platform 驱动
    // ...

    return 0;
}
```