#include "editor-cache-scene.h"
#include "../../../boo-editor.h"

#include "../../../../engine/boo.h"
#include "../../../../engine/core/assets/assets-manager.h"
#include "../../../../engine/core/assets/texture-asset.h"
#include "../../../../engine/core/assets/asset-cache.h"
#include "../../../../engine/core/assets/asset-struct.h"
#include "../../../../engine/core/assets/scene-asset.h"
#include "../../../../engine/core/utils/file-util.h"
#include "../../../../engine/core/utils/uuid-util.h"
#include "../../../../engine/core/utils/dialog-util.h"
#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"
#include "../../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../../engine/core/component/component.h"
#include "../../../../engine/core/component/component-property-reflect.h"

// 引用初始化
EditorCacheScene::EditorCacheScene() : _scene(nullptr)
{
}
/**
 * @brief 打开场景
 * @param sceneName 场景名称
 */
void EditorCacheScene::openScene(std::string path)
{
    std::cout << "EditorCacheScene::openScene: 打开场景, path: " << path << "  " << this->_scene << std::endl;
    // 清空已打开场景
    if (this->_scene != nullptr)
    {
        std::cout << "EditorCacheScene::openScene: 销毁已打开场景" << std::endl;
        this->_scene->destroy();
        delete this->_scene;
        this->_scene = nullptr;
    }
    std::cout << "EditorCacheScene::openScene: 打开场景1, path: " << path << std::endl;
    if (path.empty())
    {
        std::cout << "EditorCacheScene::openScene: 新建场景" << std::endl;
        this->_assetPath = "";
        std::string uuid = UuidUtil::generateUUID();
        this->_scene = new Scene("NewScene", uuid);
        this->_scene->createRoot3D();
        this->_scene->createRoot2D();
        this->_saveFlag = 1;
    }
    else
    {
        std::cout << "EditorCacheScene::openScene: 加载场景资产, path: " << path << std::endl;
        Asset *asset = Boo::game->assetsManager()->getAsset(path);
        if (asset == nullptr)
        {
            this->_scene = nullptr;
            std::cout << "EditorCacheScene::openScene: 场景资产不存在, path: " << path << std::endl;
            return;
        }
        SceneAsset *sceneAsset = dynamic_cast<SceneAsset *>(asset);
        if (sceneAsset == nullptr)
        {
            this->_scene = nullptr;
            std::cout << "EditorCacheScene::openScene: 场景资产不存在, path: " << path << std::endl;
            return;
        }
        this->_saveFlag = 0;
        this->_assetPath = path;
        this->_scene = sceneAsset->getScene();
    }
}
Scene *EditorCacheScene::getScene()
{
    return this->_scene;
}
void EditorCacheScene::saveScene()
{
    json sceneData;
    this->_serializeSceneData(this->_scene, sceneData);
    if (this->_assetPath.empty())
    {
        this->_assetPath = this->_preSavePath();
    }
    // 检查资产路径是否为空
    if (this->_assetPath.empty())
    {
        return;
    }
    std::string sceneSavePath = (std::filesystem::path(BooEditor::projectPath) / "assets" / this->_assetPath).generic_string();
    FileUtil::saveJsonToBinary(sceneSavePath, sceneData);
}

std::string EditorCacheScene::_preSavePath()
{
    std::string assetPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    std::string path = DialogUtil::saveFileDialog(assetPath, "scene");
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
    std::cout << "EditorCacheScene::_preSavePath: " << path << std::endl;
    std::string relativePath = std::filesystem::relative(std::filesystem::path(path), assetPath).generic_string();
    return relativePath;
}
void EditorCacheScene::_serializeSceneData(Scene *scene, json &sceneData)
{
    std::function<void(Node *, json &)> _serializeNode = [&](Node *node, json &nodeData)
    {
        // 节点数据
        nodeData["_name"] = node->getName();
        nodeData["_layer"] = node->getLayer();
        nodeData["_uuid"] = node->getUuid();
        nodeData["_visibility"] = node->getVisibility();
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
            compData["_properties"] = ReflectionRegistry::getInstance().serializeToJson((void *)comp, comp->getName());
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
    sceneData["_scene"] = json::object();
    _serializeNode(scene, sceneData["_scene"]);
}

EditorCacheScene::~EditorCacheScene()
{
}