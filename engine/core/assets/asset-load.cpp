#include "asset-load.h"
#include "asset.h"
#include "texture-asset.h"
#include <filesystem>
#include <iostream>
#include "shader-asset.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "asset-task.h"


AssetLoad::AssetLoad(AssetsManager *mgr)
{
    this->_mgr = mgr;
}
Asset *AssetLoad::loadAsset(const std::string &path)
{
    AssetCache *cache = this->_mgr->getAssetsCache();
    Asset *asset = nullptr;
    asset = cache->getAsset(path);
    if (asset != nullptr)
    {
        return asset;
    }
    int taskID = this->_TaskNextID++;
    AssetTask task(this->_mgr, taskID);
    asset = task.load(path);
    if(asset == nullptr){
        return nullptr;
    }
    cache->addAsset(path,asset);
    return asset;
}
// Asset *AssetLoad::loadAssetByPath(const std::string &path)
// {
//     // AssetCache *cache = this->_mgr->getAssetsCache();
//     // Asset *asset = nullptr;
//     // asset = cache->getAssetByPath(path);
//     // if (asset != nullptr)
//     // {
//     //     return asset;
//     // }
//     // int taskID = this->_TaskNextID++;
//     // AssetTask task(this->_mgr, taskID);
//     // AssetDB *db = cache->getAssetDBByPath(path);
//     // if(db == nullptr){
//     //     return nullptr;
//     // }
//     // asset = task.load(db);
//     // cache->addAsset(path,asset);
//     // return asset;
//     return nullptr;
// }


// Asset *AssetLoad::load(const std::string path)
// {
//     // 从缓存中获取资产
//     std::filesystem::path _path = std::filesystem::path(path).generic_string();
//     Asset *asset = nullptr;
//     // 从缓存中获取资产
//     asset = this->_cache->getAsset(_path.string());
//     if(asset != nullptr){
//         return asset;
//     }

//     // 当前资源映射文件
//     std::vector<AssetDB> assetConfigs = this->_cache->getAssetsDB()[_path.string()];
//     if (assetConfigs.size() <= 0)
//     {
//         return nullptr;
//     }

//     //
//     // if (asset != nullptr)
//     // {
//     //     return asset;
//     // }
//     // std::string normPath = tempPath.generic_string();
//     // Asset *asset1 = this->_cache->getAsset(normPath);
//     // if (asset1 != nullptr)
//     // {
//     //     return asset1;
//     // }
//     // int taskID = this->_TaskNextID++;
//     // // 创建加载任务

//     // AssetTask task(this->_mgr, this->_cache, taskID);
//     // Asset *asset2 = task.load(normPath);

//     // return asset2;
// }
// /**
//  * @brief 获取资产
//  * @param path 资产路径 相对于assets 目录的路径
//  * @return Asset* 资产指针
//  */
// Asset *AssetLoad::getAsset(const std::string &path)
// {
//     std::filesystem::path key = std::filesystem::path(path);
//     std::string normPath = key.generic_string();
//     Asset *asset = this->_cache->getAsset(path);
//     if (asset != nullptr)
//     {
//         return asset;
//     }
//     return nullptr;
// }
// /**
//  * @brief 清除加载任务回调
//  * @param loadId 加载任务ID
//  */
// void AssetLoad::clearLoadCall(const int loadId)
// {
//     for (AssetTask &task : this->_tasks)
//     {
//         if (task.getID() == loadId)
//         {
//             task.clearCallback();
//         }
//     }
// }

/**
 * @brief 更新加载任务
 * @param deltaTime 时间间隔
 */
void AssetLoad::update(float deltaTime)
{
    this->updateLoadTasks();
}
void AssetLoad::updateLoadTasks()
{
    // // 删除已经完成的任务
    // auto it = std::remove_if(this->_tasks.begin(), this->_tasks.end(),
    //                          [](AssetTask &task)
    //                          {
    //                              return task.isComplete();
    //                          });

    // this->_tasks.erase(it, this->_tasks.end());
    // // 执行剩余任务的run
    // int loadCount = 0;
    // for (AssetTask &task : this->_tasks)
    // {
    //     task.run();
    //     loadCount++;
    //     if (loadCount >= this->_MAX_LOAD_COUNT)
    //     {
    //         break;
    //     }
    // }
}

AssetLoad::~AssetLoad()
{
}