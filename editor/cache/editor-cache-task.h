#pragma once
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"
#include "../../engine/core/utils/uuid-util.h"

struct AssetDB;

class EditorCacheTask
{
private:
    std::string _relativePath;
    std::string _parentPath;
    std::vector<AssetDB> _configs;

    bool _verifyLibraryAsset(AssetDB &config);
    void _clearLibraryAsset();
    void _createLibraryAsset();

public:
    EditorCacheTask();
    /**
     * 相对路径
     * 父路径
     * 资产配置
     */
    void init(std::string relativePath, std::string parentPath, std::vector<AssetDB> configs);
    void run();
    ~EditorCacheTask();
};
