# 输入系统表单定义

## 鼠标按下/释放/移动监听

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `func` | `std::function<void(FInputMouseEvent&)>` | 回调函数 | 是 |

**返回值**：`uint64_t` 监听器 ID

## 取消鼠标监听

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `id` | `uint64_t` | 监听器 ID | 是 |

## 鼠标滚轮/触控板滚动监听

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `func` | `std::function<void(FInputScrollEvent&)>` | 回调函数 | 是 |

**返回值**：`uint64_t` 监听器 ID

## FInputMouseEvent 字段

| 字段 | 类型 | 描述 |
|------|------|------|
| `button` | `int` | 鼠标按键（0=左键，1=右键，2=中键） |
| `x` | `double` | 鼠标 X 坐标 |
| `y` | `double` | 鼠标 Y 坐标 |

## FInputScrollEvent 字段

| 字段 | 类型 | 描述 |
|------|------|------|
| `xoffset` | `double` | 水平滚动量 |
| `yoffset` | `double` | 垂直滚动量 |
