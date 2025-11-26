#include "editor-main.h"
#include "boo-editor.h"
#include "cache/editor-project-cache.h"
#include "cache/editor-scene-cache.h"
#include "layout/editor-loading.h"
#include "layout/editor-layout.h"
#include "layout/scene/editor-scene.h"

#include "../engine/boo.h"
#include "../engine/core/game.h"
#include "../engine/core/assets/assets-manager.h"
#include "../engine/core/assets/scene-asset.h"
#include "../engine/core/scene/scene.h"
#include "../engine/core/utils/dialog-util.h"

EditorMain::EditorMain()
{
}
void EditorMain::init()
{
    this->_initDelayScheduleID = Boo::game->scheduleOnce(&EditorMain::_startLoading, this, 2.0f);
}
void EditorMain::_startLoading()
{
    return;
    // 加载场景
    // Scene *scene = new Scene("Editor-Loading-Scene");
    // Node2D *node2d = scene->getRoot2D();
    // EditorLoading *editorLoading = new EditorLoading("EditorLoading", node2d);
    // node2d->addComponent(editorLoading);
    // Boo::game->openScene(scene);
    // editorLoading->setOnLoadComplete([this]()
    //                                  { this->_launchEditor(); });
}
void EditorMain::_launchEditor()
{
    // BooEditor::scene->clear();
    // Scene *scene = new Scene("Editor-Scene");
    // Node2D *node2d = scene->getRoot2D();
    // this->_editorLayout = new EditorLayout("EditorLayout", node2d);
    // node2d->addComponent(this->_editorLayout);
    // Boo::game->openScene(scene);
    // // 初始化编辑器模块
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
    // 初始化启动场景
    std::string launchScene = BooEditor::project->getLaunchScene();
    if (launchScene.empty()) // 没有指定启动场景
    {
        BooEditor::scene->newScene();
    }
    else
    {
        SceneAsset *sceneAsset = static_cast<SceneAsset *>(Boo::game->assetsManager()->get(launchScene));
        if (sceneAsset)
        {
            BooEditor::scene->openScene(sceneAsset->getName(), launchScene, sceneAsset->getScene());
        }
        else
        {
            BooEditor::scene->newScene();
        }
    }
}
void EditorMain::_launchEditorScene()
{
   
}

EditorMain::~EditorMain()
{
    Boo::game->unschedule(this->_initDelayScheduleID);
}
