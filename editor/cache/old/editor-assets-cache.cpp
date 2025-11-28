// #include "editor-assets-cache.h"
// #include "../boo-editor.h"

// EditorAssetsCache::EditorAssetsCache()
// {
// }
// void EditorAssetsCache::init()
// {
//     // // 初始化setting目录
//     // std::filesystem::path settingPath = (std::filesystem::path(BooEditor::projectPath) / "setting").generic_string();
//     // std::filesystem::path assetsCachePath = std::filesystem::path(settingPath) / "assets.bin";
//     // if (std::filesystem::is_regular_file(assetsCachePath))
//     // {
//     //     this->_assetsCache = FileUtil::readJsonFromBinary(assetsCachePath.string());
//     // }
//     // else
//     // {
//     //     this->_assetsCache = json::object();
//     // }
//     // std::cout << "init:Assets Cache: " << this->_assetsCache << std::endl;
//     // this->reset();
// }
// void EditorAssetsCache::reset()
// {
//     // // 初始化资源列表,和assets下的所有资源进行关联
//     // std::vector<std::string> existAssets = this->_getAssetsList();
//     // this->_insertNewAsset(existAssets);
//     // this->_delNotExistAssets(existAssets);

//     // std::filesystem::path assetsCachePath = std::filesystem::path(BooEditor::projectPath) / "setting" / "assets.bin";
//     // std::cout << "update:Assets Cache: " << this->_assetsCache << std::endl;
//     // FileUtil::saveJsonToBinary(assetsCachePath.string(), this->_assetsCache);
// }

// // void EditorAssetsCache::_insertNewAsset(std::vector<std::string> &existAssets)
// // {
// //     // 初始化资源列表,和assets下的所有资源进行关联
// //     for (const auto &relativePath : existAssets)
// //     {
// //         // 如果已经存在,则跳过
// //         if (this->_assetsCache.contains(relativePath))
// //         {
// //             continue;
// //         }
// //         else
// //         {
// //             this->_assetsCache[relativePath] = json::object();
// //             // 重新设置数据体
// //         }
// //     }
// // }
// // void EditorAssetsCache::_delNotExistAssets(std::vector<std::string> &existAssets)
// // {
// //     std::vector<std::string> keysToRemove;
// //     for (const auto &entry : this->_assetsCache.items())
// //     {
// //         if (std::find(existAssets.begin(), existAssets.end(), entry.key()) == existAssets.end())
// //         {
// //             keysToRemove.push_back(entry.key());
// //         }
// //     }
// //     for (const auto &key : keysToRemove)
// //     {
// //         this->_assetsCache.erase(key);
// //     }
// // }
// // /**
// //  * @brief 获取assets目录下的所有资源列表
// //  * @return std::vector<std::string>
// //  */
// // std::vector<std::string> EditorAssetsCache::_getAssetsList()
// // {
// //     std::filesystem::path assetsPath = (std::filesystem::path(BooEditor::projectPath) / "assets").generic_string();
// //     // 判断当前目录存在不,不存在创建
// //     if (!std::filesystem::exists(assetsPath))
// //     {
// //         std::filesystem::create_directories(assetsPath);
// //     }
// //     // 初始化资源列表,和assets下的所有资源进行关联
// //     std::vector<std::string> existAssets;
// //     for (const auto &entry : std::filesystem::recursive_directory_iterator(assetsPath))
// //     {
// //         if (std::filesystem::is_regular_file(entry))
// //         {
// //             std::string relativePath = std::filesystem::relative(entry.path(), std::filesystem::path(BooEditor::projectPath)).generic_string();
// //             existAssets.push_back(relativePath);
// //         }
// //     }
// //     return existAssets;
// // }

// EditorAssetsCache::~EditorAssetsCache()
// {
// }
