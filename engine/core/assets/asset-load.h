#pragma once
#include <string>
class Asset;
class Shader;
class AssetsManager;
class AssetsTask;
class AssetCache;

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
    int MAX_LOAD_COUNT = 30;

    

public:
    AssetLoad(AssetsManager *mgr);
    Asset *load(const std::string path);
    Asset *getAsset(const std::string &path);
    ~AssetLoad();
};




