# UISprite UI 精灵组件

## 概述

UISprite（UI 精灵组件）用于在 2D 界面上显示图片。通过 UISprite 组件，可以将纹理绑定到节点上，实现图片的显示。

### 组件层级

Layer2D 组件，只能添加到 Node2D 节点。

---

## 头文件

```cpp
#include "core/renderer/ui/ui-sprite.h"
```

---

## 尺寸模式（ESizeMode）

| 值       | 说明       |
| ------- | -------- |
| `Raw`   | 图片原始尺寸 |
| `Custom` | 自定义尺寸   |

尺寸模式用于控制 UISprite 的显示尺寸：
- **Raw**：自动使用纹理的原始尺寸
- **Custom**：使用 Node2D 节点设置的尺寸

---

## API 接口

#### 纹理设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setTexture(texture)` | 通过路径设置纹理 | texture: 纹理路径 | void |
| `setTexture(textureAsset)` | 直接设置纹理资产 | textureAsset: TextureAsset 指针 | void |

#### 材质设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setMaterial(material)` | 通过名称设置材质 | material: 材质名称 | void |
| `setMaterial(materialAsset)` | 直接设置材质资产 | materialAsset: MaterialAsset 指针 | void |

#### 颜色设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setColor(color)` | 设置颜色 | color: Color 引用 | void |
| `setColor(color)` | 通过字符串设置颜色 | color: 颜色字符串（如 "#FF0000"） | void |
| `setColor(r, g, b, a)` | 通过 RGBA 设置颜色 | r, g, b, a: 颜色分量（0-1） | void |
| `setAlpha(alpha)` | 设置透明度 | alpha: 透明度（0-1） | void |

#### 尺寸模式

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setSizeMode(sizeMode)` | 设置尺寸模式 | sizeMode: ESizeMode 枚举 | void |

---

## 使用示例

```cpp
auto *bgNode = new Boo::Node2D("bg");
bgNode->setSize(720.0f, 1280.0f);
_node2d->addChild(bgNode);
auto *bgSprite = dynamic_cast<Boo::UISprite *>(bgNode->addComponent("UISprite"));
if (bgSprite)
{
    bgSprite->setTexture("bricked/textures/bg.png");
}
```
