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
    std::string _path;
    std::vector<AssetDB> _configs;


    bool _verifyLibraryAsset(AssetDB &config);
    void _clearLibraryAsset();
    void _createLibraryAsset();


public:
    EditorCacheTask();
    void init(std::string path, std::vector<AssetDB> config);
    void run();
    ~EditorCacheTask();
};
