#pragma once
#include <string>
#include <vector>
#include <functional>
#include "asset-cache.h"
#include "asset-task.h"

class Asset;
class Shader;
class AssetsManager;

class AssetLoad
{
private:
    AssetsManager *_mgr;
    /**
     * @brief 资产缓存
     */
    AssetCache *_cache;
    /**
     * @brief 最大加载数量
     */
    int _MAX_LOAD_COUNT = 30;

    int _TaskNextID = 0;
    std::vector<AssetTask> _tasks;

public:
    AssetLoad(AssetsManager *mgr);
    void setMaxLoadCount(int count)
    {
        this->_MAX_LOAD_COUNT = count;
    };

    Asset *load(const std::string path);

    template <typename T, typename Func>
    int loadAsync(const std::string &path, Func callback, T *instance)
    {
        std::filesystem::path key = std::filesystem::path(path);
        std::string normPath = key.generic_string();
        Asset *asset = this->_cache->getAsset(normPath);
        int taskID = this->_TaskNextID++;
        if (asset != nullptr)
        {
            (instance->*callback)();
            return taskID;
        }
        AssetTask task(this->_mgr, this->_cache, taskID);
        task.loadAsync(normPath, callback, instance);
        this->_tasks.push_back(task);
        return taskID;
    }
    template <typename T, typename Func>
    int loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
    {
        AssetLoadResult *result = new AssetLoadResult();
        result->all = paths.size();
        int taskID = this->_TaskNextID++;
        for (const std::string &path : paths)
        {
            std::filesystem::path key = std::filesystem::path(path);
            std::string normPath = key.generic_string();
            Asset *asset = this->_cache->getAsset(normPath);
            if (asset != nullptr)
            {
                result->complete++;
                continue;
            }
            AssetTask task(this->_mgr, this->_cache, taskID);
            task.loadASync(normPath, result, callback, instance);
            this->_tasks.push_back(task);
        }
        // 所有任务完成
        if (result->complete >= result->all)
        {
            int complete = result->complete;
            int all = result->all;
            float progress = (float)complete / (float)all;
            (instance->*callback)(complete, all, progress);
            delete result;
            result = nullptr;
        }
        return taskID;
    }
    /**
     * @brief 清除加载任务回调
     * @param loadId 加载任务ID
     */
    void clearLoadCall(const int loadId);
    Asset *getAsset(const std::string &path);

    /**
     * @brief 更新加载任务
     * @param deltaTime 时间间隔
     */
    void update(float deltaTime);
    void updateLoadTasks();
    ~AssetLoad();
};
