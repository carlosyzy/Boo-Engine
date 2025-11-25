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
    // 加载场景
    Scene *scene = new Scene("Editor-Loading-Scene");
    Node2D *node2d = scene->getRoot2D();
    EditorLoading *editorLoading = new EditorLoading("EditorLoading", node2d);
    node2d->addComponent(editorLoading);
    Boo::game->openScene(scene);
    editorLoading->setOnLoadComplete([this]()
                                     { this->_launchEditor(); });
}
void EditorMain::_launchEditor()
{
    BooEditor::scene->clear();
    Scene *scene = new Scene("Editor-Scene");
    Node2D *node2d = scene->getRoot2D();
    this->_editorLayout = new EditorLayout("EditorLayout", node2d);
    node2d->addComponent(this->_editorLayout);
    Boo::game->openScene(scene);
    this->_runEditorScene();
    // std::string path=DialogUtil::saveFileDialog(BooEditor::projectPath, SceneFilterList);
    // std::cout << "path: " << path << std::endl;
}
void EditorMain::_runEditorScene()
{
    // 初始化启动场景
    this->_preRunEditorScene();
    
    this->_editorScene = new EditorScene();
    
}
void EditorMain::_preRunEditorScene()
{
    BooEditor::runScene = new BooEditorRunScene();
    BooEditor::runScene->sceneName = "";
    BooEditor::runScene->scenePath = "";
    BooEditor::runScene->scene = nullptr;
    std::string launchScene = BooEditor::project->getLaunchScene();
    if (launchScene.empty()) // 没有指定启动场景
    {
        BooEditor::runScene->sceneName = "scene";
        BooEditor::runScene->scenePath = "";
        BooEditor::runScene->scene = new Scene("scene");
    }
    else
    {
        SceneAsset *sceneAsset = static_cast<SceneAsset*>(Boo::game->assetsManager()->get(launchScene));
        if(sceneAsset){
            // BooEditor::runScene->scene = sceneAsset->_scene;
            // BooEditor::runScene->sceneName = sceneAsset->getKey();
            // BooEditor::runScene->scenePath = sceneAsset->getPath();
        }else{
            BooEditor::runScene->sceneName = "scene";
            BooEditor::runScene->scenePath = BooEditor::projectPath + "/scene";
            BooEditor::runScene->scene = new Scene("scene");
        }
    }
}

EditorMain::~EditorMain()
{
    Boo::game->unschedule(this->_initDelayScheduleID);
}
