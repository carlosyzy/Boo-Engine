# AudioAsset 音频资产

## 概述

AudioAsset（音频资产）用于存储音频数据，支持加载和播放音频文件。

### 资源类型

<code>EAssetType::Audio</code>

---

## 头文件

```cpp
#include "core/asset/audio-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `AudioAsset()` | 创建空音频资产 |
| `AudioAsset(uuid)` | 通过 UUID 创建音频资产 |
| `AudioAsset(uuid, path, name)` | 通过完整参数创建音频资产 |

#### 音频创建

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(audioBuffer)` | 通过音频数据创建音频资产 | audioBuffer: 音频数据缓冲区 | void |

#### 音频查询

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getTotalSeconds()` | 获取音频总时长（秒） | - | const float& |
| `getAudioBuffer()` | 获取音频数据缓冲区 | - | std::vector&lt;uint8_t&gt;& |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁音频资产 | - | void |
