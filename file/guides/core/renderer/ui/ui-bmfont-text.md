# UIBMFontText BMFont 文本组件

## 概述

UIBMFontText（BMFont 文本组件）用于在 2D 界面上显示 BMFont 位图字体文本。通过 BMFont 组件，可以设置文本内容、字体大小、行高等属性，实现文字的显示。

### 组件层级

Layer2D 组件，只能添加到 Node2D 节点。

---

## 头文件

```cpp
#include "core/renderer/ui/ui-bmfont-text.h"
```

---

## API 接口

#### 文本设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setText(text)` | 设置文本内容（string） | text: 文本字符串 | void |
| `setText(text)` | 设置文本内容（wstring） | text: 宽字符串 | void |
| `setText(text)` | 设置文本内容（wchar_t*） | text: 宽字符数组 | void |
| `getText()` | 获取文本内容 | - | const std::wstring& |

#### 字体设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setSize(fontSize)` | 设置字体大小 | fontSize: 字体大小（像素） | void |
| `getSize()` | 获取字体大小 | - | const int& |
| `setLineHeight(lineHeight)` | 设置行高 | lineHeight: 行高（像素） | void |
| `getLineHeight()` | 获取行高 | - | const int& |

#### BMFont 设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setBMFont(path)` | 通过路径设置 BMFont | path: BMFont 路径 | void |
| `setBMFont(bmFont)` | 直接设置 BMFont 资产 | bmFont: BMFontAsset 指针 | void |

#### 颜色设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setColor(color)` | 设置颜色 | color: Color 引用 | void |
| `setColor(color)` | 通过字符串设置颜色 | color: 颜色字符串（如 "#FF0000"） | void |
| `setColor(r, g, b, a)` | 通过 RGBA 设置颜色 | r, g, b, a: 颜色分量（0-1） | void |
| `setAlpha(alpha)` | 设置透明度 | alpha: 透明度（0-1） | void |

#### 材质设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setMaterial(material)` | 通过名称设置材质 | material: 材质名称 | void |
| `setMaterial(materialAsset)` | 直接设置材质资产 | materialAsset: MaterialAsset 指针 | void |

---

## 使用示例

```cpp
auto *textNode = new Boo::Node2D("text");
textNode->setPosition(360.0f, 640.0f, 0.0f);
_node2d->addChild(textNode);

auto *text = dynamic_cast<Boo::UIBMFontText *>(textNode->addComponent("UIBMFontText"));
if (text)
{
    text->setText("Hello World");
    text->setSize(40);
    text->setLineHeight(50);
}
```
