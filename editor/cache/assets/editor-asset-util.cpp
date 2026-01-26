// #include "editor-asset-util.h"
// #include "../../boo-editor.h"

// #include "../../../engine/boo.h"
// #include "../../../engine/core/assets/assets-manager.h"
// #include "../../../engine/core/assets/texture-asset.h"
// #include "../../../engine/core/assets/asset-cache.h"
// #include "../../../engine/core/assets/asset-struct.h"

// /**
//  * @brief 更新资产
//  *
//  * @param assetPath 资产路径
//  */
// void EditorAssetUtil::updateSceneAsset(const std::string &assetPath, const json &sceneData, const json &mateData)
// {
//     // 保存资产
//     std::string sceneStorageFullPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / assetPath).generic_string();
//     FileUtil::saveJsonToBinary(sceneStorageFullPath, sceneData);
//     // 保存资产元数据
//     EditorAssetUtil::updateAssetMeta(assetPath, mateData);
//     // 更新Library目录下的资产元数据
//     EditorAssetUtil::updateLibraryAsset(assetPath, mateData);
//     // 更新资产映射
//     EditorAssetUtil::updateAssetMap(assetPath, mateData);
// }
// void EditorAssetUtil::updateAssetMeta(const std::string &assetPath, const json &mateData)
// {
//     // 保存资产元数据
//     std::string assetMetaPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / assetPath).generic_string() + ".mate";
//     FileUtil::saveJsonToText(assetMetaPath, mateData);
// }
// void EditorAssetUtil::updateLibraryAsset(const std::string &assetPath, const json &mateData)
// {
//     // 更新Library目录下的资产
//     std::string originalAssetPath = (std::filesystem::path(BooEditor::projectPath) / "assets" / assetPath).generic_string();
//     std::string fileName = mateData["uuid"].get<std::string>() + mateData["extension"].get<std::string>();
//     std::string libraryPath = (std::filesystem::path(BooEditor::projectPath) / "library" / fileName).generic_string();
//     FileUtil::copyFile(originalAssetPath, libraryPath);
// }
// void EditorAssetUtil::updateAssetMap(const std::string &assetPath, const json &mateData)
// {
//     // 更新资产映射
//     Boo::game->assetsManager()->getAssetsCache()->_updateAssetMateCache(mateData["uuid"].get<std::string>(), mateData);
//     Boo::game->assetsManager()->getAssetsCache()->_updateAssetPathCache(mateData["uuid"].get<std::string>(), assetPath);
// }