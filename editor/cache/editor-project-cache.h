#pragma once
#include <vector>
#include <filesystem>
#include "../../engine/core/utils/file-util.h"

/**
 * @brief 编辑器资源缓存
 */
class EditorProjectCache
{
    json _projectCache;

public:
    EditorProjectCache();
    void init();
    void update();
    std::string getLaunchScene();
    void setLaunchScene(std::string launchScene);

    ~EditorProjectCache();
};
