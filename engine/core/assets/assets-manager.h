#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
enum class AssetType;
class Asset;

// 基础图片资源
class AssetsManager
{
private:
    /**
     * @brief 资产根目录
     */
    std::string _root;
    /**
     * @brief 资产映射表
     */
    std::unordered_map<std::string, Asset *> _assetsMap;
    /**
     * @brief 最大加载数量
     */
    int MAX_LOAD_COUNT = 30;
    void _initRoot();

public:
    AssetsManager();
    /**
     * @brief 初始化资产管理器
     */
    void init();
    /**
     * @brief 加载资产
     * @param path 资产路径
     * @return Asset 资产对象
     */
    Asset *load(const std::string &path);
    // /**
    //  * @brief 异步加载资产
    //  * @param path 资产路径
    //  */
    // void loadAsync(const std::string &path, std::function<void(Asset *)> callback);
    ~AssetsManager();
};