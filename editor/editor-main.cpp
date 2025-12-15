#include "editor-main.h"
#include "boo-editor.h"
// #include "cache/editor-project-cache.h"
// #include "cache/editor-scene-cache.h"
#include "cache/editor-cache.h"
#include "loading/editor-loading.h"
#include "layout/editor-layout.h"
#include "layout/scene/editor-scene.h"

#include "../engine/boo.h"
#include "../engine/core/game.h"
#include "../engine/core/assets/assets-manager.h"
#include "../engine/core/assets/scene-asset.h"
#include "../engine/core/scene/scene.h"
#include "../engine/core/utils/dialog-util.h"
#include "../engine/core/utils/time-util.h"

EditorMain::EditorMain()
{
    this->_deltaTime = TimeUtil::nowTime();
    this->_frameRate = 30;
    BooEditor::cache = new EditorCache();
    BooEditor::cache->init();
}
void EditorMain::init()
{
    std::cout << "EditorMain::_startLoading" << std::endl;
    // 加载场景
    Scene *scene = new Scene("Editor-Loading-Scene");
    Node2D *node2d = scene->getRoot2D();
    EditorLoading *editorLoading = static_cast<EditorLoading *>(node2d->addComponent("EditorLoading"));
    Boo::game->openScene(scene);
    editorLoading->setOnLoadComplete([this]()
                                     { this->_launchEditor(); });
}
void EditorMain::_launchEditor()
{
    Scene *scene = new Scene("Editor-Scene");
    Node2D *node2d = scene->getRoot2D();
    this->_editorLayout = static_cast<EditorLayout *>(node2d->addComponent("EditorLayout"));
    Boo::game->openScene(scene);
    // BooEditor::scene->clear();
    // 初始化编辑器模块
    // this->_initEditorModules();
    // // 初始化编辑器场景
    // this->_initEditorRunScene();
    // // 启动编辑器场景
    // this->_launchEditorScene();
}
void EditorMain::_initEditorModules()
{
    this->_editorScene = new EditorScene();
}
void EditorMain::_initEditorRunScene()
{
    // // 初始化启动场景
    // std::string launchScene = BooEditor::project->getLaunchScene();
    // if (launchScene.empty()) // 没有指定启动场景
    // {
    //     BooEditor::scene->newScene();
    // }
    // else
    // {
    //     SceneAsset *sceneAsset = static_cast<SceneAsset *>(Boo::game->assetsManager()->get(launchScene));
    //     if (sceneAsset)
    //     {
    //         BooEditor::scene->openScene(sceneAsset->getName(), launchScene, sceneAsset->getScene());
    //     }
    //     else
    //     {
    //         BooEditor::scene->newScene();
    //     }
    // }
}
void EditorMain::_launchEditorScene()
{
    // BooEditor::project->setLaunchScene(BooEditor::scene->getSceneName());
    // BooEditor::scene->save();
}
void EditorMain::tick()
{
    long long deltaTime = TimeUtil::nowTime();
    long long t = deltaTime - this->_deltaTime;
    if (t > (1000.0f / this->_frameRate))
    {
        this->update(t / 1000.0f);
        this->_deltaTime = deltaTime;
    }
}
void EditorMain::update(float deltaTime)
{
    if (BooEditor::cache)
    {
        BooEditor::cache->update(deltaTime);
    }
}

EditorMain::~EditorMain()
{
}
