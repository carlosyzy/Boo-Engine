#include "editor-scene-cache.h"
#include "../boo-editor.h"

#include "../../engine/core/component/component.h"
#include "../../engine/core/component/component-property-reflect.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/utils/dialog-util.h"
#include "../../engine/core/utils/file-util.h"

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
    // 测试代码
    // Node2D *node2d = this->_scene->getRoot2D();
    // Node2D *ndMain = new Node2D("Editor-Main");
    // ndMain->setSize(100.0f, 100.0f);
    // node2d->addChild(ndMain);
    // UISprite *spriteMain = dynamic_cast<UISprite *>(ndMain->addComponent("UISprite"));
    // if (spriteMain != nullptr)
    // {
    //     std::cout << "EditorLayout::_initMainUI1" << std::endl;
    //     spriteMain->setTextureAsset("resources/texture/ic-default.png");
    //     spriteMain->setMaterialAsset(nullptr);
    //     spriteMain->setColor(0.1f, 0.1f, 0.1f, 1.0f);
    // }
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
    this->_scenePath = std::filesystem::relative(std::filesystem::path(path), std::filesystem::path(BooEditor::projectPath)).generic_string();
    json sceneData;
    this->_serializeSceneData(this->_scene, sceneData);
    std::cout << "EditorSceneCache::_saveScene: " << sceneData << std::endl;
    FileUtil::saveJsonToBinary(path, sceneData);
}
std::string EditorSceneCache::_preSavePath()
{
    std::string path;
    std::string assetPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    if (this->_scenePath.empty())
    {
        path = DialogUtil::saveFileDialog(assetPath, "scene");
        if (path.empty())
        {
            return "";
        }
        std::string pathStr(path);
        // 检查是否已经有 .scene 后缀 如果没有 .scene 后缀，自动添加
        if (pathStr.size() < 6 || pathStr.substr(pathStr.size() - 6) != ".scene")
        {
            path += ".scene";
        }
    }
    else
    {
        std::string sceneKey = std::filesystem::path(this->_scenePath).generic_string();
        path = (std::filesystem::path(assetPath) / sceneKey).generic_string();
    }
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
        if (node->getLayer() == NodeLayer::Node2D && dynamic_cast<Node2D *>(node) != nullptr)
        {
            Node2D *node2d = dynamic_cast<Node2D *>(node);
            if (node2d != nullptr)
            {
                nodeData["_size"] = {node2d->getSize().getWidth(), node2d->getSize().getHeight()};
            }
        }
        // 组件数据
        json compsData = json::array();
        for (Component *comp : node->getComponents())
        {
            json compData;
            compData["_name"] = comp->getName();
            compData["_uuid"] = comp->getUuid();
            compData["_enable"] = comp->isEnabled() ? 1 : 0;
            compData = ReflectionRegistry::getInstance().serializeToJson((void *)comp, comp->getName());
            compsData.push_back(compData);
        }
        nodeData["_components"] = compsData;
        for (Node *child : node->getChildren())
        {
            json childData;
            _serializeNode(child, childData);
            nodeData["_children"].push_back(childData);
        }
    };
    sceneData["_name"] = scene->getName();
    sceneData["_type"] = "SceneAsset";
    sceneData["_node"] = json::object();
    _serializeNode(scene, sceneData["_node"]);
}
std::string EditorSceneCache::getSceneName()
{
    return this->_sceneName;
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