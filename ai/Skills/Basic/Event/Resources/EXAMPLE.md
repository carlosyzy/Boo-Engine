# 事件系统示例

## 示例 1: 监听引擎启动事件

```cpp
#include "engine/boo.h"

class GameApp {
public:
    void init() {
        // 监听引擎启动完成事件
        Boo::event->on(Boo::EventEngine_Launch, [this]() {
            this->onLaunch();
        });
    }

    void onLaunch() {
        LOGI("引擎启动完成，开始创建场景");

        Boo::Scene* scene = new Boo::Scene("MainScene");
        Boo::game->openScene(scene);
    }
};
```

## 示例 2: 自定义游戏事件

```cpp
#include "engine/boo.h"

class GameManager {
private:
    uint64_t _playerDiedListenerId = 0;

public:
    void init() {
        // 监听玩家死亡事件
        _playerDiedListenerId = Boo::event->on("PlayerDied", [this]() {
            LOGI("玩家死亡，显示游戏结束界面");
            this->showGameOver();
        });
    }

    void onPlayerHealthZero() {
        // 触发玩家死亡事件
        Boo::event->emit("PlayerDied");
    }

    void showGameOver() {
        // 显示游戏结束 UI ...
    }

    void cleanup() {
        // 取消监听
        Boo::event->off(_playerDiedListenerId);
    }
};
```

## 示例 3: 取消某事件的全部监听

```cpp
#include "engine/boo.h"

void cleanupSceneEvents() {
    // 取消场景内注册的所有 "EnemySpawn" 监听者
    Boo::event->off("EnemySpawn");
    LOGI("已取消所有 EnemySpawn 事件监听");
}
```

## 示例 4: 多个监听者

```cpp
#include "engine/boo.h"

class AudioManager {
public:
    void init() {
        Boo::event->on("GameStart", []() {
            LOGI("AudioManager: 播放背景音乐");
        });
    }
};

class UIManager {
public:
    void init() {
        Boo::event->on("GameStart", []() {
            LOGI("UIManager: 隐藏开始菜单");
        });
    }
};

// 触发 "GameStart"，AudioManager 和 UIManager 的回调都会执行
Boo::event->emit("GameStart");
```
