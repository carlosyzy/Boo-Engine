# Camera 相机组件

## 概述

Camera（相机组件）是场景中用于渲染视图的组件。每个相机对应一个离屏渲染目标（RenderTexture），将可见物体渲染到贴图上，然后再将贴图渲染到屏幕上。

### 组件层级

Default 组件，可以添加到 Node2D 或 Node3D 节点。

***

## 头文件

```cpp
#include "core/renderer/camera.h"
```

***

## 关键属性

| 属性             | 默认值         | 说明       |
| -------------- | ----------- | -------- |
| `_priority`    | 0           | 渲染优先级    |
| `_groupIds`     | 1           | 可见性组 ID  |
| `_projection`   | Perspective | 投影类型     |
| `_fov`         | 45.0f       | 视场角度（度）  |
| `_nearClip`     | 0.1f        | 近裁剪平面    |
| `_farClip`      | 100.0f      | 远裁剪平面    |
| `_orthoHeight`  | 1.0f        | 正交投影高度   |
| `_isOnScreen`   | false       | 是否在屏幕上显示 |

***

## 核心概念

### 投影类型（ECameraProjection）

| 值             | 说明   |
| ------------- | ---- |
| `Ortho`       | 正交投影 |
| `Perspective` | 透视投影 |

### 视图矩阵

将 3D 世界空间变换为 3D 相机空间，负责将世界坐标变换到摄像机/观察空间。

### 投影矩阵

将 3D 观察空间变换为 2D 裁剪空间（NDC 空间：-1 到 1），根据摄像机的视场角度、近裁剪平面、远裁剪平面进行变换。

***

## API 接口

#### 屏幕设置

| 接口                          | 说明         | 参数                     | 返回值  |
| --------------------------- | ---------- | ---------------------- | ---- |
| `setIsOnScreen(isOnScreen)` | 设置是否在屏幕上显示 | isOnScreen: true/false | void |
| `getIsOnScreen()`           | 获取是否在屏幕上显示 | -                      | bool |

#### 优先级设置

| 接口                      | 说明      | 参数                    | 返回值  |
| ----------------------- | ------- | --------------------- | ---- |
| `setPriority(priority)` | 设置渲染优先级 | priority: 优先级（越小越先渲染） | void |
| `getPriority()`         | 获取渲染优先级 | -                     | int  |

#### 可见性组

| 接口                      | 说明        | 参数                     | 返回值  |
| ----------------------- | --------- | ---------------------- | ---- |
| `setGroupIDs(groupIDs)` | 设置可见性组 ID | groupIDs: 组 ID 位掩码     | void |
| `addGroupID(groupID)`   | 添加可见性组 ID | groupID: 组 ID（整数值）     | void |
| `addGroupID(groupID)`   | 添加可见性组 ID | groupID: ENodeGroup 枚举 | void |
| `getGroupIDs()`         | 获取可见性组 ID | -                      | int  |

#### 投影设置

| 接口                          | 说明     | 参数                               | 返回值  |
| --------------------------- | ------ | -------------------------------- | ---- |
| `setProjection(projection)` | 设置投影类型 | projection: ECameraProjection 枚举 | void |

#### 视图更新

| 接口                 | 说明     | 参数 | 返回值  |
| ------------------ | ------ | -- | ---- |
| `updateViewSize()` | 更新视图大小 | -  | void |

#### 属性设置

| 接口                  | 说明              | 参数            | 返回值  |
| ------------------- | --------------- | ------------- | ---- |
| `setProperty(data)` | 从 JSON 数据设置组件属性 | data: json 对象 | void |

***

## JSON 属性

> 并非所有属性都得传，只传需要更新的属性和值即可。

| 属性名           | 类型    | 说明               |
| -------------- | ----- | ---------------- |
| `uuid`         | string | 组件 UUID           |
| `enable`       | bool  | 是否启用             |
| `priority`     | int   | 渲染优先级            |
| `groupIds`     | int   | 可见性组 ID           |
| `projection`   | int   | 投影类型（0: Ortho, 1: Perspective） |
| `fov`          | float | 视场角度（度）          |
| `nearClip`     | float | 近裁剪平面            |
| `farClip`      | float | 远裁剪平面            |
| `orthoHeight`  | float | 正交投影高度           |
| `isOnScreen`   | bool  | 是否在屏幕上显示         |

#### JSON 示例

```json
{
    "uuid": "550e8400-e29b-41d4-a716-446655440000",
    "enable": true,
    "priority": 0,
    "groupIds": 1,
    "projection": 1,
    "fov": 45.0,
    "nearClip": 0.1,
    "farClip": 100.0,
    "orthoHeight": 1.0,
    "isOnScreen": false
}
```

***

## 使用示例

### 创建 2D 相机

```cpp
auto *scene = Boo::game->getScene();
auto *root2D = scene->getRoot2D();

auto *cameraNode = new Boo::Node2D("Camera2D");
cameraNode->setPosition(0.0f, 0.0f, 0.0f);
root2D->addChild(cameraNode);

auto *camera = dynamic_cast<Boo::Camera *>(cameraNode->addComponent("Camera"));
if (camera)
{
    camera->addGroupID(Boo::ENodeGroup::Node2D);
    camera->setProjection(Boo::ECameraProjection::Ortho);
    camera->setPriority(100);
}
```

### 创建 3D 相机

```cpp
Boo::Node3D *root3D = m_scene->getRoot3D();
if (!root3D)
    return;

// 创建 3D 相机节点
m_cameraNode = new Boo::Node3D("Camera3D");
root3D->addChild(m_cameraNode);
m_cameraNode->setPosition(0, 30.0f, 30.0f);
m_cameraNode->setEulerAngles(-45, 0, 0);
m_cameraNode->setScale(1, 1, 1);

Boo::Camera *m_camera = dynamic_cast<Boo::Camera *>(m_cameraNode->addComponent("Camera"));
if (m_camera)
{
    m_camera->addGroupID(Boo::ENodeGroup::Node3D);
    m_camera->setProjection(Boo::ECameraProjection::Perspective);
    m_camera->setPriority(0);
}
```
