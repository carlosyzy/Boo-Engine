#include "asset-cache.h"
#include "assets-manager.h"
#include "asset-struct.h"
#include "../utils/file-util.h"
#include "../utils/json-util.h"

AssetCache::AssetCache()
{
}
void AssetCache::initAssetsDB(const std::string &path)
{
    json content = FileUtil::readJsonFromBinary(path);
    for (auto &asset : content.items())
    {
        std::string path = asset.key();
        json value = asset.value();
        AssetType type = (AssetType)value["type"];
        if (type == AssetType::Texture)
        {
            this->_insertTextureAssetDB(path, value);
        }
        else if (type == AssetType::Material)
        {
            this->_insertMaterialAssetDB(path, value);
        }
        else
        {
            std::cout << "Unknown asset type: " << value["name"] << std::endl;
        }
    }
}
void AssetCache::_insertTextureAssetDB(const std::string &path, json assetDB)
{
    std::cout << "Insert texture asset db: " << path << std::endl;
    AssetDB db{};
    db.name = assetDB["name"];
    db.path = path;
    db.uuid = assetDB["uuid"];
    db.type = (AssetType)assetDB["type"];
    db.extension = assetDB["extension"];
    this->_textureAssetsDB[path] = db;
    this->_uuidAssetsDB[db.uuid] = db;
}
void AssetCache::_insertMaterialAssetDB(const std::string &path, json assetDB)
{
    std::cout << "Insert material asset db: " << path << std::endl;
    AssetDB db{};
    db.name = assetDB["name"];
    db.path = path;
    db.uuid = assetDB["uuid"];
    db.type = (AssetType)assetDB["type"];
    db.extension = assetDB["extension"];
    this->_materialAssetsDB[path] = db;
    this->_uuidAssetsDB[db.uuid] = db;
}
const std::unordered_map<std::string, AssetDB> &AssetCache::_getTextureAssetsDB()
{
    return this->_textureAssetsDB;
}
void AssetCache::_updateTextureAssetsDB(const std::string &path, const AssetDB &config)
{
    this->_textureAssetsDB[path] = config;
}



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

// Asset *AssetCache::getAssetByUuid(const std::string &uuid)
// {
//     if (this->_assetsUuidMap.find(uuid) != this->_assetsUuidMap.end())
//     {
//         return this->_assetsUuidMap[uuid];
//     }
//     return nullptr;
// }
// /**
//  * @brief 获取资产配置
//  * @param uuid 资产uuid
//  * @return AssetDB 资产配置
//  */
// const AssetDB &AssetCache::getAssetDBByUuid(const std::string &uuid)
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
AssetCache::~AssetCache()
{
}