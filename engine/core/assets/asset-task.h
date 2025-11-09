
#pragma once
#include <string>

class Asset;
class Shader;
class AssetsManager;
class AssetCache;

struct AssetLoadResult
{
    /**
     * @brief 所有资产数量
     */
    int all = 0;
    /**
     * @brief 已完成资产数量
     */
    int complete = 0;
    /**
     * @brief 加载进度
     */
    float progress = 0.0f;
};

/**
 * @brief 资产加载任务
 */
class AssetTask
{
private:
    AssetsManager *_mgr;
    /**
     * @brief 资产缓存
     */
    AssetCache *_cache;
    /**
     * @brief 资产路径
     */
    std::string _path;
public:
    AssetTask(AssetsManager *mgr, AssetCache *cache);
    Asset *load(const std::string &path);
    void load(const std::string path, AssetLoadResult *result);
    Asset* run();
    ~AssetTask();
};