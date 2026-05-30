# 事件系统 API

## Event 类

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `on(const std::string &eventName, std::function<void()> call)` | 注册事件监听 | `eventName`: 事件名称<br>`call`: 回调函数 | `uint64_t`: 监听器 ID |
| `off(uint64_t id)` | 取消指定 ID 的监听 | `id`: 监听器 ID | `void` |
| `off(const std::string &eventName)` | 取消某事件的所有监听 | `eventName`: 事件名称 | `void` |
| `emit(const std::string &eventName)` | 触发事件 | `eventName`: 事件名称 | `void` |

## 内置事件名称

| 常量 | 值 | 描述 |
|------|----|------|
| `Boo::EventEngine_Launch` | `"Launch"` | 引擎启动完成事件 |
