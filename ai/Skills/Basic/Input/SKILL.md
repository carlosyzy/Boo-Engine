---
name: "input"
description: "输入系统，处理鼠标和触控板输入事件。"
---

# 输入系统

输入系统负责处理玩家的鼠标和触控板输入。通过注册回调函数来响应输入事件，而不是轮询输入状态。

## 核心功能

### 鼠标输入
- **鼠标按键事件**：监听鼠标按下和释放
- **鼠标移动事件**：监听鼠标移动
- **鼠标滚轮事件**：监听鼠标滚轮滚动

### 触控板输入
- **触控板滚动事件**：监听触控板滚动

## 主要接口

### 鼠标事件监听
```cpp
// 注册鼠标按下监听，返回监听器 ID
uint64_t onMouseDown(std::function<void(FInputMouseEvent&)> func);

// 注册鼠标释放监听，返回监听器 ID
uint64_t onMouseUp(std::function<void(FInputMouseEvent&)> func);

// 注册鼠标移动监听，返回监听器 ID
uint64_t onMouseMove(std::function<void(FInputMouseEvent&)> func);

// 取消鼠标事件监听
void offMouse(uint64_t id);
```

### 鼠标滚轮事件监听
```cpp
// 注册鼠标滚轮监听，返回监听器 ID
uint64_t onMouseScroll(std::function<void(FInputScrollEvent&)> func);

// 注册触控板滚动监听，返回监听器 ID
uint64_t onTouchpadScroll(std::function<void(FInputScrollEvent&)> func);

// 取消鼠标滚轮监听
void offMouseScroll(uint64_t id);

// 取消触控板滚动监听
void offTouchpadScroll(uint64_t id);
```

## 事件结构体

### FInputMouseEvent

| 字段 | 类型 | 描述 |
|------|------|------|
| `button` | `int` | 鼠标按键（0=左键，1=右键，2=中键） |
| `x` | `double` | 鼠标 X 坐标 |
| `y` | `double` | 鼠标 Y 坐标 |

### FInputScrollEvent

| 字段 | 类型 | 描述 |
|------|------|------|
| `xoffset` | `double` | 水平滚动量 |
| `yoffset` | `double` | 垂直滚动量 |

## 使用示例

### 监听鼠标点击

```cpp
#include "engine/boo.h"

void setupInput() {
    Boo::game->input->onMouseDown([](Boo::FInputMouseEvent& e) {
        if (e.button == 0) {
            LOGI("鼠标左键按下，位置: (%.1f, %.1f)", e.x, e.y);
        }
    });
}
```

### 监听鼠标移动

```cpp
Boo::game->input->onMouseMove([](Boo::FInputMouseEvent& e) {
    LOGI("鼠标移动到: (%.1f, %.1f)", e.x, e.y);
});
```

### 监听滚轮

```cpp
Boo::game->input->onMouseScroll([](Boo::FInputScrollEvent& e) {
    LOGI("滚轮滚动: xoffset=%.1f, yoffset=%.1f", e.xoffset, e.yoffset);
});
```

## 最佳实践

1. **保存监听器 ID**：保存 `onMouseDown` 等方法返回的 ID，以便在不需要时调用 `offMouse` 取消
2. **避免重复注册**：每次调用 `onMouseDown` 等都会新增一个监听器，注意避免重复注册
3. **及时取消监听**：对象销毁前通过 `offMouse(id)` 取消监听，防止访问悬空指针

## 常见问题

- **回调未触发**：确认注册时机，需要在引擎启动后（`EventEngine_Launch` 之后）注册
- **坐标系**：鼠标坐标为屏幕像素坐标，原点在左上角

## 总结

输入系统通过事件回调模式处理鼠标和触控板输入。通过 `onMouseDown/Up/Move` 和 `onMouseScroll/onTouchpadScroll` 注册回调，通过对应的 `offMouse/offMouseScroll/offTouchpadScroll` 取消监听。
