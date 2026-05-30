# GroundGrid 地面网格组件

## 概述

GroundGrid（地面网格组件）用于在 3D 场景中显示地面网格。通过 GroundGrid 组件，可以创建一个带有网格纹理的平面，用于场景参考和调试。

### 组件层级

Layer3D 组件，只能添加到 Node3D 节点。

---

## 头文件

```cpp
#include "core/components/tool/ground-grid.h"
```

---

## 使用示例

```cpp
// ── 创建地面网格节点 ──────────────────────────────
Boo::Node3D *root3D = m_scene->getRoot3D();
auto *groundNode = new Boo::Node3D("GroundGrid");
groundNode->setPosition(0.0f, 0.0f, 0.0f);
root3D->addChild(groundNode);

// ── 添加 GroundGrid 组件 ──────────────────────────
groundNode->addComponent("GroundGrid");
```
