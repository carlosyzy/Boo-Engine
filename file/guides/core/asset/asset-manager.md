# AssetManager 资产管理器

## 概述

AssetManager（资产管理器）是引擎中统一管理所有资源的模块，负责资源的加载、缓存和卸载。

---

## 头文件

```cpp
#include "core/asset/asset-manager.h"
```

---

## 支持的资源类型

| 扩展名      | 说明       |
| --------- | -------- |
| `.png`    | PNG 纹理    |
| `.jpg`    | JPG 纹理    |
| `.jpeg`   | JPEG 纹理   |
| `.vert`   | 顶点着色器   |
| `.frag`   | 片段着色器   |
| `.mtl`    | 材质文件    |
| `.glb`    | GLB 模型   |

---

## 资源类型（EAssetType）

| 值          | 说明       |
| ---------- | -------- |
| `None`     | 无类型      |
| `Texture`  | 纹理资源     |
| `Audio`    | 音频资源     |
| `Font`     | 字体资源     |
| `Shader`   | 着色器资源    |
| `Material` | 材质资源     |
| `Scene`    | 场景资源     |
| `GLTF`     | GLTF 模型资源 |
| `Mesh`     | 网格资源     |

---

## 网格模式（EMeshMode）

| 值       | 说明       |
| ------- | -------- |
| `UI`    | UI 网格    |
| `Model` | 模型网格    |

---

## 网格类型（EMeshType）

| 值        | 说明       |
| -------- | -------- |
| `Static` | 静态网格    |
| `Dynamic`| 动态网格    |

---

## 纹理类型（ETextureType）

| 值        | 说明       |
| -------- | -------- |
| `Static` | 静态纹理    |
| `Dynamic`| 动态纹理    |

---

## 渲染器层（ERendererLayer）

| 值       | 说明       |
| ------- | -------- |
| `UI`    | UI 渲染器层 |
| `Model` | 模型渲染器层 |

---

## 渲染器类型（ERendererType）

| 值           | 说明         |
| ----------- | ---------- |
| `Opaque`    | 不透明渲染器   |
| `Transparent`| 透明渲染器    |

---

## API 接口

#### 资源加载

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `loadAsset(uuid)` | 加载指定 UUID 的资源 | uuid: 资源 UUID | Asset* |
| `getAsset(uuid, loadIfNotFound)` | 获取指定 UUID 的资源 | uuid: 资源 UUID，loadIfNotFound: 未找到时是否加载 | Asset* |

#### 资源卸载

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `unloadAsset(uuid)` | 卸载指定 UUID 的资源 | uuid: 资源 UUID | void |

---

## 资源列表

- MeshAsset 网格资产
- MaterialAsset 材质资产
- ShaderAsset 着色器资产
- TextureAsset 纹理资产
- GLTFAsset GLTF 资产
