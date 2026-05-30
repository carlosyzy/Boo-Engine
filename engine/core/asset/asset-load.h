#pragma once
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <iostream>
#include "core/asset/asset-task.h"
namespace Boo
{
    class Asset;
    class AssetLoad
    {
    private:
        int _TaskNextID = 0;
        std::vector<AssetTask> _tasks;

    public:
        AssetLoad();
        Asset *loadAsset(const std::string &path);
        /**
         * @brief 更新加载任务
         * @param deltaTime 时间间隔
         */
        void update(float deltaTime);
        ~AssetLoad();
    };

} // namespace Boo
