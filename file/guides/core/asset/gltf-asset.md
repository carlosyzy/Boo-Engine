# GLTFAsset GLTF 资产

## 概述

GLTFAsset（GLTF 资产）用于加载和解析 GLTF 格式的 3D 模型文件，将 GLTF 数据转换为引擎可用的 MeshAsset。

---

## 头文件

```cpp
#include "core/asset/gltf-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `GLTFAsset()` | 创建空 GLTF 资产 |
| `GLTFAsset(uuid)` | 通过 UUID 创建 GLTF 资产 |
| `GLTFAsset(uuid, path, name)` | 通过完整参数创建 GLTF 资产 |

#### GLTF 解析

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(m_asset)` | 解析 GLTF 数据 | m_asset: fastgltf Asset 指针 | void |
| `getRoot()` | 获取 GLTF 根节点 | - | const GLTFNode& |
| `getMeshs()` | 获取所有网格资产 | - | map<string, MeshAsset*>& |
| `getMesh(meshName)` | 获取指定名称的网格资产 | meshName: 网格名称 | MeshAsset* |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁 GLTF 资产 | - | void |

---

## GLTF 节点结构（GLTFNode）

| 属性        | 类型     | 说明        |
| --------- | ------ | --------- |
| `name`    | string | 节点名称     |
| `local`   | Mat4   | 本地变换矩阵   |
| `path`    | string | 相对场景的路径 |
| `meshUuid` | string | 网格 UUID  |
| `children` | vector<GLTFNode> | 子节点列表   |
