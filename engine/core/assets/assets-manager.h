#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>

#include "asset-struct.h"
#include "asset-load.h"
#include "asset-cache.h"
#include "texture-asset.h"
#include "material-asset.h"
#include "shader-asset.h"

// 基础图片资源
class AssetsManager
{
private:
	/**
	 * @brief 资产根目录
	 */
	std::string _assetsRoot;

	/**
	 * @brief 资产加载器
	 */
	AssetLoad *_assetsLoad;
	/**
	 * @brief 资产缓存
	 */
	AssetCache *_assetsCache;


	void _initDefaultBuiltinAssets();
public:
	AssetsManager();
	/**
	 * @brief 初始化资产管理器
	 */
	void init();

	void initAssetsDB(const std::string &path);
	// /**
	//  * @brief 获取资产数据库
	//  * @return
	//  */
	// std::unordered_map<std::string, std::vector<AssetDB>> &getAssetsDB();
	// /**
	//  * @brief 更新资产数据库
	//  * @param path 资产路径
	//  * @param configs 资产配置
	//  */
	// void updateAssetsDB(const std::string path, const std::vector<AssetDB> configs);

	void setAssetsRoot(const std::string &root);
	const std::string &getAssetsRoot();
	void setMaxLoadCount(int count);
	AssetLoad *getAssetsLoad();
	AssetCache *getAssetsCache();

	Asset *loadAsset(const std::string &uuid);
	Asset *getAsset(const std::string &uuid);
	/**
	 * @brief 通过获取场景配置
	 * @param sceneName 场景名称
	 */
	AssetDB* getSceneAssetDB(const std::string &sceneName);

	// Asset *loadByPath(const std::string &path);
	// Asset *getByPath(const std::string &path);

	// template <typename T, typename Func>
	// int loadAsync(const std::string &path, Func callback, T *instance)
	// {
	// 	return this->_assetLoad->loadAsync(path, callback, instance);
	// }
	// template <typename T, typename Func>
	// int loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
	// {
	// 	return this->_assetLoad->loadListAsync(paths, callback, instance);
	// }
	void clearLoadCall(const int loadId);

	void update(float deltaTime);

	~AssetsManager();
};