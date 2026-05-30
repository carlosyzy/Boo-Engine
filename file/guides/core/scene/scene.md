# Scene 场景系统设计文档

## 概述

Scene（场景）是 Boo 引擎中最核心的概念之一，它是所有内容的容器。一个 Scene 包含场景中的所有节点，包括 2D 节点和 3D 节点。

## 核心职责

- **层级管理**：维护 2D 和 3D 两套节点树结构
- **生命周期**：管理场景的启动、更新和销毁
- **变换传播**：通过世界矩阵变换确保子节点正确继承父节点的变换
- **组件管理**：支持在节点上添加各种组件

## 系统架构

### 节点类型

引擎提供两种类型的节点供使用：

- **Node2D**：用于 2D UI 渲染
- **Node3D**：用于 3D 场景渲染

## 核心概念

### 节点层级（ENodeLayer）

| 值 | 说明 |
|------|------|
| Default | 默认层级 |
| Node2D | 2D 节点层 |
| Node3D | 3D 节点层 |
| Scene | 场景层 |

### 组件层级（EComponentLayer）

组件层级与节点层级对应，确保组件只能添加到对应类型的节点上：

- Node2D 只能添加 Layer2D 组件
- Node3D 只能添加 Layer3D 组件

### 节点组（ENodeGroup）

节点组用于相机的可见性判断。

## 头文件

```cpp
#include "core/scene/scene.h"
```

## API 接口

### 构造函数

| 接口 | 说明 |
|------|------|
| `Scene(name, uuid = "")` | 创建场景 |

### 场景管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getRoot2D()` | 获取 2D 根节点 | - | Node2D* |
| `getRoot3D()` | 获取 3D 根节点 | - | Node3D* |

## 使用示例

### 创建启动场景

```cpp
auto *scene = new Boo::Scene("StartScene");
Boo::game->openScene(scene);
auto *root2D = scene->getRoot2D();
```
