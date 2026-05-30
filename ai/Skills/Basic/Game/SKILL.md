---
name: "game"
description: "Boo引擎的核心游戏实例，负责游戏的初始化、更新、渲染和场景管理。作为引擎对外的核心全局实例，提供游戏引擎的全自动运行和用户控制接口。"
---

# Game 类

Game 类是 Boo 引擎的核心游戏实例，负责游戏的初始化、更新、渲染和场景管理。作为引擎对外的核心全局实例，它提供了游戏引擎的全自动运行和用户控制接口。

## 核心功能

### 初始化与启动
- **初始化游戏**：初始化各个引擎模块，包括图形系统、事件系统、输入系统等
- **平台支持**：支持窗口和Android平台的初始化
- **视图配置**：支持设置设计分辨率和适配模式

### 场景管理
- **场景切换**：打开新场景，销毁当前场景
- **场景访问**：获取当前场景实例

### 更新与渲染
- **游戏循环**：管理游戏的更新和渲染循环
- **时间管理**：处理游戏时间和帧率
- **调度系统**：支持定时任务和回调

### 输入处理
- **鼠标状态**：更新鼠标按钮状态和位置
- **键盘状态**：更新键盘按键状态

### 资源管理
- **组件清理**：管理组件的清理
- **节点清理**：管理节点的清理

## 主要接口

### 初始化
```cpp
// 初始化引擎（桌面端）
void init(Window* window, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode);

// 设置/获取目标帧率
void setFrameRate(int frameRate);
int getFrameRate();
int getFps();
```

### 场景管理
```cpp
// 获取当前场景
Scene *getScene();

// 打开新场景
void openScene(Scene *scene);

// 销毁当前场景
void destroyScene();
```

### 视图管理
```cpp
// 调整视图大小
void resizeView(const int width, const int height);
```

### 调度系统
```cpp
// 安排定时任务（重复执行）
template <typename T, typename Func>
int schedule(Func func, T *instance, float interval);

// 安排定时任务（执行一次）
template <typename T, typename Func>
int scheduleOnce(Func func, T *instance, float interval);

// 取消定时任务
void unschedule(int scheduleID);
```

### 场景管理

```cpp
// 创建场景
Boo::Scene* scene = new Boo::Scene("MainScene");

// 打开场景
Boo::game->openScene(scene);

// 获取当前场景
Boo::Scene* currentScene = Boo::game->getScene();

// 销毁当前场景
Boo::game->destroyScene();
```

### 使用调度系统

```cpp
class GameManager {
public:
    void update() {
        // 游戏逻辑更新
    }
    
    void onTimer() {
        // 定时执行的逻辑
    }
};

GameManager* gameManager = new GameManager();

// 每帧执行一次
int updateID = Boo::game->schedule(&GameManager::update, gameManager, 0.0f);

// 每2秒执行一次
int timerID = Boo::game->schedule(&GameManager::onTimer, gameManager, 2.0f);

// 5秒后执行一次
int onceID = Boo::game->scheduleOnce(&GameManager::onTimer, gameManager, 5.0f);

// 取消定时任务
Boo::game->unschedule(timerID);
```

## 总结

Game 类是 Boo 引擎的核心入口，提供了游戏开发所需的主要功能。通过 Game 类，你可以：

- 初始化和启动游戏
- 管理游戏场景
- 处理游戏更新和渲染
- 管理定时任务
- 处理输入事件
- 管理资源生命周期

Game 类是连接引擎各个模块的中心，是构建完整游戏应用的基础。通过合理使用 Game 类的接口，你可以创建出功能完整、性能优良的游戏应用。