// #pragma once
// #include <string>
// #include <filesystem>
// #include "../../../engine/core/utils/file-util.h"
// #include "../../../engine/core/utils/json-util.h"
// #include "../../../engine/core/utils/uuid-util.h"

// class EditorAssetUtil
// {
// public:
//     static void updateSceneAsset(const std::string &assetPath, const json &sceneData, const json &mateData);
//     /**
//      * 更新资产元数据
//      * @param assetPath 资产路径:相对assets的相对路径
//      * @param mateData 资产元数据
//      */
//     static void updateAssetMeta(const std::string &assetPath, const json &mateData);
//     /**
//      * 更新Library目录下的资产
//      * @param assetPath 资产路径:相对assets的相对路径
//      * @param mateData 资产元数据
//      */
//     static void updateLibraryAsset(const std::string &assetPath, const json &mateData);
//     /**
//      * 更新资产映射
//      * @param assetPath 资产路径:相对assets的相对路径
//      * @param mateData 资产映射元数据
//      */
//     static void updateAssetMap(const std::string &assetPath, const json &mateData);
// };  