# MaterialAsset 材质资产

## 概述

MaterialAsset（材质资产）用于存储材质的配置数据，包括顶点着色器、片段着色器、纹理、渲染状态等信息。

---

## 头文件

```cpp
#include "core/asset/material-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `MaterialAsset()` | 创建空材质资产 |
| `MaterialAsset(uuid)` | 通过 UUID 创建材质资产 |
| `MaterialAsset(uuid, path, name)` | 通过完整参数创建材质资产 |

#### 材质创建

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(materialData)` | 通过 JSON 数据创建材质 | materialData: JSON 格式的材质数据 | void |
| `create(mtl)` | 通过材质资产创建 | mtl: MaterialAsset 指针 | void |
| `getOriginData()` | 获取原始材质数据 | - | const json& |

#### 着色器设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setVertexShader(vert)` | 直接设置顶点着色器 | vert: ShaderAsset 指针 | void |
| `setVertexShader(vert)` | 通过路径设置顶点着色器 | vert: 着色器路径 | void |
| `setFragmentShader(frag)` | 直接设置片段着色器 | frag: ShaderAsset 指针 | void |
| `setFragmentShader(frag)` | 通过路径设置片段着色器 | frag: 着色器路径 | void |

#### 矩阵设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setModelWorldMatrix(modelMatrix)` | 设置模型世界矩阵 | modelMatrix: 16 元素数组 | void |
| `setModelWorldMatrixIT(modelMatrixIT)` | 设置模型世界矩阵逆转置矩阵 | modelMatrixIT: 16 元素数组 | void |

#### UI 属性设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setUIColor(r, g, b, w)` | 设置 UI 颜色 | r, g, b, w: RGBA 分量 | void |

#### 纹理设置

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `setTexture(texture)` | 设置主纹理 | texture: TextureAsset 指针 | void |
| `setTexture(key, texture)` | 设置指定键的纹理 | key: 纹理键，texture: TextureAsset 指针 | void |

#### 资源查询

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `getGfxMaterial()` | 获取图形材质 | - | GfxMaterial* |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁材质资产 | - | void |

---

## MTL 文件 JSON 属性

材质资产通过 MTL 文件（.mtl）定义，JSON 格式。完整属性如下：

### 基础属性

| 属性名 | 类型 | 说明 | 默认值 |
| ------ | ---- | ---- | ------ |
| `layer` | int | 渲染层级（见 ERendererLayer） | 必填 |
| `type` | int | 渲染类型（见 ERendererType） | 必填 |
| `vert` | string | 顶点着色器路径 | 必填 |
| `frag` | string | 片段着色器路径 | 必填 |

**ERendererLayer 渲染层级**

| 值 | 说明 |
| -- | ---- |
| 0 | UI（UI 渲染层级） |
| 1 | Model（模型渲染层级） |

**ERendererType 渲染类型**

| 值 | 说明 |
| -- | ---- |
| 0 | Opaque（不透明渲染） |
| 1 | Transparent（透明渲染） |

### 渲染状态

#### 多边形模式（polygonMode）

| 值 | 说明 |
| -- | ---- |
| 0 | Fill（填充多边形） |
| 1 | Line（线框模式） |

#### 剔除模式（cullMode）

| 值 | 说明 |
| -- | ---- |
| 0 | None（不剔除任何三角形） |
| 1 | Front（剔除正面三角形） |
| 2 | Back（剔除反面三角形） |

#### 深度测试（depthTest）

| 值 | 说明 |
| -- | ---- |
| 0 | 关闭 |
| 1 | 开启 |

#### 深度写入（depthWrite）

| 值 | 说明 |
| -- | ---- |
| 0 | 关闭 |
| 1 | 开启 |

#### 深度比较操作（depthCompareOp）

| 值 | 说明 |
| -- | ---- |
| 0 | Never（从不通过） |
| 1 | Less（小于通过） |
| 2 | LessOrEqual（小等于通过） |
| 3 | Greater（大于通过） |
| 4 | GreaterOrEqual（大等于通过） |
| 5 | Equal（等于通过） |
| 6 | NotEqual（不等于通过） |
| 7 | Always（总是通过） |

#### 模板测试（stencilTest）

| 值 | 说明 |
| -- | ---- |
| 0 | 关闭 |
| 1 | 开启 |

#### 颜色混合（colorBlend）

| 值 | 说明 |
| -- | ---- |
| 0 | 关闭 |
| 1 | 开启 |

#### 混合因子

**源/目标颜色混合因子（srcColorBlendFactor/dstColorBlendFactor）**

| 值 | 说明 |
| -- | ---- |
| 0 | One（加法混合） |
| 1 | Zero（减法混合） |
| 2 | SrcColor（源颜色） |
| 3 | DstColor（目标颜色） |
| 4 | SrcAlpha（源 Alpha） |
| 5 | DstAlpha（目标 Alpha） |
| 6 | OneMinusSrcAlpha（1-源Alpha） |
| 7 | OneMinusDstAlpha（1-目标Alpha） |

**颜色/Alpha 混合操作（colorBlendOp/alphaBlendOp）**

| 值 | 说明 |
| -- | ---- |
| 0 | Add（加法混合） |
| 1 | Subtract（减法混合） |

### 完整示例

```json
{
    "layer": 1,
    "type": 0,
    "vert": "builtin::unlit.vert",
    "frag": "builtin::unlit.frag",
    "polygonMode": 0,
    "cullMode": 2,
    "depthTest": 1,
    "depthWrite": 1,
    "depthCompareOp": 1,
    "stencilTest": 0,
    "colorBlend": 0,
    "srcColorBlendFactor": 4,
    "dstColorBlendFactor": 6,
    "colorBlendOp": 0,
    "srcAlphaBlendFactor": 0,
    "dstAlphaBlendFactor": 6,
    "alphaBlendOp": 0,
    "properties": [
        {
            "name": "color",
            "type": "vec4",
            "value": [1.0, 1.0, 1.0, 1.0]
        }
    ],
    "textures": {
        "mainTexture": {
            "binding": 1,
            "path": "texture.png"
        }
    }
}
```

### properties 属性

properties 用于定义材质自定义参数：

| 属性名 | 类型 | 说明 |
| ------ | ---- | ---- |
| `name` | string | 参数名称 |
| `type` | string | 参数类型（int, float, vec2, vec3, vec4） |
| `value` | array | 参数值 |

**类型对应关系：**

| type | value 格式 |
| ---- | ---------- |
| `int` | [数值] |
| `float` | [数值] |
| `vec2` | [x, y] |
| `vec3` | [x, y, z] |
| `vec4` | [x, y, z, w] |

### textures 属性

textures 用于定义材质纹理：

| 属性名 | 类型 | 说明 |
| ------ | ---- | ---- |
| `binding` | int | 纹理绑定点 |
| `path` | 纹理路径 | string |
