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
    assetMateMapJson["path"] = this->_assetPath;
    // 更新存储到本地
    FileUtil::saveJsonToText(assetMateMapPath, assetMateMapJson);
    // 更新资产库
    this->_updateLibraryAsset(assetMateMapJson);
    // 更新资产缓存
    Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(assetMateMapJson["uuid"].get<std::string>(), assetMateMapJson);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}
void EditorCacheAssetDBTask::_parseMaterialAssetDB()
{
    // AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
    // std::vector<AssetDB *> materialAssetsDB = assetCache->getAssetDBByPath(this->_assetPath);
    // if (materialAssetsDB.empty() || materialAssetsDB.size() <= 0)
    // {
    //     // 没有找到资产配置
    //     // 创建资产配置
    //     // 拷贝资产资源
    //     std::cout << "EditorCacheTask::create new material asset db: " << this->_assetPath << std::endl;
    //     this->_createANewAssetDB(AssetType::Material);
    // }
    // else
    // {
    //     AssetDB *assetDB = materialAssetsDB.at(0);
    //     if (assetDB->uuid.empty() || this->_assetPath != assetDB->path || this->_assetExtension != assetDB->extension)
    //     {
    //         // 资产配置为空，或者路径和扩展名不匹配，重新赋值,更新资产配置
    //         // 拷贝资产资源
    //         this->_createANewAssetDB(AssetType::Material);
    //     }
    //     else
    //     {
    //         // 资产配置OK，无条件重新拷贝资产资源
    //         this->_updateLibraryAsset(*assetDB);
    //         // 加载资产
    //         Boo::game->assetsManager()->loadAsset(assetDB->uuid);
    //     }
    // }
}
void EditorCacheAssetDBTask::_parseSceneAssetDB()
{
    // AssetCache *assetCache = Boo::game->assetsManager()->getAssetsCache();
    // std::vector<AssetDB *> sceneAssetsDB = assetCache->getAssetDBByPath(this->_assetPath);
    // if (sceneAssetsDB.empty() || sceneAssetsDB.size() <= 0)
    // {
    //     // 没有找到资产配置
    //     // 创建资产配置
    //     // 拷贝资产资源
    //     this->_createANewAssetDB(AssetType::Scene);
    // }
    // else
    // {
    //     AssetDB *assetDB = sceneAssetsDB.at(0);
    //     if (assetDB->uuid.empty() || this->_assetPath != assetDB->path || this->_assetExtension != assetDB->extension)
    //     {
    //         // 资产配置为空，或者路径和扩展名不匹配，重新赋值,更新资产配置
    //         // 拷贝资产资源
    //         this->_createANewAssetDB(AssetType::Scene);
    //     }
    //     else
    //     {
    //         // 资产配置OK，无条件重新拷贝资产资源
    //         this->_updateLibraryAsset(*assetDB);
    //         // 加载资产
    //         Boo::game->assetsManager()->loadAsset(assetDB->uuid);
    //     }
    // }
}
// json EditorCacheAssetDBTask::_readAssetBooMapFile(const std::string &path)
// {
//     json assetBooMapJson = FileUtil::readJsonFromText(path);
//     return assetBooMapJson;
// }

void EditorCacheAssetDBTask::_updateLibraryAsset(json &assetBooMapJson)
{
    // std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / assetBooMapJson["uuid"].get<std::string>() + this->_assetExtension).generic_string();
    // FileUtil::copyFile(this->_originalPath, libraryPath);
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