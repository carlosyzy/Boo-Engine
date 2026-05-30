# AudioManager 音频管理器

## 概述

AudioManager（音频管理器）是引擎中统一管理音频播放的模块，负责音频的加载、播放和控制。

---

## 头文件

```cpp
#include "core/audio/audio-manager.h"
```

---

## API 接口

#### 初始化

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `init()` | 初始化音频系统 | - | void |

#### 音乐播放

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `playMusic(audioPath, loop)` | 通过路径播放音乐 | audioPath: 音频路径，loop: 是否循环 | Audio* |
| `playMusic(audioAsset, loop)` | 通过音频资产播放音乐 | audioAsset: AudioAsset 指针，loop: 是否循环 | Audio* |

#### 音效播放

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `playSound(audioPath)` | 通过路径播放音效 | audioPath: 音频路径 | void |
| `playSound(audioAsset)` | 通过音频资产播放音效 | audioAsset: AudioAsset 指针 | void |

#### 更新与销毁

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `update(dt)` | 每帧更新 | dt: 时间增量 | void |
| `destroy()` | 销毁音频管理器 | - | void |

---

## 使用示例

```cpp
// ── 播放背景音乐 ─────────────────────────────────────
auto *bgMusic = Boo::audioManager->playMusic("audio/bgm.mp3", true);
if (bgMusic)
{
    bgMusic->setVolume(0.5f);
}

// ── 播放音效 ────────────────────────────────────────
Boo::audioManager->playSound("audio/click.wav");
```
