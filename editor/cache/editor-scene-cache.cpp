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
            return "";
        }
    }
    else
    {
        // path = std::filesystem::path(BooEditor::projectPath, std::filesystem::path(this->_scenePath).generic_string()).string();
    }
    std::cout << "EditorSceneCache::_saveScene: " << path << std::endl;
    return path;
}
void EditorSceneCache::_serializeSceneData(Scene *scene, json &sceneData)
{
    std::function<void(Node *, json &)> _serializeNode = [&](Node *node, json &nodeData)
    {
        // 节点数据
        nodeData["_name"] = node->getName();
        nodeData["_layer"] = node->getLayer();
        nodeData["_uuid"] = node->getUuid();
        nodeData["_visible"] = 0;
        nodeData["_active"] = node->isActive() ? 1 : 0;
        nodeData["_position"] = {node->getPosition().getX(), node->getPosition().getY(), node->getPosition().getZ()};
        nodeData["_rotation"] = {node->getEulerAngles().getX(), node->getEulerAngles().getY(), node->getEulerAngles().getZ()};
        nodeData["_scale"] = {node->getScale().getX(), node->getScale().getY(), node->getScale().getZ()};
        if (static_cast<Node2D *>(node) != nullptr)
        {
            nodeData["_size"] = {static_cast<Node2D *>(node)->getSize().getWidth(), static_cast<Node2D *>(node)->getSize().getHeight()};
        }
        // 组件数据
        nodeData["_components"] = json::array();
        for (Component *comp : node->getComponents())
        {
            json compData;
            nodeData["_components"].push_back(compData);
        }

        //    for (Component *comp : node->getComponents())
        //    {
        //        json compData;
        //        comp->serialize(compData);
        //        nodeData["_components"].push_back(compData);
        //    }
        //    for (Node *child : node->getChildren())
        //    {
        //        json childData;
        //        _serializeNode(child, childData);
        //        nodeData["_children"].push_back(childData);
        //    }
    };
    sceneData["_name"] = scene->getName();
    sceneData["_type"] = "SceneAsset";
    sceneData["_data"] = json::object();
    _serializeNode(scene, sceneData["_data"]);
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