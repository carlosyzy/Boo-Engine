# TextureAsset 纹理资产

## 概述

TextureAsset（纹理资产）用于存储纹理图像数据，包括宽度、高度、通道数、像素数据等信息。

---

## 头文件

```cpp
#include "core/asset/texture-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `TextureAsset()` | 创建空纹理资产 |
| `TextureAsset(uuid)` | 通过 UUID 创建纹理资产 |
| `TextureAsset(uuid, path, name)` | 通过完整参数创建纹理资产 |

#### 纹理创建

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(width, height, channels, pixelsVector, format)` | 通过像素数据创建纹理 | width: 宽度，height: 高度，channels: 通道数，pixelsVector: 像素数据，format: 纹理格式 | void |
| `create(data, size)` | 通过数据创建纹理 | data: 数据指针，size: 数据大小 | void |

#### 纹理查询

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getWidth()` | 获取纹理宽度 | - | const int |
| `getHeight()` | 获取纹理高度 | - | const int |
| `channels()` | 获取纹理通道数 | - | int |
| `getGfxTexture()` | 获取图形纹理 | - | GfxTexture* |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁纹理资产 | - | void |
