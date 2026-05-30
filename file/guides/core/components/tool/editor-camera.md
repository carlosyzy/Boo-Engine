# EditorCamera 编辑器相机组件

## 概述

EditorCamera（编辑器相机组件）用于在编辑器场景中提供视角控制。通过 EditorCamera 组件，可以实现鼠标滚轮缩放、中键拖拽旋转等相机操作。

### 组件层级

Default 组件，可以添加到 Node2D 或 Node3D 节点。

---

## 头文件

```cpp
#include "core/components/tool/editor-camera.h"
```

---

## 使用示例

```cpp
// ── 创建编辑器相机节点 ──────────────────────────────
Boo::Node3D *root3D = m_scene->getRoot3D();
auto *editorCameraNode = new Boo::Node3D("EditorCamera");
editorCameraNode->setPosition(0.0f, 10.0f, 10.0f);
editorCameraNode->setEulerAngles(-45.0f, 0.0f, 0.0f);
root3D->addChild(editorCameraNode);

// ── 添加 EditorCamera 组件 ────────────────────────
auto *editorCamera = dynamic_cast<Boo::EditorCamera *>(editorCameraNode->addComponent("EditorCamera"));
if (editorCamera)
{
    editorCamera->setFocusRadius(10.0f);
}
```
