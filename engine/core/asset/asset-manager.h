#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>
#include "platforms/platform.h"
#include "core/asset/asset-cache.h"
#include "core/asset/asset-struct.h"
#if defined(BOO_PLATFORM_ANDROID)
#include <android/asset_manager.h>
#endif
namespace Boo
{
	class AssetLoad;
	class AssetCache;
	class AssetBuiltin;
	class Asset;
	class AssetTask;
	class SceneAsset;
		
	/**
	 * @brief 资产管理器
	 */
	class AssetManager
	{
	private:
		std::string _assetsRoot = "";
		/**
		 * @brief 资产加载器
		 */
		AssetLoad *_assetsLoad;
		/**
		 * @brief 资产缓存
		 */
		AssetCache *_assetsCache;

		AssetBuiltin *_assetsBuiltin;
	public:
		AssetManager();
		/**
		 * @brief 初始化资产管理器
		 */
		void init();
		const std::string &getAssetsRoot();
		void setMaxLoadCount(int count);
		AssetLoad *getAssetsLoad();
		AssetCache *getAssetsCache();

		Asset *loadAsset(const std::string &uuid);
		Asset *getAsset(const std::string &uuid, bool loadIfNotFound = false);
		void unloadAsset(const std::string &uuid);

		void update(float deltaTime);

		~AssetManager();
	};

} // namespace Boo