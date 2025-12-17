#include "editor-cache-assets-db-task.h"
#include "../../boo-editor.h"

#include "../../../engine/boo.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"
#include "../../../engine/core/assets/asset-cache.h"
#include "../../../engine/core/assets/asset-struct.h"
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/uuid-util.h"

EditorCacheAssetDBTask::EditorCacheAssetDBTask()
{
}
void EditorCacheAssetDBTask::init(const std::string originalPath)
{
    this->_originalPath = originalPath;
    this->_assetPath = std::filesystem::relative(this->_originalPath, std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
    this->_assetExtension = std::filesystem::path(this->_originalPath).extension().generic_string();
    this->_assetType = this->getAssetType(this->_originalPath);
}
/**
 * @brief 获取资产路径
 * @return std::string 资产路径
 */
std::string EditorCacheAssetDBTask::getAssetPath()
{
    return this->_assetPath;
}
void EditorCacheAssetDBTask::run()
{
    if (this->_assetType == AssetType::Texture)
    {
        this->_parseTextureAssetDB();
    }
    else if (this->_assetType == AssetType::Material)
    {
        this->_parseMaterialAssetDB();
    }
    else if (this->_assetType == AssetType::Scene)
    {
        this->_parseSceneAssetDB();
    }
}
void EditorCacheAssetDBTask::_parseTextureAssetDB()
{
    // 读取对应的.mate文件
    std::string assetMateMapPath = this->_originalPath + ".mate";
    json assetMateMapJson = FileUtil::readJsonFromText(assetMateMapPath);
    if (assetMateMapJson.empty() || !assetMateMapJson.contains("uuid") || assetMateMapJson["uuid"].is_null())
    {
        // 没有找到资产配置  // 创建资产配置  // 拷贝资产资源
        assetMateMapJson["uuid"] = UuidUtil::generateUUID();
    }
    assetMateMapJson["type"] = uint32_t(AssetType::Texture);
    assetMateMapJson["name"] = std::filesystem::path(this->_assetPath).stem().generic_string();
    assetMateMapJson["extension"] = this->_assetExtension;
    // 更新存储到本地
    FileUtil::saveJsonToText(assetMateMapPath, assetMateMapJson);
    // 更新资产库
    this->_updateLibraryAsset(assetMateMapJson);
    // 更新资产缓存
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(assetMateMapJson["uuid"].get<std::string>(), assetMateMapJson);
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetPathCache(assetMateMapJson["uuid"].get<std::string>(), this->_assetPath);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}
void EditorCacheAssetDBTask::_parseMaterialAssetDB()
{
    // 读取对应的.mate文件
    std::string assetMateMapPath = this->_originalPath + ".mate";
    json assetMateMapJson = FileUtil::readJsonFromText(assetMateMapPath);
    if (assetMateMapJson.empty() || !assetMateMapJson.contains("uuid") || assetMateMapJson["uuid"].is_null())
    {
        // 没有找到资产配置  // 创建资产配置  // 拷贝资产资源
        assetMateMapJson["uuid"] = UuidUtil::generateUUID();
    }
    assetMateMapJson["type"] = uint32_t(AssetType::Material);
    assetMateMapJson["name"] = std::filesystem::path(this->_assetPath).stem().generic_string();
    assetMateMapJson["extension"] = this->_assetExtension;
    // 更新存储到本地
    FileUtil::saveJsonToText(assetMateMapPath, assetMateMapJson);
    // 更新资产库
    this->_updateLibraryAsset(assetMateMapJson);
    // 更新资产缓存
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(assetMateMapJson["uuid"].get<std::string>(), assetMateMapJson);
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetPathCache(assetMateMapJson["uuid"].get<std::string>(), this->_assetPath);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}
void EditorCacheAssetDBTask::_parseSceneAssetDB()
{
    // 读取对应的.mate文件
    std::string assetMateMapPath = this->_originalPath + ".mate";
    json assetMateMapJson = FileUtil::readJsonFromText(assetMateMapPath);
    if (assetMateMapJson.empty() || !assetMateMapJson.contains("uuid") || assetMateMapJson["uuid"].is_null())
    {
        // 没有找到资产配置  // 创建资产配置  // 拷贝资产资源
        assetMateMapJson["uuid"] = UuidUtil::generateUUID();
    }
    assetMateMapJson["type"] = uint32_t(AssetType::Scene);
    assetMateMapJson["name"] = std::filesystem::path(this->_assetPath).stem().generic_string();
    assetMateMapJson["extension"] = this->_assetExtension;
    // 更新存储到本地
    FileUtil::saveJsonToText(assetMateMapPath, assetMateMapJson);
    // 更新资产库
    this->_updateLibraryAsset(assetMateMapJson);
    // 更新资产缓存
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(assetMateMapJson["uuid"].get<std::string>(), assetMateMapJson);
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetPathCache(assetMateMapJson["uuid"].get<std::string>(), this->_assetPath);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}

void EditorCacheAssetDBTask::_updateLibraryAsset(json &assetBooMapJson)
{
    std::string fileName = assetBooMapJson["uuid"].get<std::string>() + this->_assetExtension;
    std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / fileName).generic_string();
    std::cout << "EditorCacheAssetDBTask::_updateLibraryAsset0: " << this->_originalPath << std::endl;
    std::cout << "EditorCacheAssetDBTask::_updateLibraryAsset1: " << libraryPath << std::endl;
    FileUtil::copyFile(this->_originalPath, libraryPath);
}
AssetType EditorCacheAssetDBTask::getAssetType(const std::string &path)
{
    std::string extension = std::filesystem::path(path).extension().generic_string();
    if (extension == ".png" || extension == ".PNG" || extension == ".jpg" || extension == ".JPG" || extension == ".jpeg" || extension == ".JPEG")
    {
        return AssetType::Texture;
    }
    else if (extension == ".mtl" || extension == ".Mtl" || extension == ".MTL")
    {
        return AssetType::Material;
    }
    else if (extension == ".scene" || extension == ".Scene" || extension == ".SCENE")
    {
        return AssetType::Scene;
    }
    else
    {
        return AssetType::None;
    }
    return AssetType::None;
}
// void EditorCacheAssetDBTask::_createANewAssetDB(AssetType assetType)
// {
//     std::cout << "EditorCacheTask::create new asset db: " << this->_assetPath << std::endl;
//     AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
//     AssetDB newAssetDB{};
//     newAssetDB.path = this->_assetPath;
//     newAssetDB.extension = this->_assetExtension;
//     newAssetDB.uuid = UuidUtil::generateUUID();
//     newAssetDB.type = assetType;
//     newAssetDB.name = std::filesystem::path(this->_assetPath).stem().generic_string();
//     assetCache->updateAssetDBByPath(this->_assetPath, 0, newAssetDB);
//     this->_updateLibraryAsset(newAssetDB);
//     Boo::game->assetsManager()->loadAsset(newAssetDB.uuid);
// }

EditorCacheAssetDBTask::~EditorCacheAssetDBTask()
{
}