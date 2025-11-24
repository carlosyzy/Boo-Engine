#pragma once
#include <iostream>

class EditorConfigCache;
class EditorAssetsCache;
class EditorProjectCache;
class EditorSceneCache;

namespace BooEditor
{
    /**
     * @brief 工程路径
     */
	extern std::string projectPath;

    /**
     * @brief 资产缓存
     */
	extern EditorAssetsCache *assets;
    /**
     * @brief 配置缓存
     */
	extern EditorConfigCache *config;
    /**
     * @brief 项目缓存
     */
    extern EditorProjectCache *project;
    /**
     * @brief 场景缓存
     */
    extern EditorSceneCache *scene;
}