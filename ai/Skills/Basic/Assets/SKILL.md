---
name: "assets"
description: "资产管理系统，负责加载和管理游戏资源。"
---

# 资产管理系统

资产管理系统负责加载和管理游戏资源，包括纹理、材质、网格等。通过 `Boo::assetManager` 全局实例访问。

## 核心功能

### 资源加载
- **统一加载接口**：`loadAsset` 按路径或 UUID 自动识别类型并加载
- **资源获取**：`getAsset` 获取已加载的资源，可选自动加载
- **资源卸载**：`unloadAsset` 卸载指定资源

### 资产类型
- **TextureAsset**：纹理资产，支持静态和动态两种模式
- **MaterialAsset**：材质资产，支持设置纹理和颜色
- **MeshAsset**：网格资产，支持静态和动态网格

## 主要接口

### AssetManager
```cpp
// 加载资产（按路径或 UUID，自动识别类型）
Asset* loadAsset(const std::string& uuid);

// 获取已加载的资产，loadIfNotFound=true 时未找到会自动加载
Asset* getAsset(const std::string& uuid, bool loadIfNotFound = false);

// 卸载资产
void unloadAsset(const std::string& uuid);

// 设置/获取资产根目录
void setAssetsRoot(const std::string& assetsRoot);
const std::string& getAssetsRoot();
```

### TextureAsset
```cpp
// 创建静态纹理（GPU 上传后释放 CPU 数据）
void create(int width, int height, int channels,
            std::vector<uint8_t> pixelsVector, GfxTextureFormat format);

// 创建动态纹理（保留 CPU 数据，支持运行时修改）
void createDynamic(int width, int height, int channels,
                   std::vector<uint8_t> pixelsVector, GfxTextureFormat format);

// 从文件内存数据创建纹理
void create(const unsigned char* data, size_t size);

// 属性获取
int getWidth();
int getHeight();
int channels();
GfxTexture* getGfxTexture();
```

### MaterialAsset
```cpp
// 从 JSON 数据创建材质
void create(const json& materialData);

// 从已有材质复制创建
void create(MaterialAsset* mtl);

// 设置纹理
void setTexture(TextureAsset* texture);
void setTexture(const std::string& key, TextureAsset* texture);

// 设置 UI 颜色
void setUIColor(float r, float g, float b, float w);

GfxMaterial* getGfxMaterial();
```

### MeshAsset
```cpp
// 创建静态网格
void create(std::vector<FMeshPrimitive> primitives);

// 创建动态网格（支持运行时更新）
void createDynamic(std::vector<FMeshPrimitive> primitives);

// 更新动态网格子网格
void updateDynamicSubMesh(int index, FMeshPrimitive& data);

size_t getSubMeshCount();
GfxMesh* getGfxMesh(int index);
```

## 内置资产常量

| 常量 | 描述 |
|------|------|
| `Boo::AssetBuiltinTexture::Default` | 默认白色纹理 |
| `Boo::AssetBuiltinMaterial::UI` | 内置 UI 材质 |

## 枚举类型

### EAssetType

| 值 | 描述 |
|----|------|
| `None` | 无 |
| `Texture` | 纹理 |
| `Audio` | 音频 |
| `Font` | 字体 |
| `Shader` | 着色器 |
| `Material` | 材质 |
| `Scene` | 场景 |
| `GLTF` | GLTF 模型 |
| `Mesh` | 网格 |

### ETextureType

| 值 | 描述 |
|----|------|
| `Static` | 静态纹理，GPU 上传后释放 CPU 数据 |
| `Dynamic` | 动态纹理，保留 CPU 数据支持运行时修改 |

## 使用示例

### 加载纹理并设置到精灵

```cpp
#include "engine/boo.h"

void loadResources() {
    // 从文件加载纹理
    Boo::TextureAsset* tex = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->loadAsset("assets/textures/player.png")
    );

    // 设置到 UISprite
    Boo::UISprite* sprite = dynamic_cast<Boo::UISprite*>(node->addComponent("UISprite"));
    if (sprite && tex) {
        sprite->setTexture(tex);
    }
}
```

### 加载材质

```cpp
Boo::MaterialAsset* mat = static_cast<Boo::MaterialAsset*>(
    Boo::assetManager->loadAsset("assets/materials/ui.mat")
);

// 设置纹理到材质
Boo::TextureAsset* tex = static_cast<Boo::TextureAsset*>(
    Boo::assetManager->getAsset("assets/textures/player.png", true)
);
mat->setTexture(tex);
```

### 使用内置资产

```cpp
// 使用默认白色纹理（无需加载，直接使用）
sprite->setTexture(Boo::AssetBuiltinTexture::Default);

// 使用内置 UI 材质
sprite->setMaterial(Boo::AssetBuiltinMaterial::UI);
```

## 最佳实践

1. **资源路径**：使用相对路径，`setAssetsRoot` 设置根目录后路径会自动拼接
2. **避免重复加载**：优先用 `getAsset` 获取已缓存资源
3. **及时卸载**：不再使用的资源调用 `unloadAsset` 释放内存

## 总结

资产管理系统通过 `loadAsset/getAsset/unloadAsset` 统一管理所有资产。内置常量 `AssetBuiltinTexture::Default` 和 `AssetBuiltinMaterial::UI` 可直接使用，无需加载。
