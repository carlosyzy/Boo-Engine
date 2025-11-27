#include "asset-load.h"
#include "asset.h"
#include "texture-asset.h"
#include <filesystem>
#include <iostream>
#include "shader.h"
#include "assets-manager.h"
#include "asset-cache.h"
#include "asset-task.h"
#include "../utils/time-util.h"

AssetLoad::AssetLoad(AssetsManager *mgr)
{
    this->_mgr = mgr;
    this->_cache = new AssetCache();
}
Asset *AssetLoad::load(const std::string path)
{
    // 从缓存中获取资产
    std::filesystem::path key = std::filesystem::path(path);
    std::string normPath = key.generic_string();
    Asset *asset1 = this->_cache->getAsset(normPath);
    if (asset1 != nullptr)
    {
        return asset1;
    }
    int taskID = this->_TaskNextID++;
    // 创建加载任务
   
    AssetTask task(this->_mgr, this->_cache, taskID);
    Asset *asset2 = task.load(normPath);
    
    return asset2;
}

Asset *AssetLoad::getAsset(const std::string &path)
{
    std::filesystem::path key = std::filesystem::path(path);
    std::string normPath = key.generic_string();
    Asset *asset = this->_cache->getAsset(path);
    if (asset != nullptr)
    {
        return asset;
    }
    return nullptr;
}
/**
 * @brief 清除加载任务回调
 * @param loadId 加载任务ID
 */
void AssetLoad::clearLoadCall(const int loadId)
{
    for (AssetTask &task : this->_tasks)
    {
        if (task.getID() == loadId)
        {
            task.clearCallback();
        }
    }
}

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
    // 删除已经完成的任务
    auto it = std::remove_if(this->_tasks.begin(), this->_tasks.end(),
                             [](AssetTask &task)
                             {
                                 return task.isComplete();
                             });

    this->_tasks.erase(it, this->_tasks.end());
    // 执行剩余任务的run
    int loadCount = 0;
    for (AssetTask &task : this->_tasks)
    {
        task.run();
        loadCount++;
        if (loadCount >= this->_MAX_LOAD_COUNT)
        {
            break;
        }
    }
}

AssetLoad::~AssetLoad()
{
}