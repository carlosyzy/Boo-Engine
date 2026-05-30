# GLTFComponent GLTF 组件

## 概述

GLTFComponent（GLTF 组件）用于加载和解析 GLTF 格式的 3D 模型文件。通过 GLTFComponent 组件，可以将 GLTF 文件解析为节点层级结构，自动创建子节点并挂载 MeshRenderer 组件。

### 组件层级

Layer3D 组件，只能添加到 Node3D 节点。

---

## 头文件

```cpp
#include "core/components/3d/gltf-component.h"
```

---

## API 接口

#### GLTF 资源设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setGLTFAsset(gltfAssetPath)` | 通过路径设置 GLTF 资产 | gltfAssetPath: GLTF 资产路径 | void |
| `setGLTFAsset(gltfAsset)` | 直接设置 GLTF 资产 | gltfAsset: GLTFAsset 指针 | void |

#### 材质设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setMaterial(meshNodePath, meshIndex, materialPath)` | 通过路径设置指定网格节点的材质 | meshNodePath: 网格节点路径，meshIndex: 网格索引，materialPath: 材质路径 | void |
| `setMaterial(meshNodePath, meshIndex, material)` | 直接设置指定网格节点的材质 | meshNodePath: 网格节点路径，meshIndex: 网格索引，material: MaterialAsset 指针 | void |

#### 纹理设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setTexture(meshNodePath, texturePath)` | 设置指定网格节点的纹理 | meshNodePath: 网格节点路径，texturePath: 纹理路径 | void |
| `setTexture(meshNodePath, texture)` | 直接设置指定网格节点的纹理 | meshNodePath: 网格节点路径，texture: TextureAsset 指针 | void |

---

## 使用示例

```cpp
// ── 创建 GLTF 节点 ─────────────────────────────────
Boo::Node3D *root3D = m_scene->getRoot3D();
auto *gltfNode = new Boo::Node3D("Model");
gltfNode->setPosition(0.0f, 0.0f, 0.0f);
root3D->addChild(gltfNode);

// ── 添加 GLTFComponent 组件 ───────────────────────
auto *gltfComponent = dynamic_cast<Boo::GLTFComponent *>(gltfNode->addComponent("GLTFComponent"));
if (gltfComponent)
{
    gltfComponent->setGLTFAsset("models/robot.gltf");
}
```
