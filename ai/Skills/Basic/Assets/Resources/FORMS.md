# 资产管理系统表单定义

## AssetManager::loadAsset

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `uuid` | `std::string` | 资产路径或 UUID | 是 |

**返回值**：`Asset*`，需转型为具体类型（`TextureAsset*`、`MaterialAsset*` 等）

## AssetManager::getAsset

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `uuid` | `std::string` | 资产路径或 UUID | 是 |
| `loadIfNotFound` | `bool` | 未找到时是否自动加载 | 否，默认 false |

**返回值**：`Asset*`

## AssetManager::unloadAsset

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `uuid` | `std::string` | 资产路径或 UUID | 是 |

## TextureAsset::create（像素数据）

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `width` | `int` | 宽度 | 是 |
| `height` | `int` | 高度 | 是 |
| `channels` | `int` | 通道数 | 是 |
| `pixelsVector` | `std::vector<uint8_t>` | 像素数据 | 是 |
| `format` | `GfxTextureFormat` | 纹理格式 | 是 |

## TextureAsset::create（文件数据）

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `data` | `const unsigned char*` | 文件内存数据 | 是 |
| `size` | `size_t` | 数据长度 | 是 |

## MaterialAsset::setTexture

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `texture` | `TextureAsset*` | 纹理资产 | 是 |

或

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `key` | `std::string` | 纹理槽名称 | 是 |
| `texture` | `TextureAsset*` | 纹理资产 | 是 |

## MaterialAsset::setUIColor

| 参数 | 类型 | 描述 | 必填 |
|------|------|------|------|
| `r` | `float` | 红色通道 | 是 |
| `g` | `float` | 绿色通道 | 是 |
| `b` | `float` | 蓝色通道 | 是 |
| `w` | `float` | Alpha 通道 | 是 |
