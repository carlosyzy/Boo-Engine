#include "editor-boot.h"
#include "../boo-editor.h"
#include "cache/editor-cache.h"
#include "loading/editor-loading.h"
#include "main/editor-main.h"

#include "../../engine/boo.h"
#include "../../engine/core/game.h"
#include "../../engine/core/assets/assets-manager.h"
#include "../../engine/core/assets/scene-asset.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/utils/dialog-util.h"
#include "../../engine/core/utils/time-util.h"

EditorBoot::EditorBoot()
{
    std::cout << "EditorBoot::EditorBoot" << std::endl;
    BooEditor::cache = new EditorCache();
    BooEditor::cache->init();
    std::cout << "EditorBoot::EditorBoot: cache init" << std::endl;
}
void EditorBoot::init()
{
    this->_launchEditorLoading();
}
void EditorBoot::_launchEditorLoading()
{
    // 加载场景
    Scene *scene = new Scene("Editor-Loading-Scene");
    scene->createRoot2D();
    Node2D *node2d = scene->getRoot2D();
    EditorLoading *editorLoading = static_cast<EditorLoading *>(node2d->addComponent("EditorLoading"));
    std::cout << "EditorMain::init: scene: " << scene->getName() << std::endl;
    Boo::game->openScene(scene);
    editorLoading->setOnLoadComplete([this]()
                                     { this->_launchEditorMain(); });
}

void EditorBoot::_launchEditorMain()
{
    std::cout << "EditorBoot::_launchEditor" << std::endl;
    // 加载场景
    Scene *scene = new Scene("Editor-Main-Scene");
    scene->createRoot2D();
    Node2D *node2d = scene->getRoot2D();
    EditorMain *editorMain = static_cast<EditorMain *>(node2d->addComponent("EditorMain"));
    std::cout << "EditorMain::openScene: scene: " << scene->getName() << std::endl;
    Boo::game->openScene(scene);
    Boo::game->input()->setRoot2D(node2d);
}
void EditorBoot::tick(float deltaTime)
{
     BooEditor::cache->update(deltaTime);
}

EditorBoot::~EditorBoot()
{
}
