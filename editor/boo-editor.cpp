#include "boo-editor.h"
#include "cache/editor-project-cache.h"
#include "cache/editor-assets-cache.h"
#include "cache/editor-config-cache.h"

namespace BooEditor
{
	/**
	 * @brief 工程路径
	 */
	std::string projectPath;
    /**
     * @brief 资产缓存
     */
	EditorAssetsCache *assets;
    /**
     * @brief 配置缓存
     */
	EditorConfigCache *config;
    /**
     * @brief 项目缓存
     */
    EditorProjectCache *project;
    /**
     * @brief 场景缓存
     */
    EditorSceneCache *scene;
}