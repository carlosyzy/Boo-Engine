#include "editor-scene-cache.h"
#include "../boo-editor.h"

#include "../../engine/core/scene/scene.h"
#include "../../engine/core/utils/dialog-util.h"

EditorSceneCache::EditorSceneCache()
{
}
void EditorSceneCache::init()
{
}
/**
 * 打开一个新的场景
 */
void EditorSceneCache::newScene()
{
    this->_sceneName = "scene";
    this->_scenePath = "";
    this->_scene = new Scene(this->_sceneName);
    this->_saveFlag = 1;
}
/**
 * 打开一个已经存在的场景
 */
void EditorSceneCache::openScene(std::string sceneName, std::string scenePath, Scene *scene)
{
    this->_sceneName = sceneName;
    this->_scenePath = scenePath;
    this->_scene = scene;
    this->_saveFlag = 0;
}
void EditorSceneCache::save()
{
    if (this->_saveFlag == 0)
    {
        return;
    }
    this->_saveFlag = 0;
    std::string path = this->_preSavePath();
    if (path.empty())
    {
        return;
    }
    json sceneData;
    this->_serializeSceneData(this->_scene, sceneData);
}
std::string EditorSceneCache::_preSavePath()
{
    std::string path;
    if (this->_scenePath.empty())
    {
        path = DialogUtil::saveFileDialog(BooEditor::projectPath, SceneFilterList);
        if (path.empty())
        {
            return;
        }
    }
    else
    {
        path = std::filesystem::path(BooEditor::projectPath, std::filesystem::path(this->_scenePath).generic_string()).string();
    }
    std::cout << "EditorSceneCache::_saveScene: " << path << std::endl;
    return path;
}
void EditorSceneCache::_serializeSceneData(Scene *scene, json &sceneData)
{
    // sceneData["sceneName"] = scene->getName();
}

void EditorSceneCache::update()
{
}
void EditorSceneCache::clear()
{
    this->_sceneName = "";
    this->_scenePath = "";
    this->_scene = nullptr;
    this->_saveFlag = 0;
}
EditorSceneCache::~EditorSceneCache()
{
}