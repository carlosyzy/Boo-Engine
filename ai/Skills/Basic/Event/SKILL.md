---
name: "event"
description: "事件系统，处理引擎和游戏事件的监听与触发。"
---

# 事件系统

事件系统负责处理引擎和游戏事件的监听与触发，允许不同系统之间进行松耦合的通信。通过事件系统，你可以监听事件并在需要时触发事件。

## 核心功能

### 事件管理
- **事件监听**：注册回调函数监听指定事件
- **事件触发**：触发事件，通知所有监听者
- **监听取消**：通过返回的监听器 ID 或事件名称取消监听

## 主要接口

### 事件监听
```cpp
// 注册事件监听，返回监听器 ID
uint64_t on(const std::string& eventName, std::function<void()> callback);

// 取消指定 ID 的监听
void off(uint64_t id);

// 取消某事件的所有监听
void off(const std::string& eventName);
```

### 事件触发
```cpp
// 触发事件
void emit(const std::string& eventName);
```

## 内置事件

| 常量 | 值 | 描述 |
|------|----|------|
| `Boo::EventEngine_Launch` | `"Launch"` | 引擎启动完成事件 |

## 使用示例

### 基本用法

```cpp
#include "engine/boo.h"

void setupEvents() {
    // 监听事件，保存 ID 以便后续取消
    uint64_t listenerId = Boo::event->on("GameStart", []() {
        LOGI("游戏开始！");
    });

    // 触发事件
    Boo::event->emit("GameStart");

    // 通过 ID 取消监听
    Boo::event->off(listenerId);
}
```

### 监听引擎启动事件

```cpp
#include "engine/boo.h"

void onLaunch() {
    LOGI("引擎已启动，开始初始化游戏逻辑");
    // 创建场景等初始化工作
}

void registerLaunchEvent() {
    Boo::event->on(Boo::EventEngine_Launch, onLaunch);
}
```

### 取消某事件的所有监听

```cpp
// 取消 "GameOver" 事件的所有监听者
Boo::event->off("GameOver");
```

## 最佳实践

1. **事件命名**：使用清晰、描述性的事件名称，如 "GameStart"、"PlayerDied" 等
2. **及时取消监听**：不再需要的事件监听应通过 `off(id)` 取消，避免悬空回调
3. **事件粒度**：事件粒度应适中，避免过于细粒度或过于粗粒度的事件

## 常见问题

- **事件未被触发**：检查事件名称是否与 `emit` 中的名称完全一致
- **回调未执行**：检查 `on` 是否在 `emit` 之前调用
- **重复监听**：每次调用 `on` 都会注册一个新的监听器，注意避免重复注册

## 总结

事件系统是 Boo 引擎中实现系统间通信的重要机制，提供了 `on/off/emit` 接口来管理事件的监听和触发。通过事件系统，不同系统之间可以通过事件进行通信，而不需要直接依赖彼此，从而提高代码的模块化和可扩展性。
