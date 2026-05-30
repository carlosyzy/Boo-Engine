# 资产管理系统 API

## AssetManager 类

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `setAssetsRoot(const std::string &assetsRoot)` | 设置资产根目录 | `assetsRoot`: 根目录路径 | `void` |
| `getAssetsRoot()` | 获取资产根目录 | 无 | `const std::string&` |
| `loadAsset(const std::string &uuid)` | 加载资产（按路径或 UUID，自动识别类型） | `uuid`: 资产路径或 UUID | `Asset*` |
| `getAsset(const std::string &uuid, bool loadIfNotFound = false)` | 获取已加载的资产 | `uuid`: 资产路径或 UUID<br>`loadIfNotFound`: 未找到时是否自动加载 | `Asset*` |
| `unloadAsset(const std::string &uuid)` | 卸载资产 | `uuid`: 资产路径或 UUID | `void` |

## Asset 基类

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `getName()` | 获取资产名称 | 无 | `const std::string&` |
| `getUuid()` | 获取资产 UUID | 无 | `const std::string&` |
| `destroy()` | 销毁资产 | 无 | `void` |

## TextureAsset（纹理资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `create(int width, int height, int channels, std::vector<uint8_t> pixelsVector, GfxTextureFormat format)` | 创建静态纹理（GPU 上传后释放 CPU 数据） | - | `void` |
| `createDynamic(int width, int height, int channels, std::vector<uint8_t> pixelsVector, GfxTextureFormat format)` | 创建动态纹理（保留 CPU 数据，支持 setPixel） | - | `void` |
| `create(const unsigned char *data, size_t size)` | 从文件内存数据创建纹理 | `data`: 文件数据<br>`size`: 数据长度 | `void` |
| `getWidth()` | 获取纹理宽度 | 无 | `int` |
| `getHeight()` | 获取纹理高度 | 无 | `int` |
| `channels()` | 获取纹理通道数 | 无 | `int` |
| `getGfxTexture()` | 获取 GFX 纹理对象 | 无 | `GfxTexture*` |

## MaterialAsset（材质资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `create(const json &materialData)` | 从 JSON 数据创建材质 | `materialData`: 材质 JSON | `void` |
| `create(MaterialAsset *mtl)` | 从已有材质复制创建 | `mtl`: 源材质 | `void` |
| `setTexture(TextureAsset *texture)` | 设置默认纹理槽 | `texture`: 纹理资产 | `void` |
| `setTexture(const std::string &key, TextureAsset *texture)` | 设置指定名称的纹理槽 | `key`: 纹理槽名称<br>`texture`: 纹理资产 | `void` |
| `setModelWorldMatrix(const std::array<float, 16> &modelMatrix)` | 设置模型世界矩阵 | `modelMatrix`: 4x4 矩阵数组 | `void` |
| `setModelWorldMatrixIT(const std::array<float, 16> &modelMatrixIT)` | 设置模型世界矩阵逆转置 | `modelMatrixIT`: 4x4 矩阵数组 | `void` |
| `setUIColor(float r, float g, float b, float w)` | 设置 UI 颜色 | `r`, `g`, `b`, `w`: 颜色通道值 | `void` |
| `getGfxMaterial()` | 获取 GFX 材质对象 | 无 | `GfxMaterial*` |

## MeshAsset（网格资产）

| 方法 | 描述 | 参数 | 返回值 |
|------|------|------|--------|
| `create(std::vector<FMeshPrimitive> primitives)` | 创建静态网格 | `primitives`: 网格基元列表 | `void` |
| `createDynamic(std::vector<FMeshPrimitive> primitives)` | 创建动态网格（支持运行时更新） | `primitives`: 网格基元列表 | `void` |
| `updateDynamicSubMesh(int index, FMeshPrimitive &data)` | 更新动态网格的子网格数据 | `index`: 子网格索引<br>`data`: 新的网格数据 | `void` |
| `getSubMeshCount()` | 获取子网格数量 | 无 | `size_t` |
| `getGfxMesh(int index)` | 获取指定索引的 GFX 网格对象 | `index`: 子网格索引 | `GfxMesh*` |

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

### EMeshMode

| 值 | 描述 |
|----|------|
| `UI` | UI 网格模式 |
| `Model` | 3D 模型网格模式 |

### ETextureType

| 值 | 描述 |
|----|------|
| `Static` | 静态纹理，GPU 上传后释放 CPU 数据 |
| `Dynamic` | 动态纹理，保留 CPU 数据支持运行时修改 |
