#include "asset-cache.h"
#include "assets-manager.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

AssetCache::AssetCache()
{
}
void AssetCache::initAssetsDB(const std::string &path)
{
    // key : 相对于assets 目录的路径
    // value : 当前目录包含子的子资源
    //        name: 子资源的名称
    //        uuid: 子资源的uuid
    //        type: 子资源的类型
    json content = FileUtil::readJsonFromBinary(path);
    for (auto &asset : content.items())
    {
        // 资源原始路径
        std::string path = asset.key();
        // 子资源配置数组
        json subConfigs = asset.value();
        std::vector<AssetDB> dbList;
        for (auto &config : subConfigs)
        {
            std::string name = config["name"];
            std::string path = config["path"];
            std::string uuid = config["uuid"];
            std::string extension = config["extension"];
            AssetType type = (AssetType)config["type"];
            AssetDB db;
            db.name = name;
            db.path = path;
            db.uuid = uuid;
            db.type = type;
            db.extension = extension;
            dbList.push_back(db);
            this->_uuidsDB[uuid] = db;
        }
        if (dbList.size() > 0)
        {
            this->_assetsDB[path] = dbList;
        }
    }
}
/**
 * @brief 获取资产数据库
 * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
 */
std::unordered_map<std::string, std::vector<AssetDB>> &AssetCache::getAssetsDB()
{
    return this->_assetsDB;
}
/**
 * @brief 更新资产数据库
 * @param path 资产路径
 * @param configs 资产配置
 */
void AssetCache::updateAssetsDB(const std::string path, const std::vector<AssetDB> configs)
{
    for (auto &db : configs)
    {
        this->_uuidsDB[db.uuid] = db;
    }
    this->_assetsDB[path] = configs;
}
/**
 * @brief 获取资产配置
 * @param path 资产路径
 * @return std::vector<AssetDB> 资产配置
 */
const std::vector<AssetDB> &AssetCache::getAssetDB(const std::string &path)
{
    if (this->_assetsDB.find(path) != this->_assetsDB.end())
    {
        return this->_assetsDB[path];
    }
    return std::vector<AssetDB>();
}

Asset *AssetCache::getAssetByUuid(const std::string &uuid)
{
    if (this->_assetsUuidMap.find(uuid) != this->_assetsUuidMap.end())
    {
        return this->_assetsUuidMap[uuid];
    }
    return nullptr;
}
/**
 * @brief 获取资产配置
 * @param uuid 资产uuid
 * @return AssetDB 资产配置
 */
const AssetDB &AssetCache::getAssetDBByUuid(const std::string &uuid)
{
    if (this->_uuidsDB.find(uuid) != this->_uuidsDB.end())
    {
        return this->_uuidsDB[uuid];
    }
    return AssetDB();
}
/**
 * @brief 添加资产到缓存
 *
 * @param uuid 资产uuid
 * @param asset 资产指针
 */
void AssetCache::addAssetByUuid(const std::string &uuid, Asset *asset)
{
    this->_assetsUuidMap[uuid] = asset;
}

// void AssetCache::addAsset(const std::string path, Asset *asset)
// {
//     this->_assetsMap[path] = asset;
// }

// Asset *AssetCache::getAsset(const std::string &path)
// {
//     // if (this->_assetsMap.find(path) != this->_assetsMap.end())
//     // {
//     //     return this->_assetsMap[path];
//     // }

//     return nullptr;
// }
// void AssetCache::addSceneAsset(const std::string path, Asset *asset)
// {
//     this->_sceneAssetsMap[path] = asset;
// }
// Asset *AssetCache::getSceneAsset(const std::string &path)
// {
//     if (this->_sceneAssetsMap.find(path) == this->_sceneAssetsMap.end())
//     {
//         return nullptr;
//     }
//     return this->_sceneAssetsMap[path];
// }
AssetCache::~AssetCache()
{
}