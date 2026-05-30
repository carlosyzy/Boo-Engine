# MeshRenderer 网格渲染组件

## 概述

MeshRenderer（网格渲染组件）用于在 3D 场景中渲染网格模型。通过 MeshRenderer 组件，可以将 MeshAsset 网格资源和 MaterialAsset 材质资源绑定到节点上，实现模型的显示。

### 组件层级

Layer3D 组件，只能添加到 Node3D 节点。

---

## 头文件

```cpp
#include "core/renderer/3d/mesh-renderer.h"
```

---

## API 接口

#### 网格设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setMesh(meshUuid)` | 通过 UUID 设置网格资产 | meshUuid: 网格 UUID | void |
| `setMesh(meshAsset)` | 直接设置网格资产 | meshAsset: MeshAsset 指针 | void |

#### 材质设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setMaterial(materialName)` | 通过名称设置材质 | materialName: 材质名称 | void |
| `setMaterial(materialAsset)` | 直接设置材质资产 | materialAsset: MaterialAsset 指针 | void |
| `setMaterial(index, materialName)` | 通过索引设置子材质 | index: 索引，materialName: 材质名称 | void |
| `setMaterial(index, materialAsset)` | 通过索引设置子材质 | index: 索引，materialAsset: MaterialAsset 指针 | void |
| `getMaterial()` | 获取主材质 | - | MaterialAsset* |
| `getMaterial(index)` | 通过索引获取材质 | index: 索引 | MaterialAsset* |
| `getMaterials()` | 获取所有材质 | - | vector<MaterialAsset*> |

#### 纹理设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setTexture(path)` | 通过路径设置纹理 | path: 纹理路径 | void |
| `setTexture(texture)` | 直接设置纹理 | texture: TextureAsset 指针 | void |

---

## 使用示例

```cpp
// ── 创建 3D 节点 ─────────────────────────────────────
Boo::Node3D *root3D = m_scene->getRoot3D();
auto *cylinderNode = new Boo::Node3D("Cylinder");
cylinderNode->setPosition(0.0f, 0.0f, 0.0f);
root3D->addChild(cylinderNode);

// ── 添加 MeshRenderer 组件 ──────────────────────────
auto *meshRenderer = dynamic_cast<Boo::MeshRenderer *>(cylinderNode->addComponent("MeshRenderer"));
if (meshRenderer)
{
    meshRenderer->setMesh("builtin/mesh/cylinder");
    meshRenderer->setMaterial("builtin/material/default");
}
```
