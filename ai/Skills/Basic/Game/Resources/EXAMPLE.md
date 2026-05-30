# Game 类使用示例

## 场景管理示例

### 创建和打开场景

```cpp
#include "engine/boo.h"

// 创建并打开场景
void openMainScene() {
    Boo::Scene* scene = new Boo::Scene("MainScene");
    // 打开场景（new Scene后必须紧接着调用open）
    Boo::game->openScene(scene);
    // 根节点由引擎自动创建，之后可通过 scene->getRoot2D() 获取
}

// 获取当前打开场景
Boo::Scene* getCurrentScene() {
    return Boo::game->getScene();
}
```

## 调度系统示例

### 重复执行的定时任务

```cpp
#include "engine/boo.h"

class GameManager {
public:
    GameManager() {
        // 每帧执行一次更新
        _updateId = Boo::game->schedule(&GameManager::update, this, 0.0f);

        // 每2秒执行一次定时任务
        _timerId = Boo::game->schedule(&GameManager::onTimer, this, 2.0f);
    }

    ~GameManager() {
        // 取消定时任务
        Boo::game->unschedule(_updateId);
        Boo::game->unschedule(_timerId);
    }

    void update() {
        // 每帧更新逻辑
    }

    void onTimer() {
        // 定时执行的逻辑
        LOGI("Timer triggered!");
    }

private:
    int _updateId;
    int _timerId;
};

// 使用示例
GameManager* gameManager = new GameManager();
```

### 一次性定时任务

```cpp
#include "engine/boo.h"

class Countdown {
public:
    void startCountdown() {
        // 5秒后执行一次
        Boo::game->scheduleOnce(&Countdown::onCountdownEnd, this, 5.0f);
        LOGI("Countdown started!");
    }

    void onCountdownEnd() {
        LOGI("Countdown ended!");
    }
};

// 使用示例
Countdown countdown;
countdown.startCountdown();
```

## 视图调整示例

### 处理窗口大小变化

```cpp
#include "engine/boo.h"

// 窗口大小变化回调
void onWindowSizeChanged(GLFWwindow* window, int width, int height) {
    Boo::game->resizeView(width, height);
}

// 注册窗口大小变化回调（以GLFW为例）
void setupWindowSizeCallback(GLFWwindow* window) {
    glfwSetWindowSizeCallback(window, onWindowSizeChanged);
}
```

## 完整游戏示例

### 简单的2D游戏示例

```cpp
#include "engine/boo.h"

class SimpleGame {
public:
    void init() {
        // 创建场景
        _scene = new Boo::Scene("GameScene");
        // 打开场景（new Scene后必须紧接着调用open）
        Boo::game->openScene(_scene);

        // 获取 2D 根节点（引擎自动创建）
        Boo::Node2D* root2D = _scene->getRoot2D();

        // 创建玩家节点
        _player = new Boo::Node2D("Player");
        _player->setPosition(100, 100, 0);
        _player->setSize(50, 50);
        root2D->addChild(_player);

        // 为玩家添加精灵组件
        _playerSprite = dynamic_cast<Boo::UISprite*>(_player->addComponent("UISprite"));
        if (_playerSprite) {
            _playerSprite->setColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
            _playerSprite->setTexture(Boo::AssetBuiltinTexture::Default);
        }

        // 创建敌人节点
        _enemy = new Boo::Node2D("Enemy");
        _enemy->setPosition(300, 100, 0);
        _enemy->setSize(50, 50);
        root2D->addChild(_enemy);

        // 为敌人添加精灵组件
        _enemySprite = dynamic_cast<Boo::UISprite*>(_enemy->addComponent("UISprite"));
        if (_enemySprite) {
            _enemySprite->setColor(0.0f, 0.0f, 1.0f, 1.0f); // 蓝色
            _enemySprite->setTexture(Boo::AssetBuiltinTexture::Default);
        }

        // 设置调度任务（每帧更新）
        _updateId = Boo::game->schedule(&SimpleGame::update, this, 0.0f);
    }

    void update() {
        // 更新玩家位置
        Boo::Vec3 playerPos = _player->getPosition();
        playerPos.x += 1.0f;
        if (playerPos.x > 800) {
            playerPos.x = 0;
        }
        _player->setPosition(playerPos.x, playerPos.y, playerPos.z);

        // 更新敌人位置
        Boo::Vec3 enemyPos = _enemy->getPosition();
        enemyPos.x -= 1.0f;
        if (enemyPos.x < 0) {
            enemyPos.x = 800;
        }
        _enemy->setPosition(enemyPos.x, enemyPos.y, enemyPos.z);
    }

private:
    Boo::Scene* _scene;
    Boo::Node2D* _player;
    Boo::Node2D* _enemy;
    Boo::UISprite* _playerSprite;
    Boo::UISprite* _enemySprite;
    int _updateId;
};

int main() {
    // 创建窗口
    Boo::Window* window = new Boo::Window();
    window->init();

    // 初始化游戏
    Boo::game->init(window, 800, 600, Boo::UIDesignFitMode::None);

    // 初始化游戏逻辑（在引擎启动事件里）
    Boo::event->on(Boo::EventEngine_Launch, []() {
        static SimpleGame game;
        game.init();
    });

    // 游戏主循环
    while (!window->shouldClose()) {
        window->pollEvents();
        Boo::game->tick();
        window->swapBuffers();
    }

    return 0;
}
```
