#include "asset-cache.h"
#include "assets-manager.h"
#include "asset-struct.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

AssetCache::AssetCache()
{
    this->_assetMates["undefined"] = json::object();
}
/**
 * @brief 初始化资产数据库
 * @param path 资产数据库路径
 * 正式环境从指定目录获取
 */
void AssetCache::initAssetsDB(const std::string &path)
{
    // json content = FileUtil::readJsonFromBinary(path);
    // std::cout << "AssetCache::initAssetsDB: " << content << std::endl;
    // for (auto &asset : content.items())
    // {
    //     std::string path = asset.key();
    //     json value = asset.value();
    //     this->_insertAssetDB(path, value);
    // }
}
void AssetCache::_updateAssetMateCache(const std::string &uuid, const json &mate)
{
    std::string path = mate["path"].get<std::string>();
    this->_assetMates[uuid] = mate;
}
void AssetCache::_updateAssetPathCache(const std::string &uuid, const std::string &path)
{
    this->_assetPathMapUuid[path] = uuid;
}

// /**
//  * @brief 解析资产数据库
//  * @param path 资产数据库路径
//  * @param assetDB 资产数据库,理论是个数组
//  */
// void AssetCache::_insertAssetDB(const std::string &path, json assetDB)
// {
//     std::vector<AssetDB *> assetsDB;
//     for (auto &asset : assetDB)
//     {
//         AssetDB *db = new AssetDB();
//         db->name = asset["name"].get<std::string>();
//         db->path = path;
//         db->uuid = asset["uuid"].get<std::string>();
//         db->type = (AssetType)asset["type"];
//         db->extension = asset["extension"].get<std::string>();
//         this->_assetsDB[db->uuid] = db;
//         if (db->type == AssetType::Scene)
//         {
//             this->_sceneAssetsMap[db->name] = db;
//         }
//         assetsDB.push_back(db);
//     }
//     this->_pathAssetsDB[path] = assetsDB;
// }
// std::vector<AssetDB *> AssetCache::getAssetDBByPath(const std::string &path)
// {
//     if (this->_pathAssetsDB.find(path) != this->_pathAssetsDB.end())
//     {
//         return this->_pathAssetsDB[path];
//     }
//     return {};
// }
// void AssetCache::updateAssetDBByPath(const std::string &path, int index, const AssetDB &assetDB)
// {
//     if(index < 0)
//     {
//         return;
//     }
//     //通过path 更新_pathAssetsDB
//     std::vector<AssetDB *> &assetsDB = this->_pathAssetsDB[path];
//     if (index >= assetsDB.size())
//     {
//         assetsDB.resize(index + 1, nullptr);
//     }
//     if (assetsDB[index] == nullptr)
//     {
//         assetsDB[index] = new AssetDB();
//     }
//     assetsDB[index]->name = assetDB.name;
//     assetsDB[index]->path = assetDB.path;
//     assetsDB[index]->uuid = assetDB.uuid;
//     assetsDB[index]->type = assetDB.type;
//     assetsDB[index]->extension = assetDB.extension;
//     // 更新_assetsDB
//     this->_assetsDB[assetDB.uuid] = assetsDB[index];
//     if (assetDB.type == AssetType::Scene)
//     {
//         this->_sceneAssetsMap[assetDB.name] = assetsDB[index];
//     }
// }
// /**
//  * @brief 获取资产数据库
//  * @return std::unordered_map<std::string, std::vector<AssetDB *>>& 资产数据库指针数组
//  */
// std::unordered_map<std::string, std::vector<AssetDB *>> &AssetCache::getPathAssetsDB()
// {
//     return this->_pathAssetsDB;
// }

/**
 * @brief 通过资产uuid获取资产数据库
 * @param uuid 资产uuid
 * @return AssetDB* 资产数据库指针
 */
json &AssetCache::getAssetMeta(const std::string &uuid)
{
    if (this->_assetMates.find(uuid) != this->_assetMates.end())
    {
        return this->_assetMates[uuid];
    }
    return this->_assetMates["undefined"];
}

/**
 * @brief 添加资产到缓存
 * @param uuid 资产uuid
 * @param asset 资产指针
 */
void AssetCache::addAsset(const std::string &uuid, Asset *asset)
{
    this->_assets[uuid] = asset;
}
/**
 * @brief 获取资产
 * @param uuid 资产uuid
 * @return Asset* 资产指针
 */
Asset *AssetCache::getAsset(const std::string &uuid)
{
    if (this->_assets.find(uuid) != this->_assets.end())
    {
        return this->_assets[uuid];
    }
    return nullptr;
}
/**
 * @brief 通过资产路径获取资产实例指针
 * @param path 资产路径
 * @return Asset* 资产实例指针
 */
Asset *AssetCache::getAssetByPath(const std::string &path)
{
    // std::vector<AssetDB *> assetsDB = this->getAssetDBByPath(path);
    // if (assetsDB.empty())
    // {
    //     return nullptr;
    // }
    // for (auto &assetDB : assetsDB)
    // {
    //     if (assetDB->path == path)
    //     {
    //         return this->_assets[assetDB->uuid];
    //     }
    // }
    return nullptr;
}
// /**
//  * @brief 通过场景名称获取场景配置
//  * @param sceneName 场景名称
//  * @return AssetDB* 场景资产数据库指针
//  */
// AssetDB *AssetCache::getSceneAssetDB(const std::string &sceneName)
// {
//     if (sceneName.empty())
//     {
//         return nullptr;
//     }
//     if (this->_sceneAssetsMap.find(sceneName) != this->_sceneAssetsMap.end())
//     {
//         return this->_sceneAssetsMap[sceneName];
//     }
//     return nullptr;
// }

AssetCache::~AssetCache()
{
}

// void AssetCache::_insertTextureAssetDB(const std::string &path, json assetDB)
// {
//     AssetDB db{};
//     db.name = assetDB["name"].get<std::string>();
//     db.path = path;
//     db.uuid = assetDB["uuid"].get<std::string>();
//     db.type = (AssetType)assetDB["type"];
//     db.extension = assetDB["extension"].get<std::string>();
//     this->_textureAssetsDB[path] = db;
//     this->_uuidAssetsDB[db.uuid] = db;
// }
// void AssetCache::_insertMaterialAssetDB(const std::string &path, json assetDB)
// {
//     std::cout << "Insert material asset db: " << path << std::endl;
//     AssetDB db{};
//     db.name = assetDB["name"].get<std::string>();
//     db.path = path;
//     db.uuid = assetDB["uuid"].get<std::string>();
//     db.type = (AssetType)assetDB["type"];
//     db.extension = assetDB["extension"].get<std::string>();
//     this->_materialAssetsDB[path] = db;
//     this->_uuidAssetsDB[db.uuid] = db;
// }
// void AssetCache::_insertSceneAssetDB(const std::string &path, json assetDB)
// {
//     // AssetDB db{};
//     // db.name = assetDB["name"].get<std::string>();
//     // db.path = path;
//     // db.uuid = assetDB["uuid"].get<std::string>();
//     // db.type = (AssetType)assetDB["type"];
//     // db.extension = assetDB["extension"].get<std::string>();
//     // this->_sceneAssetsDB[path] = db;
//     // this->_uuidAssetsDB[db.uuid] = db;
// }

// const std::unordered_map<std::string, AssetDB> &AssetCache::_getTextureAssetsDB()
// {
//     return this->_textureAssetsDB;
// }
// void AssetCache::_updateTextureAssetsDB(const std::string &path, const AssetDB &config)
// {
//     this->_textureAssetsDB[path] = config;
//     this->_uuidAssetsDB[config.uuid] = config;
// }

// /**
//  * @brief 获取资产配置
//  * @param uuid 资产uuid
//  * @return AssetDB 资产配置
//  */
// const AssetDB &AssetCache::getAssetDB(const std::string &uuid)
// {
//     return this->_uuidAssetsDB[uuid];
// }

// const std::unordered_map<std::string, AssetDB> &AssetCache::getTextureAssetsDB()
// {
//     return this->_textureAssetsDB;
// }

// void AssetCache::updateTextureAssetsDB(const std::string &path, const AssetDB &config)
// {
//     this->_textureAssetsDB[path] = config;
//     this->_uuidAssetsDB[config.uuid] = config;
// }
// const std::unordered_map<std::string, AssetDB> &AssetCache::getAudioAssetsDB()
// {
//     return this->_audioAssetsDB;
// }
// /**
//  * @brief 更新音频资产数据库
//  * @param path 资产路径
//  * @param configs 资产配置
//  */
// void AssetCache::updateAudioAssetsDB(const std::string &path, const AssetDB &config)
// {
//     this->_audioAssetsDB[path] = config;
//     this->_uuidAssetsDB[config.uuid] = config;
// }
// const std::unordered_map<std::string, AssetDB> &AssetCache::getMaterialAssetsDB()
// {
//     return this->_materialAssetsDB;
// }
// /**
//  * @brief 更新材质资产数据库
//  * @param path 资产路径
//  * @param configs 资产配置
//  */
// void AssetCache::updateMaterialAssetsDB(const std::string &path, const AssetDB &config)
// {
//     this->_materialAssetsDB[path] = config;
//     this->_uuidAssetsDB[config.uuid] = config;
// }

// /**
//  * @brief 获取资产数据库
//  * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
//  */
// std::unordered_map<std::string, std::vector<AssetDB>> &AssetCache::getAssetsDB()
// {
//     return this->_assetsDB;
// }
// /**
//  * @brief 更新资产数据库
//  * @param path 资产路径
//  * @param configs 资产配置
//  */
// void AssetCache::updateAssetsDB(const std::string path, const std::vector<AssetDB> configs)
// {
//     for (auto &db : configs)
//     {
//         this->_uuidsDB[db.uuid] = db;
//     }
//     this->_assetsDB[path] = configs;
// }
// /**
//  * @brief 获取资产配置
//  * @param path 资产路径
//  * @return std::vector<AssetDB> 资产配置
//  */
// const std::vector<AssetDB> &AssetCache::getAssetDB(const std::string &path)
// {
//     if (this->_assetsDB.find(path) != this->_assetsDB.end())
//     {
//         return this->_assetsDB[path];
//     }
//     return std::vector<AssetDB>();
// }

// /**
//  * @brief 获取资产配置
//  * @param uuid 资产uuid
//  * @return AssetDB 资产配置
//  */
// const AssetDB &AssetCache::getAssetDB(const std::string &uuid)
// {
//     if (this->_uuidsDB.find(uuid) != this->_uuidsDB.end())
//     {
//         return this->_uuidsDB[uuid];
//     }
//     return AssetDB();
// }
// /**
//  * @brief 添加资产到缓存
//  *
//  * @param uuid 资产uuid
//  * @param asset 资产指针
//  */
// void AssetCache::addAssetByUuid(const std::string &uuid, Asset *asset)
// {
//     this->_assetsUuidMap[uuid] = asset;
// }

// // void AssetCache::addAsset(const std::string path, Asset *asset)
// // {
// //     this->_assetsMap[path] = asset;
// // }

// // Asset *AssetCache::getAsset(const std::string &path)
// // {
// //     // if (this->_assetsMap.find(path) != this->_assetsMap.end())
// //     // {
// //     //     return this->_assetsMap[path];
// //     // }

// //     return nullptr;
// // }
// // void AssetCache::addSceneAsset(const std::string path, Asset *asset)
// // {
// //     this->_sceneAssetsMap[path] = asset;
// // }
// // Asset *AssetCache::getSceneAsset(const std::string &path)
// // {
// //     if (this->_sceneAssetsMap.find(path) == this->_sceneAssetsMap.end())
// //     {
// //         return nullptr;
// //     }
// //     return this->_sceneAssetsMap[path];
// // }
