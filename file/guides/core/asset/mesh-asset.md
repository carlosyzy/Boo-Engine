# MeshAsset 网格资产

## 概述

MeshAsset（网格资产）用于存储 3D 模型的网格数据，包括顶点位置、法线、UV 坐标、索引等信息。

---

## 头文件

```cpp
#include "core/asset/mesh-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `MeshAsset()` | 创建空网格资产 |
| `MeshAsset(uuid)` | 通过 UUID 创建网格资产 |
| `MeshAsset(uuid, path, name)` | 通过完整参数创建网格资产 |

#### 网格创建

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(primitives)` | 创建静态网格 | primitives: 网格原语列表 | void |
| `createDynamic(primitives)` | 创建动态网格 | primitives: 网格原语列表 | void |
| `updateDynamicSubMesh(index, data)` | 更新动态网格子网格 | index: 索引，data: 网格原语数据 | void |

#### 网格查询

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getSubMeshCount()` | 获取子网格数量 | - | size_t |
| `getGfxMesh(index)` | 获取指定索引的图形网格 | index: 索引 | GfxMesh* |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁网格资产 | - | void |
---

## 网格模式（EMeshMode）
| 值      | 说明    |
| ------ | ----- |
| `UI`   | UI 网格  |
| `Model` | 模型网格  |

---
## 网格类型（EMeshType）
| 值        | 说明    |
| -------- | ----- |
| `Static`  | 静态网格  |
| `Dynamic` | 动态网格  |
---

## 网格原语结构（FMeshPrimitive）

| 属性        | 类型            | 说明        |
| --------- | ------------- | --------- |
| `index`   | int           | 索引        |
| `type`    | EMeshType     | 网格类型      |
| `_positions` | vector<float> | 顶点位置     |
| `_normals`  | vector<float> | 顶点法线      |
| `_uvs`      | vector<float> | 顶点 UV 坐标  |
| `_uvs1`     | vector<float> | 顶点 UV1 坐标 |
| `_uvs2`     | vector<float> | 顶点 UV2 坐标 |
| `_colors`   | vector<float> | 顶点颜色     |
| `_tangents` | vector<float> | 顶点切线     |
| `_indices`   | vector<uint32_t> | 顶点索引   |
