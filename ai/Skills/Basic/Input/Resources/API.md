# 输入系统 API

## Input 类

### 鼠标事件监听

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `onMouseDown(std::function<void(FInputMouseEvent&)> func)` | 注册鼠标按下监听 | `func`: 回调函数 | `uint64_t`: 监听器 ID |
| `onMouseUp(std::function<void(FInputMouseEvent&)> func)` | 注册鼠标释放监听 | `func`: 回调函数 | `uint64_t`: 监听器 ID |
| `onMouseMove(std::function<void(FInputMouseEvent&)> func)` | 注册鼠标移动监听 | `func`: 回调函数 | `uint64_t`: 监听器 ID |
| `offMouse(uint64_t id)` | 取消鼠标事件监听 | `id`: 监听器 ID | `void` |

### 鼠标滚轮事件监听

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `onMouseScroll(std::function<void(FInputScrollEvent&)> func)` | 注册鼠标滚轮监听 | `func`: 回调函数 | `uint64_t`: 监听器 ID |
| `onTouchpadScroll(std::function<void(FInputScrollEvent&)> func)` | 注册触控板滚动监听 | `func`: 回调函数 | `uint64_t`: 监听器 ID |
| `offMouseScroll(uint64_t id)` | 取消鼠标滚轮监听 | `id`: 监听器 ID | `void` |
| `offTouchpadScroll(uint64_t id)` | 取消触控板滚动监听 | `id`: 监听器 ID | `void` |

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
