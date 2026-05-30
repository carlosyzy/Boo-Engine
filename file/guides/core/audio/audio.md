# Audio 音频实例

## 概述

Audio（音频实例）是引擎中用于控制单个音频播放的类。通过 AudioManager 播放音乐或音效后，会返回一个 Audio 指针，用于控制该音频的播放状态、音量等属性。

---

## 头文件

```cpp
#include "core/audio/audio.h"
```

---

## 枚举类型

### 音频状态（EAudioState）

| 值 | 说明 |
|------|------|
| None | 无状态 |
| Initialized | 已初始化 |
| Playing | 正在播放 |
| Paused | 已暂停 |
| Stopped | 已停止 |

### 音频类型（EAudioType）

| 值 | 说明 |
|------|------|
| Music | 音乐 |
| Sound | 音效 |

---

## API 接口

### 播放控制

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `play(loop)` | 开始/继续播放 | loop: 是否循环 | void |
| `pause()` | 暂停播放 | - | void |
| `resume()` | 继续播放 | - | void |
| `stop()` | 停止播放 | - | void |

### 音量控制

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setVolume(volume)` | 设置音量 | volume: 音量值（0-1） | void |
| `getVolume()` | 获取音量 | - | float |

### 状态查询

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getState()` | 获取当前状态 | - | EAudioState |
| `getId()` | 获取音频 ID | - | const int |

### 更新与销毁

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `update(dt)` | 每帧更新 | dt: 时间增量 | void |
| `destroy()` | 销毁音频实例 | - | void |

---

## 使用示例

```cpp
// ── 播放背景音乐 ─────────────────────────────────────
auto *bgMusic = Boo::audioManager->playMusic("audio/bgm.mp3", true);
if (bgMusic)
{
    bgMusic->setVolume(0.5f);
}

// ── 暂停音乐 ─────────────────────────────────────────
bgMusic->pause();

// ── 继续播放 ─────────────────────────────────────────
bgMusic->resume();

// ── 停止音乐 ─────────────────────────────────────────
bgMusic->stop();

// ── 播放音效 ─────────────────────────────────────────
auto *clickSound = Boo::audioManager->playSound("audio/click.wav");
if (clickSound)
{
    clickSound->setVolume(1.0f);
}
```