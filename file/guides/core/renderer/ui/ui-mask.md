# UIMask UI 遮罩组件

## 概述

UIMask（UI 遮罩组件）用于在 2D 界面上创建遮罩效果。通过 UIMask 组件，可以对子元素进行遮罩处理，实现圆形、圆角等特殊显示效果。

### 组件层级

Layer2D 组件，只能添加到 Node2D 节点。

---

## 头文件

```cpp
#include "core/renderer/ui/ui-mask.h"
```

---

## 使用示例

```cpp
auto *bgNode = new Boo::Node2D("bg");
bgNode->setSize(720.0f, 1280.0f);
_node2d->addChild(bgNode);
bgNode->addComponent("UIMask");
```
