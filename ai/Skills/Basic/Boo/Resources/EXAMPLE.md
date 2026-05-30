# Boo 命名空间使用示例

## 示例 1: 引擎启动流程

```cpp
#include "engine/boo.h"
#include "engine/log.h"

int main() {
    // 创建窗口
    Boo::Window* window = new Boo::Window();
    window->init();

    // 初始化引擎
    Boo::game->init(window, 1280, 720, Boo::UIDesignFitMode::Width);

    // 监听引擎启动完成事件，在此回调中创建场景
    Boo::event->on(Boo::EventEngine_Launch, []() {
        LOGI("引擎启动完成");

        Boo::Scene* scene = new Boo::Scene("MainScene");
        Boo::game->openScene(scene);

        Boo::Node2D* root2D = scene->getRoot2D();

        // 创建相机
        Boo::Node2D* camNode = new Boo::Node2D("Camera");
        root2D->addChild(camNode);
        auto* cam = dynamic_cast<Boo::Camera*>(camNode->addComponent("Camera"));
        if (cam) {
            cam->setIsOnScreen(true);
        }

        // 创建精灵
        Boo::Node2D* spriteNode = new Boo::Node2D("Sprite");
        spriteNode->setPosition(640, 360, 0);
        spriteNode->setSize(100, 100);
        root2D->addChild(spriteNode);

        auto* sprite = dynamic_cast<Boo::UISprite*>(spriteNode->addComponent("UISprite"));
        if (sprite) {
            sprite->setTexture(Boo::AssetBuiltinTexture::Default);
            sprite->setColor(1.0f, 0.5f, 0.0f, 1.0f);
        }
    });

    // 主循环
    while (!window->shouldClose()) {
        window->pollEvents();
        Boo::game->tick();
        window->swapBuffers();
    }

    return 0;
}
```

## 示例 2: 访问全局系统

```cpp
#include "engine/boo.h"

void accessGlobals() {
    // 访问游戏系统
    int fps = Boo::game->getFps();
    LOGI("当前帧率: %d", fps);

    Boo::Scene* scene = Boo::game->getScene();

    // 访问事件系统
    Boo::event->emit("MyCustomEvent");

    // 访问资产管理
    Boo::assetManager->setAssetsRoot("assets/");
    auto* tex = static_cast<Boo::TextureAsset*>(
        Boo::assetManager->loadAsset("textures/bg.png")
    );
}
```

## 示例 3: 使用内置常量

```cpp
#include "engine/boo.h"

void useBuiltins(Boo::Node2D* node) {
    auto* sprite = dynamic_cast<Boo::UISprite*>(node->addComponent("UISprite"));
    if (sprite) {
        // 使用默认白色纹理，无需加载任何文件
        sprite->setTexture(Boo::AssetBuiltinTexture::Default);

        // 使用内置 UI 材质
        sprite->setMaterial(Boo::AssetBuiltinMaterial::UI);
    }
}
```
