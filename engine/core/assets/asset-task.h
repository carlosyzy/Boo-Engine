
#pragma once
#include <string>
#include <functional>

#include "../utils/time-util.h"
#include "../utils/json-util.h"

#include "asset-cache.h"
#include "asset-struct.h"

class Asset;
class ShaderAsset;
class AssetsManager;
class AssetCache;
class SceneAsset;

struct AssetLoadResult
{
    /**
     * @brief 所有资产数量
     */
    int all = 0;
    /**
     * @brief 已完成资产数量
     */
    int complete = 0;
    /**
     * @brief 加载进度
     */
    float progress = 0.0f;
};
enum class AssetTaskType
{
    /**
     * @brief 资产加载任务状态-异步加载
     */
    Sync,
    /**
     * @brief 资产加载任务状态-同步加载-一次
     */
    AsyncOnce,
    /**
     * @brief 资产加载任务状态-同步加载-列表
     */
    AsyncList,
};
/**
 * @brief 资产加载任务
 */
class AssetTask
{
private:
    // AssetsManager *_mgr;
    /**
     * @brief 资产任务ID
     */
    int _id = 0;
    // /**
    //  * @brief 资产路径
    //  */
    // std::string _assetPath;
    //  /**
    //  * @brief 资产扩展名
    //  */
    // std::string _assetExtension;
    // /**
    //  * @brief 资产加载任务类型
    //  */
    // AssetTaskType _type;
    /**
     * @brief 是否完成加载
     */
    bool _isComplete = false;
    /**
     * @brief 资产加载实例
     */
    std::function<void()> _callbackOnce;
    /**
     * @brief 资产加载回调
     */
    std::function<void(const int complete, const int all, const float progress)> _callbackList;

    AssetLoadResult *_result;

    /**
     * @brief 创建贴图
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    Asset *_createTexture(const std::string &rootPath,const std::string &assetPath);
    /**
     * @brief 创建场景
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    Asset *_createScene(const std::string &rootPath,const std::string &assetPath);

    /**
     * @brief 创建着色器
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    Asset *_createShader(const std::string &rootPath,const std::string &assetPath);

    /**
     * @brief 创建材质
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    Asset *_createMaterial(const std::string &rootPath,const std::string &assetPath);

    /**
     * @brief 加载完成
     */
    void _loadComplete();
    /**
     * @brief 加载错误
     */
    void _loadError();

public:
    AssetTask(int id);
    /**
     * @brief 获取资产任务ID
     * @return int 资产任务ID
     */
    const int getID()
    {
        return this->_id;
    }
    /**
     * @brief 加载资产
     * @param rootPath 资产根路径
     * @param assetPath 资产路径:相对于资产根路径
     * @return Asset* 资产实例
     */
    Asset *load(const std::string &rootPath,const std::string &assetPath);
    void clearCallback();
    bool isComplete()
    {
        return this->_isComplete;
    }
    ~AssetTask();
};