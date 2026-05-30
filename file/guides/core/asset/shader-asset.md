# ShaderAsset 着色器资产

## 概述

ShaderAsset（着色器资产）用于存储着色器代码，包括顶点着色器和片段着色器。

---

## 头文件

```cpp
#include "core/asset/shader-asset.h"
```

---

## API 接口

#### 构造函数

| 接口 | 说明 |
|------|------|
| `ShaderAsset()` | 创建空着色器资产 |
| `ShaderAsset(uuid)` | 通过 UUID 创建着色器资产 |
| `ShaderAsset(uuid, path, name)` | 通过完整参数创建着色器资产 |

#### 着色器创建

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `create(type, glslData, macros)` | 通过 GLSL 代码创建着色器 | type: 着色器类型，glslData: GLSL 代码，macros: 宏定义映射 | void |
| `create(data)` | 通过 SPIR-V 数据创建着色器 | data: SPIR-V 数据向量 | void |

#### 资源管理

| 接口 | 说明 | 参数 | 返回值 |
|------|------|------|--------|
| `destroy()` | 销毁着色器资产 | - | void |

---

## 着色器类型

| 常量名     | 说明      |
| -------- | ------- |
| `ShaderVertexAsset` | 顶点着色器 |
| `ShaderFragmentAsset` | 片段着色器 |
