
#pragma once
#include <string>
#include <functional>
#include <fstream>
#include <iostream>

namespace Boo
{

    class Asset;
    class ShaderAsset;
    class AssetsManager;
    class AssetCache;
    class SceneAsset;

    /**
     * @brief 资产加载任务
     */
    class AssetTask
    {

    private:
        bool _isTextureAsset(const std::string &assetPath);
        bool _isShaderVertAsset(const std::string &assetPath);
        bool _isShaderFragAsset(const std::string &assetPath);
        bool _isMaterialAsset(const std::string &assetPath);
        bool _isGLBAsset(const std::string &assetPath);
        bool _isBMFontAsset(const std::string &assetPath);
        bool _isAudioAsset(const std::string &assetPath);

        /**
         * @brief 创建贴图
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createTexture(const std::string &assetPath);
        /**
         * @brief 创建着色器
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createShader(const std::string &assetPath, const std::string &type);

        /**
         * @brief 创建材质
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createMaterial(const std::string &assetPath);

        /**
         * @brief 创建glb资产
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createGLBAsset(const std::string &assetPath);
        /**
         * @brief 创建bmfont资产
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createBMFontAsset(const std::string &assetPath);
        /**
         * @brief 创建音频资产
         *
         * @param resKey 资产键值
         * @param fullPath 资产路径
         */
        Asset *_createAudioAsset(const std::string &assetPath);
    public:
        AssetTask();
        /**
         * @brief 加载资产
         * @param rootPath 资产根路径
         * @param assetPath 资产路径:相对于资产根路径
         * @return Asset* 资产实例
         */
        Asset *load(const std::string &assetPath);

        ~AssetTask();
    };

} // namespace Boo