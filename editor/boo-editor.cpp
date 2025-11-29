#include "boo-editor.h"
#include <string>

namespace BooEditor
{
    // std::string editorVersion = "0.0.1";
    std::string editorPath = "boo-editor";
	/**
	 * @brief 工程路径
	 */
	std::string projectPath;

    /**
     * @brief 工程缓存
     */
    EditorCache *cache;
}