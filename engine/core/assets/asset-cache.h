#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
enum class AssetType;
class Asset;
class AssetsManager;
/**
 * @brief 资产缓存类
 */
class AssetCache
{
private:
    /**
	 * @brief 资产映射表
	 */
	std::unordered_map<std::string, Asset*> _assetsMap;
public:
    AssetCache();
    /**
     * @brief 添加资产到缓存
     * 
     * @param path 资产路径
     * @param asset 资产指针
     */
    void addAsset(const std::string path, Asset *asset);
    Asset *getAsset(const std::string &path);
    ~AssetCache();
};

