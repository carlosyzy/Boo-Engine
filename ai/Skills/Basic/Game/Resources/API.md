# Game 类核心 API 文档

## 初始化

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `init(Window *window, int uiDesignWidth, int uiDesignHeight, UIDesignFitMode fitMode)` | `void` | 初始化引擎（桌面端） |
| `setFrameRate(int frameRate)` | `void` | 设置目标帧率 |
| `getFrameRate()` | `int` | 获取目标帧率 |
| `getFps()` | `int` | 获取当前实际帧率 |

## 场景管理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `getScene()` | `Scene*` | 获取当前场景 |
| `openScene(Scene *scene)` | `void` | 打开指定场景 |
| `destroyScene()` | `void` | 销毁当前场景 |

## 视图管理

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `resizeView(const int width, const int height)` | `void` | 调整视图大小 |

## 调度系统

| 方法 | 返回类型 | 描述 |
|------|----------|------|
| `schedule(Func func, T *instance, float interval)` | `int` | 注册定时重复任务，返回任务 ID |
| `scheduleOnce(Func func, T *instance, float interval)` | `int` | 注册定时单次任务，返回任务 ID |
| `unschedule(int scheduleID)` | `void` | 取消指定 ID 的定时任务 |

## 枚举类型

### UIDesignFitMode

| 值 | 描述 |
|----|------|
| `None` | 不适配 |
| `Width` | 按宽度适配 |
| `Height` | 按高度适配 |
