#pragma once
#include <iostream>

#include "cache/editor-cache.h"

namespace BooEditor
{
    
    /**
     * @brief 编辑器路径
     */
	extern std::string editorPath;

    /**
     * @brief 工程路径
     */
	extern std::string projectPath;

    /**
     * @brief 工程缓存
     */
    extern EditorCache *cache;

    // /**
    //  * @brief 资产缓存
    //  */
	// extern EditorAssetsCache *assets;
    // /**
    //  * @brief 配置缓存
    //  */
	// extern EditorConfigCache *config;
    // /**
    //  * @brief 项目缓存
    //  */
    // extern EditorProjectCache *project;
    // /**
    //  * @brief 场景缓存
    //  */
    // extern EditorSceneCache *scene;
}