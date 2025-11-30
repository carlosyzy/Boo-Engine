#pragma once
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "../../engine/core/assets/asset-struct.h"
#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"
#include "../../engine/core/utils/uuid-util.h"

struct AssetDB;

class EditorCacheTask
{
private:
    /**
     * 资产原始路径
     * 全路径
     */
    std::string _originalPath;
    /**
     * 资产库路径
     * 相对路径
     */
    std::string _assetPath;
    /**
     * 资产扩展名
     */
    std::string _assetExtension;
    /**
     * 资产类型
     */
    AssetType _assetType;

    AssetType getAssetType(const std::string &path);

    void _parseTextureAssetDB();
    void _parseMaterialAssetDB();
    /**
     * @brief 更新资产
     * 拷贝资产资源到资产库
     */
    void _updateLibraryAsset(const AssetDB &db);
public:
    EditorCacheTask();
    /**
     * 相对路径
     * 父路径
     * 资产配置
     */
    void init(const std::string assetPath);
    void run();
    ~EditorCacheTask();
};
