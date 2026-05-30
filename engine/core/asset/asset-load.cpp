#include "asset-load.h"
#include "boo.h"
#include "log.h"
#include "core/asset/asset.h"
#include "core/asset/asset-cache.h"
namespace Boo
{

    AssetLoad::AssetLoad() : _TaskNextID(0)
    {
    }
    Asset *AssetLoad::loadAsset(const std::string &assetPath)
    {
        AssetCache *cache = assetManager->getAssetsCache();
        Asset *asset = nullptr;
        asset = cache->getAsset(assetPath);
        if (asset != nullptr)
        {
            return asset;
        }
        int taskID = this->_TaskNextID++;
        AssetTask task;
        asset = task.load(assetPath);
        if (asset == nullptr)
        {
            return nullptr;
        }
        cache->addAsset(assetPath, asset);
        return asset;
    }
    /**
     * @brief 更新加载任务
     * @param deltaTime 时间间隔
     */
    void AssetLoad::update(float deltaTime)
    {
    
    }
    AssetLoad::~AssetLoad()
    {
    }

} // namespace Boo