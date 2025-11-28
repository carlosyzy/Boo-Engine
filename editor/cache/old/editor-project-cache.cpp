// #include "editor-project-cache.h"
// #include "../boo-editor.h"

// EditorProjectCache::EditorProjectCache()
// {
// }
// void EditorProjectCache::init()
// {
//     // 初始化setting目录
//     std::filesystem::path projectCachePath = (std::filesystem::path(BooEditor::projectPath) / "setting").generic_string();
//     std::filesystem::path projectCachePath = std::filesystem::path(projectCachePath) / "project.bin";
//     if (std::filesystem::is_regular_file(projectCachePath))
//     {
//         this->_projectCache = FileUtil::readJsonFromBinary(projectCachePath.generic_string());
//     }
//     else
//     {
//         this->_projectCache = json::object();
//         this->_projectCache["name"] = std::filesystem::path(BooEditor::projectPath).filename().string();
//         this->_projectCache["width"] = 1280;
//         this->_projectCache["height"] = 720;
//         this->_projectCache["launchScene"] = "";
//     }
//     std::cout << "init:Project Cache: " << this->_projectCache << std::endl;
//     this->update();
// }

// std::string EditorProjectCache::getLaunchScene()
// {
//     return this->_projectCache["launchScene"];
// };
// void EditorProjectCache::setLaunchScene(std::string launchScene)
// {
//     this->_projectCache["launchScene"] = launchScene;
//     this->update();
// };
// /**
//  * @brief 更新项目缓存
//  */
// void EditorProjectCache::update()
// {
//     std::filesystem::path projectCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "project.bin";
//     FileUtil::saveJsonToBinary(projectCachePath.generic_string(), this->_projectCache);
//     std::cout << "update:Project Cache: " << this->_projectCache << std::endl;
// }

// EditorProjectCache::~EditorProjectCache()
// {
// }
