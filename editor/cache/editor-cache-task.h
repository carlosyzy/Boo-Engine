#pragma once
#include <vector>
#include <filesystem>

#include "../../engine/core/utils/file-util.h"
#include "../../engine/core/utils/json-util.h"
#include "../../engine/core/utils/uuid-util.h"

class EditorCacheTask
{
private:
    std::string _path;
    json _config;


    void _createLibraryAsset();


public:
    EditorCacheTask();
    void init(std::string path,json config);
    void run();
    ~EditorCacheTask();
};
