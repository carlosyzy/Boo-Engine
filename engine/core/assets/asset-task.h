
#pragma once
#include <string>
#include <functional>

class Asset;
class Shader;
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
    AssetsManager *_mgr;
    /**
     * @brief 资产缓存
     */
    AssetCache *_cache;
    /**
     * @brief 资产任务ID
     */
    int _id = 0;
    /**
     * @brief 资产路径
     */
    std::string _path;
    /**
     * @brief 资产加载任务类型
     */
    AssetTaskType _type;
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
    void _createTexture(const std::string resKey, const std::string fullPath);
    /**
     * @brief 创建GLSL着色器
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createGlslShader(const std::string resKey, const std::string fullPath);
    /**
     * @brief 创建SPIR-V着色器
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createSpirvShader(const std::string resKey, const std::string fullPath);
    /**
     * @brief 创建场景
     *
     * @param resKey 资产键值
     * @param fullPath 资产路径
     */
    void _createScene(const std::string resKey, const std::string fullPath);




    /**
     * @brief 加载完成
     */
    void _loadComplete();
    /**
     * @brief 加载错误
     */
    void _loadError();

public:
    AssetTask(AssetsManager *mgr, AssetCache *cache, int id);
    /**
     * @brief 获取资产任务ID
     * @return int 资产任务ID
     */
    const int getID()
    {
        return this->_id;
    }
    Asset *load(const std::string &path);
    template <typename T, typename Func>
    void loadAsync(const std::string &path, Func callback, T *instance)
    {
        this->_path = path;
        this->_type = AssetTaskType::AsyncOnce;
        this->_callbackOnce = [instance, callback]()
        {
            if (instance != nullptr && callback != nullptr)
            {
                (instance->*callback)();
            }
        };
    }
    template <typename T, typename Func>
    void loadASync(const std::string path, AssetLoadResult *result, Func callback, T *instance)
    {
        this->_path = path;
        this->_type = AssetTaskType::AsyncList;
        this->_result = result;
        this->_callbackList = [instance, callback](const int complete, const int all, const float progress)
        {
            if (instance != nullptr && callback != nullptr)
            {
                (instance->*callback)(complete, all, progress);
            }
        };
    }
    void run();
    void clearCallback();
    bool isComplete()
    {
        return this->_isComplete;
    }
    ~AssetTask();
};