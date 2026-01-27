#pragma once
#include <string>
#include <filesystem>
#include <functional>
#include "../../../../engine/core/assets/asset-struct.h"
#include "editor-cache-assets-task.h"

class EditorCacheAssets
{
private:
    std::string _assetsPath;

    std::vector<EditorCacheAssetsTask> _initAssetsTasks;
    std::function<void(const float progress, std::string file)> _progressCallback;
    std::function<void()> _completeCallback;
    int _initAssetsTaskComplete = 0;
    int _initAssetsTaskAll = 0;
public:
    EditorCacheAssets();
    void init(std::string assetsPath);
    void load(std::function<void(const float progress, std::string file)> progress, std::function<void()> complete);
    void update(float deltaTime);
    ~EditorCacheAssets();
};




