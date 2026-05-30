# 输入系统示例

## 示例 1: 鼠标点击检测

```cpp
#include "engine/boo.h"

class MyGame {
private:
    uint64_t _mouseDownId = 0;
    uint64_t _mouseUpId = 0;

public:
    void init() {
        _mouseDownId = Boo::game->input->onMouseDown([](Boo::FInputMouseEvent& e) {
            if (e.button == 0) {
                LOGI("鼠标左键按下，位置: (%.1f, %.1f)", e.x, e.y);
            } else if (e.button == 1) {
                LOGI("鼠标右键按下，位置: (%.1f, %.1f)", e.x, e.y);
            }
        });

        _mouseUpId = Boo::game->input->onMouseUp([](Boo::FInputMouseEvent& e) {
            LOGI("鼠标按键释放: button=%d", e.button);
        });
    }

    void cleanup() {
        Boo::game->input->offMouse(_mouseDownId);
        Boo::game->input->offMouse(_mouseUpId);
    }
};
```

## 示例 2: 鼠标移动追踪

```cpp
#include "engine/boo.h"

class MouseTracker {
private:
    uint64_t _mouseMoveId = 0;
    double _mouseX = 0;
    double _mouseY = 0;

public:
    void init() {
        _mouseMoveId = Boo::game->input->onMouseMove([this](Boo::FInputMouseEvent& e) {
            _mouseX = e.x;
            _mouseY = e.y;
        });
    }

    double getMouseX() const { return _mouseX; }
    double getMouseY() const { return _mouseY; }

    void cleanup() {
        Boo::game->input->offMouse(_mouseMoveId);
    }
};
```

## 示例 3: 鼠标滚轮缩放

```cpp
#include "engine/boo.h"

class CameraController {
private:
    uint64_t _scrollId = 0;
    float _zoom = 1.0f;

public:
    void init() {
        _scrollId = Boo::game->input->onMouseScroll([this](Boo::FInputScrollEvent& e) {
            _zoom += static_cast<float>(e.yoffset) * 0.1f;
            if (_zoom < 0.1f) _zoom = 0.1f;
            if (_zoom > 10.0f) _zoom = 10.0f;
            LOGI("缩放比例: %.2f", _zoom);
        });
    }

    void cleanup() {
        Boo::game->input->offMouseScroll(_scrollId);
    }
};
```

## 示例 4: 触控板滚动

```cpp
#include "engine/boo.h"

class ScrollView {
private:
    uint64_t _touchpadScrollId = 0;
    float _scrollX = 0;
    float _scrollY = 0;

public:
    void init() {
        _touchpadScrollId = Boo::game->input->onTouchpadScroll([this](Boo::FInputScrollEvent& e) {
            _scrollX += static_cast<float>(e.xoffset);
            _scrollY += static_cast<float>(e.yoffset);
            LOGI("滚动位置: (%.1f, %.1f)", _scrollX, _scrollY);
        });
    }

    void cleanup() {
        Boo::game->input->offTouchpadScroll(_touchpadScrollId);
    }
};
```
