#pragma once
#include <vector>
#include <filesystem>
#include "../../engine/core/utils/file-util.h"

/**
 * @brief 编辑器资源缓存
 */
class EditorAssetsCache
{
    json _assetsCache;

    /**
     * @brief 插入新资源
     * 
     * @param relativePath 资源路径
     */
    void _insertNewAsset(std::vector<std::string> &existAssets);
    /**
     * @brief 删除不存在的资源
     * 
     */
    void _delNotExistAssets(std::vector<std::string> &existAssets);
    /**
     * @brief 获取资源列表
     * 
     * @return std::vector<std::string> 资源列表
     */
    std::vector<std::string>  _getAssetsList();
public:
    EditorAssetsCache();
    void init();
    void reset();
    ~EditorAssetsCache();
};
