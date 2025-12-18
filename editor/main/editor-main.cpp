#include "editor-main.h"
#include <filesystem>
#include <iostream>
#include "../boo-editor.h"
#include "../config/editor-config.h"
#include "../cache/scene/editor-cache-scene.h"
#include "../cache/setting/editor-cache-setting.h"
#include "../cache/assets-db/editor-cache-assets-db.h"

#include "editor-layout.h"
#include "hierarchy/editor-hierarchy.h"

#include "../../engine/boo.h"
#include "../../engine/core/game.h"
#include "../../engine/core/input/input.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/scene/node-2d.h"
#include "../../engine/core/renderer/camera.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/renderer/ui/ui-mask.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/component/ui/ui-widget.h"

EditorMain::EditorMain(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功后，更新模块尺寸
 */
void EditorMain::_deserialized()
{
    Component::_deserialized();
}
void EditorMain::Awake()
{
    this->_initCamera();
    this->_initLayout();
    this->_initHierarchy();
    this->_initScene();
}
void EditorMain::Enable()
{
    Component::Enable();
}
void EditorMain::_initCamera()
{
    Scene *scene = Boo::game->getScene();
    if (scene == nullptr)
    {
        return;
    }
    Node2D *node2d = scene->getRoot2D();
    Node2D *ndCamera = new Node2D("Editor-EditorLoading-Camera");
    node2d->addChild(ndCamera);
    ndCamera->setPosition(0.0f, 0.0f, -100.0f);
    this->_uiCamera = dynamic_cast<Camera *>(ndCamera->addComponent("Camera"));
}
void EditorMain::_initLayout()
{
    this->_layout = new EditorLayout();
    this->_layout->init();
}
void EditorMain::_initHierarchy()
{
    this->_hierarchy = new EditorHierarchy(this->_layout->getHierarchy());
    this->_hierarchy->init();
}




void EditorMain::_initScene()
{
    BooEditor::cache->SceneCache()->openScene("");
    this->_scene = BooEditor::cache->SceneCache()->getScene();
    std::cout << "EditorMain::_initScene: " << this->_scene << std::endl;
    // // ctrl+S 保存
    // BooEditor::cache->SceneCache()->saveScene();
    // BooEditor::cache->SettingCache()->saveSetting();
    // BooEditor::cache->AssetsDBCache()->saveAssetsDB();
}

void EditorMain::Update(float deltaTime)
{
    Component::Update(deltaTime);
    this->_layout->update(deltaTime);
    this->_hierarchy->update(deltaTime);
}
void EditorMain::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
}

void EditorMain::Disable()
{
    Component::Disable();
}
void EditorMain::destroy()
{
    Component::destroy();
}

EditorMain::~EditorMain()
{
    std::cout << "EditorLayout::~EditorLayout()" << std::endl;
}