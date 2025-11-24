#include "editor-main.h"
#include "../engine/boo.h"
#include "layout/editor-loading.h"
#include "../engine/core/scene/scene.h"

EditorMain::EditorMain()
{
}
void EditorMain::init()
{
    this->_initDelayScheduleID = Boo::game->scheduleOnce(&EditorMain::_startLoading, this, 2.0f);
}
void EditorMain::_startLoading()
{
    // // 加载场景
    // Scene *scene = new Scene("Editor-Loading-Scene");
    // Node2D *node2d = scene->getRoot2D();
    // EditorLoading *editorLoading = new EditorLoading("EditorLoading", node2d);
    // node2d->addComponent(editorLoading);
    // Boo::game->openScene(scene);
}
EditorMain::~EditorMain()
{
    Boo::game->unschedule(this->_initDelayScheduleID);
}
