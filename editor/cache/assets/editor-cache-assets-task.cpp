#include "editor-cache-assets-task.h"
#include "../../boo-editor.h"
#include "editor-asset-util.h"

#include "../../../engine/boo.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"
#include "../../../engine/core/assets/asset-cache.h"
#include "../../../engine/core/assets/asset-struct.h"
#include "../../../engine/core/utils/file-util.h"
#include "../../../engine/core/utils/uuid-util.h"

EditorCacheAssetsTask::EditorCacheAssetsTask()
{
}
void EditorCacheAssetsTask::init(const std::string originalPath)
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
std::string EditorCacheAssetsTask::getAssetPath()
{
    return this->_assetPath;
}
void EditorCacheAssetsTask::run()
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
void EditorCacheAssetsTask::_parseTextureAssetDB()
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
    // 更新资产元数据
    EditorAssetUtil::updateAssetMeta(this->_assetPath, assetMateMapJson);
    // 更新资产库
    EditorAssetUtil::updateLibraryAsset(this->_assetPath, assetMateMapJson);
    // 更新资产缓存
    EditorAssetUtil::updateAssetMap(this->_assetPath, assetMateMapJson);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}
void EditorCacheAssetsTask::_parseMaterialAssetDB()
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
    assetMateMapJson["path"] = this->_assetPath;
    // 更新资产元数据
    EditorAssetUtil::updateAssetMeta(this->_assetPath, assetMateMapJson);
    // 更新资产库
    EditorAssetUtil::updateLibraryAsset(this->_assetPath, assetMateMapJson);
    // 更新资产缓存
    EditorAssetUtil::updateAssetMap(this->_assetPath, assetMateMapJson);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}
void EditorCacheAssetsTask::_parseSceneAssetDB()
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
    assetMateMapJson["path"] = this->_assetPath;
    
    // 更新资产元数据
    EditorAssetUtil::updateAssetMeta(this->_assetPath, assetMateMapJson);
    // 更新资产库
    EditorAssetUtil::updateLibraryAsset(this->_assetPath, assetMateMapJson);
    // 更新资产缓存
    EditorAssetUtil::updateAssetMap(this->_assetPath, assetMateMapJson);
    // 加载资产
    Boo::game->assetsManager()->loadAsset(assetMateMapJson["uuid"].get<std::string>());
}

void EditorCacheAssetsTask::_updateLibraryAsset(json &assetBooMapJson)
{
    std::string fileName = assetBooMapJson["uuid"].get<std::string>() + this->_assetExtension;
    std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / fileName).generic_string();
    FileUtil::copyFile(this->_originalPath, libraryPath);
}
AssetType EditorCacheAssetsTask::getAssetType(const std::string &path)
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

EditorCacheAssetsTask::~EditorCacheAssetsTask()
{
}