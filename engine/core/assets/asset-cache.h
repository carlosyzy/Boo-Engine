#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <filesystem>
#include <functional>
#include "asset-struct.h"
#include "../../core/utils/json-util.h"

enum class AssetType;
class Asset;
class TextureAsset;
class AssetsManager;

/**
 * @brief 资产缓存类
 */
class AssetCache
{
private:
    /**
     * 全局资产映射
     * key: 资产路径 全局唯一
     * value : 资产实例指针
     */
    std::unordered_map<std::string, Asset *> _assets;

public:
    AssetCache();
    /**
     * @brief 添加资产到缓存
     * @param uuid 资产uuid
     * @param asset 资产实例指针
     */
    void addAsset(const std::string &path, Asset *asset);
    /**
     * @brief 通过资产uuid获取资产实例指针
     * @param uuid 资产uuid
     * @return Asset* 资产实例指针
     */
    Asset *getAsset(const std::string &path);

    ~AssetCache();
};
