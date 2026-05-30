# 组件系统使用示例

## 内置组件使用

### 相机组件

```cpp
#include "engine/boo.h"

void setupCamera(Boo::Node2D* root2D) {
    Boo::Node2D* cameraNode = new Boo::Node2D("MainCamera");
    cameraNode->setPosition(0, 0, 0);
    root2D->addChild(cameraNode);

    Boo::Component* cameraComp = cameraNode->addComponent("Camera");
    Boo::Camera* camera = dynamic_cast<Boo::Camera*>(cameraComp);
    if (camera) {
        camera->setIsOnScreen(true);
        camera->setPriority(0);
    }
}
```

### UI组件

```cpp
#include "engine/boo.h"

void setupUI(Boo::Node2D* root2D) {
    // 背景节点
    Boo::Node2D* bgNode = new Boo::Node2D("Background");
    bgNode->setPosition(400, 300, 0);
    bgNode->setSize(800, 600);
    root2D->addChild(bgNode);

    Boo::UISprite* background = dynamic_cast<Boo::UISprite*>(bgNode->addComponent("UISprite"));
    if (background) {
        background->setColor(0.1f, 0.1f, 0.1f, 1.0f);
        background->setTexture(Boo::AssetBuiltinTexture::Default);
    }

    // 玩家精灵节点
    Boo::Node2D* playerNode = new Boo::Node2D("Player");
    playerNode->setPosition(100, 100, 0);
    playerNode->setSize(50, 50);
    root2D->addChild(playerNode);

    Boo::UISprite* playerSprite = dynamic_cast<Boo::UISprite*>(playerNode->addComponent("UISprite"));
    if (playerSprite) {
        playerSprite->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        playerSprite->setTexture(Boo::AssetBuiltinTexture::Default);
    }

    // 分数文本节点
    Boo::Node2D* scoreNode = new Boo::Node2D("ScoreText");
    scoreNode->setPosition(700, 500, 0);
    scoreNode->setSize(200, 40);
    root2D->addChild(scoreNode);

    Boo::UIText* scoreText = dynamic_cast<Boo::UIText*>(scoreNode->addComponent("UIText"));
    if (scoreText) {
        scoreText->setText("Score: 0");
        scoreText->setSize(24);
        scoreText->setLineHeight(30);
        scoreText->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // 遮罩节点
    Boo::Node2D* maskNode = new Boo::Node2D("Mask");
    maskNode->setPosition(300, 200, 0);
    maskNode->setSize(200, 200);
    root2D->addChild(maskNode);

    Boo::UIMask* mask = dynamic_cast<Boo::UIMask*>(maskNode->addComponent("UIMask"));
    if (mask) {
        mask->setAlpha(0.5f);
    }
}
```

## 创建自定义组件

### 1. 定义自定义组件

#### 头文件 (my-component.h)

```cpp
#pragma once
#include "engine/core/component/component.h"
#include "engine/core/component/component-register.h"

class GameLogic : public Boo::Component
{
private:
    int score;
    float timer;

public:
    GameLogic(std::string name, Boo::Node *node, std::string uuid = "");

    void   OnAwake() override;
    void   OnEnable () override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void   OnDisable() override;
    void destroy() override;
    ~GameLogic() override;

    void addScore(int value);
    int getScore() const;
};

REGISTER_COMPONENT(GameLogic, "Game Logic Component")
```

#### 源文件 (my-component.cpp)

```cpp
#include "my-component.h"

GameLogic::GameLogic(std::string name, Boo::Node *node, std::string uuid)
    : Boo::Component(name, node, uuid), score(0), timer(0.0f) {
}

void GameLogic::  OnAwake() {
    auto node = getNode();

    auto sprite = new Boo::Node2D("Background");
    node->addChild(sprite);
    sprite->setPosition(100, 100, 0);
    sprite->setSize(200, 200);

    auto spriteComp = dynamic_cast<Boo::UISprite*>(sprite->addComponent("UISprite"));
    if (spriteComp) {
        spriteComp->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        spriteComp->setTexture(Boo::AssetBuiltinTexture::Default);
    }
}

void GameLogic::  OnEnable () {
}

void GameLogic::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= 1.0f) {
        timer = 0.0f;
        addScore(10);
    }
}

void GameLogic::LateUpdate(float deltaTime) {
}

void GameLogic::  OnDisable() {
}

void GameLogic::destroy() {
    Boo::Component::destroy();
}

void GameLogic::addScore(int value) {
    score += value;
}

int GameLogic::getScore() const {
    return score;
}

GameLogic::~GameLogic() {
}
```

### 2. 使用自定义组件

```cpp
#include "my-component.h"

void setupGameLogic(Boo::Node2D* root2D) {
    Boo::Node2D* gameNode = new Boo::Node2D("GameManager");
    root2D->addChild(gameNode);

    Boo::Component* comp = gameNode->addComponent("GameLogic");
    GameLogic* gameLogic = dynamic_cast<GameLogic*>(comp);
    if (gameLogic) {
        gameLogic->addScore(100);
    }
}
```

**注意**：在组件内部创建子节点时，应使用 `getNode()` 获取组件所在的节点，而不是使用 `root2D`。这样组件更加独立和可复用。
