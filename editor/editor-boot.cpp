#include "editor-boot.h"
#include "boo-editor.h"
#include "cache/editor-cache.h"
#include "loading/editor-loading.h"
#include "main/editor-main.h"

#include "../engine/boo.h"
#include "../engine/core/game.h"
#include "../engine/core/assets/assets-manager.h"
#include "../engine/core/assets/scene-asset.h"
#include "../engine/core/scene/scene.h"
#include "../engine/core/utils/dialog-util.h"
#include "../engine/core/utils/time-util.h"

EditorBoot::EditorBoot()
{
    this->_deltaTime = TimeUtil::nowTime();
    this->_frameRate = 30;
    BooEditor::cache = new EditorCache();
    BooEditor::cache->init();
}
void EditorBoot::init()
{
    this->_launchEditorLoading();
}
void EditorBoot::_launchEditorLoading()
{
    std::cout << "EditorBoot::_startLoading" << std::endl;
    // 加载场景
    Scene *scene = new Scene("Editor-Loading-Scene");
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
    Node2D *node2d = scene->getRoot2D();
    EditorMain *editorMain = static_cast<EditorMain *>(node2d->addComponent("EditorMain"));
    std::cout << "EditorMain::openScene: scene: " << scene->getName() << std::endl;
    Boo::game->openScene(scene);
}
void EditorBoot::tick()
{
    long long deltaTime = TimeUtil::nowTime();
    long long t = deltaTime - this->_deltaTime;
    if (t > (1000.0f / this->_frameRate))
    {
        this->update(t / 1000.0f);
        this->_deltaTime = deltaTime;
    }
}
void EditorBoot::update(float deltaTime)
{
    if (BooEditor::cache)
    {
        BooEditor::cache->update(deltaTime);
    }
}

EditorBoot::~EditorBoot()
{
}
