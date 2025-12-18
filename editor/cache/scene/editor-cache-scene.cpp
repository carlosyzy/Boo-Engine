#include "editor-cache-scene.h"
#include "../../boo-editor.h"

#include "../../../engine/boo.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"
#include "../../../engine/core/assets/asset-cache.h"
#include "../../../engine/core/assets/asset-struct.h"
#include "../../../engine/core/assets/scene-asset.h"
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/uuid-util.h"
#include "../../../engine/core/utils/dialog-util.h"
#include "../../../engine/core/scene/scene.h"
#include "../../../engine/core/scene/node.h"
#include "../../../engine/core/scene/node-2d.h"
#include "../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../engine/core/component/component.h"
#include "../../../engine/core/component/component-property-reflect.h"

// 引用初始化
EditorCacheScene::EditorCacheScene(json *settingConfig)
{
    this->_settingConfig = settingConfig;
    std::cout << "EditorCacheScene::EditorCacheScene" << std::endl;
    this->_initSceneConfig();
}
void EditorCacheScene::_initSceneConfig()
{
    std::cout << "EditorCacheScene::_initSceneConfig:" << *this->_settingConfig << std::endl;
    if (!this->_settingConfig->contains("scene"))
    {
        (*this->_settingConfig)["scene"] = "";
    }
    std::cout << "EditorCacheScene::_initSceneConfig: scene: " << (*this->_settingConfig)["scene"].get<std::string>() << std::endl;
}
/**
 * @brief 打开场景
 * @param sceneName 场景名称
 */
void EditorCacheScene::openScene(std::string sceneName)
{
    // 获取当前打开场景资产数据库
    std::cout << "EditorCacheScene::openScene: sceneName: " << sceneName << std::endl;
    std::string currentScene = sceneName;
    if (sceneName.empty() || !this->_settingConfig->contains("scene") || this->_settingConfig->at("scene").empty())
    {
        currentScene = ""; // 打开一个空场景
    }
    else
    {
        currentScene = this->_settingConfig->at("scene").get<std::string>();
    }
    std::cout << "EditorCacheScene::openScene: sceneName: " << sceneName << ", currentScene: " << currentScene << std::endl;
    json *sceneMate = Boo::game->assetsManager()->_getSceneAssetMate(currentScene);
    if (sceneMate == nullptr)
    {
        this->_openNewScene();
    }
    else
    {
        this->_currentSceneAssetMate = sceneMate;
        this->_openOldScene();
    }
}
void EditorCacheScene::_openNewScene()
{
    std::string name = "New Scene";
    std::string uuid = UuidUtil::generateUUID();
    this->_tempNewSceneAssetMate = json::object();
    this->_tempNewSceneAssetMate["name"] = name;
    this->_tempNewSceneAssetMate["uuid"] = uuid;
    this->_tempNewSceneAssetMate["path"] = "";
    this->_tempNewSceneAssetMate["type"] = uint32_t(AssetType::Scene);
    this->_tempNewSceneAssetMate["extension"] = "";

    this->_scene = new Scene(name, uuid);
    // 测试代码
    Node2D *node2d = this->_scene->getRoot2D();
    Node2D *ndMain = new Node2D("Editor-Main");
    ndMain->setSize(100.0f, 100.0f);
    node2d->addChild(ndMain);
    UISprite *spriteMain = dynamic_cast<UISprite *>(ndMain->addComponent("UISprite"));
    if (spriteMain != nullptr)
    {
        std::cout << "EditorLayout::_initMainUI1" << std::endl;
        // spriteMain->setTextureAsset("resources/texture/ic-default.png");
        // spriteMain->setMaterialAsset(nullptr);
        spriteMain->setColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    this->_saveFlag++;
}
void EditorCacheScene::_openOldScene()
{
    std::string uuid = this->_currentSceneAssetMate->at("uuid").get<std::string>();
    Asset *asset = Boo::game->assetsManager()->getAsset(uuid);
    if (asset == nullptr)
    {
        this->_scene = nullptr;
        // 当前资产对应的场景不存在,删除当前资产数据库,打开一个新场景
        std::cout << "EditorCacheScene::_openOldScene: 场景不存在, uuid: " << uuid << ", name: " << this->_currentSceneAssetMate->at("name").get<std::string>() << std::endl;
        return;
    }
    SceneAsset *sceneAsset = dynamic_cast<SceneAsset *>(asset);
    if (sceneAsset == nullptr)
    {
        this->_scene = nullptr;
        std::cout << "EditorCacheScene::_openOldScene: 场景资产不存在, uuid: " << uuid << ", name: " << this->_currentSceneAssetMate->at("name").get<std::string>() << std::endl;
        return;
    }
    this->_saveFlag = 0;
    this->_scene = sceneAsset->getScene();
}
Scene *EditorCacheScene::getScene()
{
    return this->_scene;
}

void EditorCacheScene::saveScene()
{
    std::cout << "EditorCacheScene::saveScene 1" << std::endl;
    if (this->_saveFlag == 0)
    {
        return;
    }
    this->_saveFlag = 0;
    std::string relativePath = "";
    if (this->_currentSceneAssetMate == nullptr)
    {
        relativePath = this->_preSavePath();
        std::string name = std::filesystem::path(relativePath).stem().generic_string();
        this->_tempNewSceneAssetMate["path"] = relativePath;
        this->_tempNewSceneAssetMate["name"] = name;
        this->_tempNewSceneAssetMate["extension"] = ".scene";

        this->_scene->setName(name);
    }
    else
    {
        relativePath = Boo::game->assetsManager()->getAssetsCache()->_getAssetPathByUuid(this->_currentSceneAssetMate->at("uuid").get<std::string>());
    }
    if (relativePath.empty())
    {
        return;
    }
    std::cout << "EditorCacheScene::_preSavePath: " << relativePath << std::endl;
    json sceneData;
    this->_serializeSceneData(this->_scene, sceneData);
    std::cout << "EditorSceneCache::_saveScene: " << sceneData << std::endl;
    std::string scenePath = (std::filesystem::path(BooEditor::projectPath) / "assets" / relativePath).generic_string();
    FileUtil::saveJsonToBinary(scenePath, sceneData);
    if (this->_currentSceneAssetMate == nullptr)
    {
        std::cout << "EditorCacheScene::_saveScene: scenePath: 1" << std::endl;
        // 更新资产对应的mate文件,
        std::string assetMateMapPath = scenePath + ".mate";
        // 更新存储到本地
        FileUtil::saveJsonToText(assetMateMapPath, this->_tempNewSceneAssetMate);
        // 更新资产库
        std::string fileName = this->_tempNewSceneAssetMate["uuid"].get<std::string>() + ".scene";
        std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / fileName).generic_string();
        FileUtil::copyFile(scenePath, libraryPath);

        std::cout << "EditorCacheScene::_saveScene: scenePath: 2" << std::endl;

        // 更新资产缓存
        Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(this->_tempNewSceneAssetMate["uuid"].get<std::string>(), this->_tempNewSceneAssetMate);
        Boo::game->assetsManager()->getAssetsCache()->_updateAssetPathCache(this->_tempNewSceneAssetMate["uuid"].get<std::string>(), relativePath);
        // 加载资产
        Boo::game->assetsManager()->loadAsset(this->_tempNewSceneAssetMate["uuid"].get<std::string>());

        std::cout << "EditorCacheScene::_saveScene: scenePath: 3" << std::endl;

        this->_currentSceneAssetMate = Boo::game->assetsManager()->_getSceneAssetMate(this->_tempNewSceneAssetMate["uuid"].get<std::string>());

        std::cout << "EditorCacheScene::_saveScene: scenePath: 31: " << this->_currentSceneAssetMate<< std::endl;
        std::cout << "EditorCacheScene::_saveScene: scenePath: 32: " << (*this->_currentSceneAssetMate)<< std::endl;
    }
    std::cout << "EditorCacheScene::_saveScene: scenePath: 4: " << (*this->_currentSceneAssetMate)<< std::endl;
    (*this->_settingConfig)["scene"] = this->_currentSceneAssetMate->at("name").get<std::string>();
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

// /**
//  * @brief 获取当前场景
//  * @return std::string 当前场景
//  */
// std::string EditorCacheScene::getCurrentScene()
// {

//     return this->_settingConfig["scene"]["current"];
// }
// /**
//  * @brief 设置当前场景
//  * @param scene 场景
//  */
// void EditorCacheScene::setCurrentScene(std::string scene)
// {
//     this->_settingConfig["scene"]["current"] = scene;
// }
// void EditorSceneCache::save()
// {
//     if (this->_saveFlag == 0)
//     {
//         return;
//     }
//     this->_saveFlag = 0;
//     std::string path = this->_preSavePath();
//     if (path.empty())
//     {
//         return;
//     }
//     this->_scenePath = std::filesystem::relative(std::filesystem::path(path), std::filesystem::path(BooEditor::projectPath)).generic_string();
//     json sceneData;
//     this->_serializeSceneData(this->_scene, sceneData);
//     std::cout << "EditorSceneCache::_saveScene: " << sceneData << std::endl;
//     FileUtil::saveJsonToBinary(path, sceneData);
// }

// void EditorSceneCache::_serializeSceneData(Scene *scene, json &sceneData)
// {
//     std::function<void(Node *, json &)> _serializeNode = [&](Node *node, json &nodeData)
//     {
//         // 节点数据
//         nodeData["_name"] = node->getName();
//         nodeData["_layer"] = node->getLayer();
//         nodeData["_uuid"] = node->getUuid();
//         nodeData["_visible"] = 0;
//         nodeData["_active"] = node->isActive() ? 1 : 0;
//         nodeData["_position"] = {node->getPosition().getX(), node->getPosition().getY(), node->getPosition().getZ()};
//         nodeData["_rotation"] = {node->getEulerAngles().getX(), node->getEulerAngles().getY(), node->getEulerAngles().getZ()};
//         nodeData["_scale"] = {node->getScale().getX(), node->getScale().getY(), node->getScale().getZ()};
//         if (node->getLayer() == NodeLayer::Node2D && dynamic_cast<Node2D *>(node) != nullptr)
//         {
//             Node2D *node2d = dynamic_cast<Node2D *>(node);
//             if (node2d != nullptr)
//             {
//                 nodeData["_size"] = {node2d->getSize().getWidth(), node2d->getSize().getHeight()};
//             }
//         }
//         // 组件数据
//         json compsData = json::array();
//         for (Component *comp : node->getComponents())
//         {
//             json compData;
//             compData["_name"] = comp->getName();
//             compData["_uuid"] = comp->getUuid();
//             compData["_enable"] = comp->isEnabled() ? 1 : 0;
//             compData = ReflectionRegistry::getInstance().serializeToJson((void *)comp, comp->getName());
//             compsData.push_back(compData);
//         }
//         nodeData["_components"] = compsData;
//         for (Node *child : node->getChildren())
//         {
//             json childData;
//             _serializeNode(child, childData);
//             nodeData["_children"].push_back(childData);
//         }
//     };
//     sceneData["_name"] = scene->getName();
//     sceneData["_type"] = "SceneAsset";
//     sceneData["_node"] = json::object();
//     _serializeNode(scene, sceneData["_node"]);
// }
// std::string EditorSceneCache::getSceneName()
// {
//     return this->_sceneName;
// }