#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <iostream>
#include <filesystem>

#include "asset-load.h"
#include "asset-cache.h"
#include "texture-asset.h"
#include "material-asset.h"
#include "shader.h"

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
	AssetLoad *_assetLoad;
	/**
	 * @brief 资产缓存
	 */
	AssetCache *_cache;

public:
	AssetsManager();
	/**
	 * @brief 初始化资产管理器
	 */
	void init();

	void initAssetsDB(const std::string &path);
	/**
	 * @brief 获取资产数据库
	 * @return std::unordered_map<std::string, std::vector<AssetDB>> 资产数据库
	 */
	std::unordered_map<std::string, std::vector<AssetDB>> &getAssetsDB();
	/**
	 * @brief 更新资产数据库
	 * @param path 资产路径
	 * @param configs 资产配置
	 */
	void updateAssetsDB(const std::string &path, const std::vector<AssetDB> &configs);

	void setAssetsRoot(const std::string &root);
	const std::string &getAssetsRoot();
	
	AssetLoad *assetLoad();
	AssetCache *assetCache();
	/**
	 * @brief 加载资产
	 * @param path 资产路径
	 * @return Asset 资产对象
	 */
	Asset *load(const std::string &path);
	template <typename T, typename Func>
	int loadAsync(const std::string &path, Func callback, T *instance)
	{
		return this->_assetLoad->loadAsync(path, callback, instance);
	}
	template <typename T, typename Func>
	int loadListAsync(const std::vector<std::string> &paths, Func callback, T *instance)
	{
		return this->_assetLoad->loadListAsync(paths, callback, instance);
	}
	void setMaxLoadCount(int count);
	void clearLoadCall(const int loadId);

	Asset *get(const std::string &path);

	void update(float deltaTime);

	~AssetsManager();
};