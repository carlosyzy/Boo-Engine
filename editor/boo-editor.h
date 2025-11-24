#pragma once
#include <iostream>

class EditorConfigCache;
class EditorAssetsCache;

namespace BooEditor
{
    /**
     * @brief 工程路径
     */
	extern std::string project;

    /**
     * @brief 资产缓存
     */
	extern EditorAssetsCache *assets;
    /**
     * @brief 配置缓存
     */
	extern EditorConfigCache *config;
}