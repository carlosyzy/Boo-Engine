#include "boo-editor.h"

namespace BooEditor
{
	/**
	 * @brief 工程路径
	 */
	std::string project;
    /**
     * @brief 资产缓存
     */
	EditorAssetsCache *assets;
    /**
     * @brief 配置缓存
     */
	EditorConfigCache *config;
}