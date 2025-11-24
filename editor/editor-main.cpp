#include "editor-main.h"
#include "../engine/boo.h"
#include "layout/editor-loading.h"

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
    EditorLoading *loadingScene = new EditorLoading("Editor-EditorLoading-Scene");
    Boo::game->openScene(loadingScene);
}
EditorMain::~EditorMain()
{
    Boo::game->unschedule(this->_initDelayScheduleID);
}
