# 资产管理系统示例

## 示例 1: 加载纹理并应用到精灵

```cpp
#include "engine/boo.h"

void setupSprite(Boo::Node2D* node) {
    // 加载纹理
    Boo::TextureAsset* tex = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->loadAsset("textures/player.png")
    );

    if (!tex) {
        LOGE("纹理加载失败");
        return;
    }

    LOGI("纹理尺寸: %dx%d", tex->getWidth(), tex->getHeight());

    // 设置到 UISprite 组件
    auto* sprite = dynamic_cast<Boo::UISprite*>(node->addComponent("UISprite"));
    if (sprite) {
        sprite->setTexture(tex);
        sprite->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
```

## 示例 2: 使用内置资产

```cpp
#include "engine/boo.h"

void setupBuiltin(Boo::Node2D* node) {
    auto* sprite = dynamic_cast<Boo::UISprite*>(node->addComponent("UISprite"));
    if (sprite) {
        // 使用默认白色纹理，无需加载
        sprite->setTexture(Boo::AssetBuiltinTexture::Default);
        // 设置颜色（红色半透明）
        sprite->setColor(1.0f, 0.0f, 0.0f, 0.5f);
    }
}
```

## 示例 3: getAsset 获取已缓存资源

```cpp
#include "engine/boo.h"

void reuseTexture() {
    // 首次加载
    Boo::assetManager->loadAsset("textures/enemy.png");

    // 之后从缓存获取，不重复读取文件
    Boo::TextureAsset* tex = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->getAsset("textures/enemy.png")
    );

    // 或者：未找到时自动加载
    Boo::TextureAsset* tex2 = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->getAsset("textures/background.png", true)
    );
}
```

## 示例 4: 加载材质并设置纹理

```cpp
#include "engine/boo.h"

void setupMaterial(Boo::Node2D* node) {
    // 加载材质
    Boo::MaterialAsset* mat = static_cast<Boo::MaterialAsset*>(
        Boo::assetManager->loadAsset("materials/custom.mat")
    );

    // 加载纹理
    Boo::TextureAsset* tex = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->loadAsset("textures/player.png")
    );

    if (mat && tex) {
        mat->setTexture(tex);
        mat->setUIColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // 设置到精灵
    auto* sprite = dynamic_cast<Boo::UISprite*>(node->addComponent("UISprite"));
    if (sprite && mat) {
        sprite->setMaterial(mat);
    }
}
```

## 示例 5: 卸载资源

```cpp
#include "engine/boo.h"

void cleanupResources() {
    // 卸载不再使用的资源
    Boo::assetManager->unloadAsset("textures/level1_bg.png");
    Boo::assetManager->unloadAsset("materials/level1.mat");
    LOGI("场景资源已卸载");
}
```
