# Boo 命名空间表单定义

## 全局实例访问

全局实例通过指针直接访问，无需参数：

| 实例 | 访问方式 |
|------|---------|
| 游戏核心 | `Boo::game->方法名(...)` |
| 事件系统 | `Boo::event->on/off/emit(...)` |
| 资产管理 | `Boo::assetManager->loadAsset/getAsset(...)` |
| 视图系统 | `Boo::view->方法名(...)` |

## 内置资产常量用法

| 常量 | 用法示例 |
|------|---------|
| `Boo::AssetBuiltinTexture::Default` | `sprite->setTexture(Boo::AssetBuiltinTexture::Default)` |
| `Boo::AssetBuiltinMaterial::UI` | `sprite->setMaterial(Boo::AssetBuiltinMaterial::UI)` |

## 内置事件常量用法

| 常量 | 用法示例 |
|------|---------|
| `Boo::EventEngine_Launch` | `Boo::event->on(Boo::EventEngine_Launch, callback)` |
