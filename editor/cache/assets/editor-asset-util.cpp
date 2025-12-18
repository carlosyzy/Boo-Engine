#include "editor-asset-util.h"
/**
 * @brief 更新资产
 * 
 * @param assetPath 资产路径
 */
void EditorAssetUtil::updateAsset(const std::string &assetPath)
{
    // 1. 确定资源存储路径(相对于项目assets目录)
    // 2. 确定资产数据
    // 3. 通过路径查找.mate文件
    // 4. 如果有.mate文件,则只: 想存储数据到存储路径,资产存储到路径,在将资产更新到library目录下,加载资产到缓存
    // 5. 如果没有.mate文件,则: 想存储数据到存储路径,资产存储到路径,并创建.mate文件存储到目录,在将资产更新到library目录下,加载资产到缓存
}